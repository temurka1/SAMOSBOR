#include "pch.h"
#include "Triangulation.h"
#include "Mesh.hpp"

using Shape = SAMOSBOR::core::occ::Shape;
using Mesh = SAMOSBOR::core::occ::Mesh;
using Triangulation = SAMOSBOR::core::occ::Triangulation;

namespace
{
    typedef opencascade::handle<Poly_Triangulation> PolyTriangulationPtr;

    struct FacesData
    {
        std::vector<TColgp_Array1OfPnt> vertices;
        std::vector<Poly_Array1OfTriangle> triangles;
        std::vector<TColgp_Array1OfDir> normals;

        std::vector<TopAbs_Orientation> orientations;
        std::vector<TopLoc_Location> locations;

        Standard_Integer verticesCount = 0;
        Standard_Integer trianglesCount = 0;
    };

    /// <summary>
    /// Calculates deflection coefficient
    /// </summary>
    Standard_Real calculate_deflection(const Shape& shape, double coefficient)
    {
        Bnd_Box total;
        BRepBndLib::Add(shape, total);

        Standard_Real xMin, yMin, zMin, xMax, yMax, zMax;
        total.Get(xMin, yMin, zMin, xMax, yMax, zMax);

        return std::max(xMax - xMin, std::max(yMax - yMin, zMax - zMin)) * coefficient;
    }

    /// <summary>
    /// Calculates face normals
    /// </summary>
    void calculate_normals(const TopoDS_Face& face, const PolyTriangulationPtr& faceTri, const TColgp_Array1OfPnt& vertices, const Poly_Array1OfTriangle triangles, TColgp_Array1OfDir* outNormals)
    {
        if (faceTri->HasNormals())
        {
            // normals pre-computed in triangulation structure
            //

            for (Standard_Integer vertexIdx = vertices.Lower(); vertexIdx <= vertices.Upper(); ++vertexIdx)
            {
                outNormals->ChangeValue(vertexIdx) = faceTri->Normal(vertexIdx);
            }

            if (face.Orientation() != TopAbs_REVERSED)
                return;

            for (Standard_Integer vertexIdx = vertices.Lower(); vertexIdx <= vertices.Upper(); ++vertexIdx)
            {
                outNormals->ChangeValue(vertexIdx).Reverse();
            }

            return;
        }

        const TopoDS_Face zeroFace = TopoDS::Face(face.Located(TopLoc_Location()));
        const Handle(Geom_Surface) surface = BRep_Tool::Surface(zeroFace);

        const Poly_ArrayOfUVNodes* nodesUv = faceTri->HasUVNodes() && !surface.IsNull() ? &faceTri->InternalUVNodes() : nullptr;
        Standard_Integer aTri[3] = { 0 };

        faceTri->AddNormals();

        for (Standard_Integer vertexIdx = vertices.Lower(); vertexIdx <= vertices.Upper(); ++vertexIdx)
        {
            if (nodesUv == nullptr || GeomLib::NormEstim(surface, nodesUv->Value(vertexIdx - 1), Precision::Confusion(), outNormals->ChangeValue(vertexIdx)) > 1)
            {
                gp_XYZ eqPlan(0.0, 0.0, 0.0);

                Poly_Connect polyConnect(faceTri);

                for (polyConnect.Initialize(vertexIdx); polyConnect.More(); polyConnect.Next())
                {
                    triangles(polyConnect.Value()).Get(aTri[0], aTri[1], aTri[2]);

                    const gp_XYZ v1(vertices(aTri[1]).Coord() - vertices(aTri[0]).Coord());
                    const gp_XYZ v2(vertices(aTri[2]).Coord() - vertices(aTri[1]).Coord());

                    const gp_XYZ vv = v1 ^ v2;
                    const Standard_Real modulus = vv.Modulus();

                    if (modulus >= Precision::Confusion())
                        eqPlan += vv / modulus;
                }

                const Standard_Real aModMax = eqPlan.Modulus();
                outNormals->ChangeValue(vertexIdx) = (aModMax > Precision::Confusion()) ? gp_Dir(eqPlan) : gp::DZ();
            }

            faceTri->SetNormal(vertexIdx, outNormals->Value(vertexIdx));
        }

        if (face.Orientation() != TopAbs_REVERSED)
            return;

        for (Standard_Integer vertexIdx = vertices.Lower(); vertexIdx <= vertices.Upper(); ++vertexIdx)
        {
            outNormals->ChangeValue(vertexIdx).Reverse();
        }
    }

    /// <summary>
    /// Triangulates faces of shape and store per face information for futher use
    /// </summary>
    FacesData get_faces_data(const Shape& shape, double coefficient, bool parallelMeshGeneration)
    {
        FacesData facesData;

        Standard_Real deflection = calculate_deflection(shape, coefficient);
        if (deflection < Precision::Confusion())
            return facesData;

        BRepMesh_IncrementalMesh mesher(shape, deflection, Standard_False, 0.5, parallelMeshGeneration);

        Standard_Integer nodesCount = 0, trianglesCount = 0;
        Standard_Integer nodeOffset = 0, triangleOffet = 0;

        for (TopExp_Explorer expl(shape, TopAbs_FACE); expl.More(); expl.Next())
        {
            const TopoDS_Face& face = TopoDS::Face(expl.Current());

            TopLoc_Location faceLocation;
            PolyTriangulationPtr faceTriangulation = BRep_Tool::Triangulation(face, faceLocation);

            if (!faceTriangulation.IsNull())
            {
                TColgp_Array1OfPnt nodes(1, faceTriangulation->NbNodes());
                for (int i = 1; i <= faceTriangulation->NbNodes(); i++)
                {
                    nodes[i] = faceTriangulation->Node(i);
                }

                Poly_Array1OfTriangle triangles(1, faceTriangulation->NbTriangles());
                for (int i = 1; i <= faceTriangulation->NbTriangles(); i++)
                {
                    triangles[i] = faceTriangulation->Triangle(i);
                }

                TColgp_Array1OfDir normals(nodes.Lower(), nodes.Upper());
                calculate_normals(face, faceTriangulation, nodes, triangles, &normals);

                facesData.vertices.push_back(nodes);
                facesData.triangles.push_back(triangles);
                facesData.normals.push_back(normals);

                facesData.orientations.push_back(face.Orientation());
                facesData.locations.push_back(faceLocation);

                nodesCount += faceTriangulation->NbNodes();
                trianglesCount += faceTriangulation->NbTriangles();
            }
        }

        facesData.verticesCount = nodesCount;
        facesData.trianglesCount = trianglesCount;

        return facesData;
    }

    /// <summary>
    /// Triangulates shape with given settings
    /// </summary>
    PolyTriangulationPtr triangulate_shape(const Shape& shape, double coefficient, bool parallelMeshGeneration)
    {
        const FacesData facesData = get_faces_data(shape, coefficient, parallelMeshGeneration);

        PolyTriangulationPtr meshTriangulation = new Poly_Triangulation(facesData.verticesCount, facesData.trianglesCount, Standard_False);

        Standard_Integer vertexOffset = 0;
        Standard_Integer triangleOffset = 0;

        for (size_t i = 0; i < facesData.triangles.size(); ++i)
        {
            const TColgp_Array1OfPnt& vertices = facesData.vertices[i];
            const TColgp_Array1OfDir& normals = facesData.normals[i];
            const TopLoc_Location& location = facesData.locations[i];
            const TopAbs_Orientation& orientation = facesData.orientations[i];
            const Poly_Array1OfTriangle& triangles = facesData.triangles[i];

            const gp_Trsf& transform = location.Transformation();

            meshTriangulation->AddNormals();

            for (Standard_Integer vertexIdx = vertices.Lower(); vertexIdx <= vertices.Upper(); ++vertexIdx)
            {
                gp_Pnt vertex = vertices(vertexIdx);
                gp_Vec normal = normals(vertexIdx).XYZ();

                Standard_Integer index = vertexIdx + vertexOffset;

                vertex.Transform(transform);
                normal.Transform(transform);

                meshTriangulation->SetNode(index, vertex);
                meshTriangulation->SetNormal(index, normal);
            }

            for (Standard_Integer triangleIdx = triangles.Lower(); triangleIdx <= triangles.Upper(); ++triangleIdx)
            {
                Poly_Triangle triangle = triangles(triangleIdx);

                Standard_Integer anId[3];
                triangle.Get(anId[0], anId[1], anId[2]);

                if (orientation == TopAbs_REVERSED)
                {
                    Standard_Integer aTmpIdx = anId[1];
                    anId[1] = anId[2];
                    anId[2] = aTmpIdx;
                }

                anId[0] += vertexOffset;
                anId[1] += vertexOffset;
                anId[2] += vertexOffset;

                triangle.Set(anId[0], anId[1], anId[2]);

                meshTriangulation->SetTriangle(triangleIdx + triangleOffset, triangle);
            }

            vertexOffset += vertices.Size();
            triangleOffset += triangles.Size();
        }

        return meshTriangulation;
    }

    /// <summary>
    /// Extracts indicies from triangulation
    /// </summary>
    std::vector<uint32_t> convert_indicies(const PolyTriangulationPtr& triangulation)
    {
        std::vector<uint32_t> indices;

        if (triangulation.IsNull())
            return indices;

        const int numTriangles = triangulation->NbTriangles();
        indices.reserve(numTriangles);

        for (int i = 1; i <= numTriangles; ++i)
        {
            int v1, v2, v3;

            triangulation->Triangle(i).Get(v1, v2, v3);

            indices.push_back(static_cast<uint32_t>(v1 - 1));
            indices.push_back(static_cast<uint32_t>(v2 - 1));
            indices.push_back(static_cast<uint32_t>(v3 - 1));
        }

        return indices;
    }

    /// <summary>
    /// Extracts normals from triangulation
    /// </summary>
    std::vector<float> convert_normals(const PolyTriangulationPtr& triangulation)
    {
        std::vector<float> normals;

        if (triangulation.IsNull())
            return normals;

        const int numVertices = triangulation->NbNodes();
        normals.reserve(numVertices);

        for (int i = 1; i <= numVertices; ++i)
        {
            gp_Dir normal = triangulation->Normal(i);

            normals.push_back(static_cast<float>(normal.X()));
            normals.push_back(static_cast<float>(normal.Y()));
            normals.push_back(static_cast<float>(normal.Z()));
        }

        return normals;
    }

    /// <summary>
    /// Extracts vertices from triangulation
    /// </summary>
    std::vector<float> convert_vertices(const PolyTriangulationPtr& triangulation)
    {
        std::vector<float> vertices;

        if (triangulation.IsNull())
            return vertices;

        const int numVertices = triangulation->NbNodes();
        vertices.reserve(numVertices);

        for (int i = 1; i <= numVertices; ++i)
        {
            const gp_Pnt& vertex = triangulation->Node(i);

            vertices.push_back(static_cast<float>(vertex.X()));
            vertices.push_back(static_cast<float>(vertex.Y()));
            vertices.push_back(static_cast<float>(vertex.Z()));
        }

        return vertices;
    }
}

Triangulation::Triangulation(const Shape& shape, double coefficient, bool parallelMeshGeneration) : _shapeTriangulation(triangulate_shape(shape, coefficient, parallelMeshGeneration))
{
}

Triangulation::~Triangulation()
{
    if (!_shapeTriangulation.IsNull())
    {
        _shapeTriangulation.Nullify();
    }
}

uint32_t Triangulation::GetTrianglesCount() const
{
    return _shapeTriangulation->NbTriangles();
}

uint32_t Triangulation::GetVerticesCount() const
{
    return _shapeTriangulation->NbNodes();
}

Mesh Triangulation::GetMesh()
{
    Mesh mesh
    {
        .vertices = convert_vertices(_shapeTriangulation),
        .normals = convert_normals(_shapeTriangulation),
        .indices = convert_indicies(_shapeTriangulation)
    };

    return mesh;
}