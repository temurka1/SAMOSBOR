#include "pch.h"
#include "Result.hpp"
#include "ResultOr.hpp"
#include "Mesh.hpp"
#include "Shape.hpp"

#include "Assembly.h"
#include "AssemblyGraph.h"
#include "AssemblySettings.hpp"
#include "AssemblyBuilder.h"

#include "Triangulation.h"

namespace core = SAMOSBOR::core;
namespace occ = SAMOSBOR::core::occ;
namespace occt = opencascade;

using Assembly = SAMOSBOR::assembly::ref::Assembly;
using AssemblyGraph = SAMOSBOR::assembly::ref::AssemblyGraph;
using AssemblyBuilder = SAMOSBOR::assembly::ref::AssemblyBuilder;
using AssemblySettings = SAMOSBOR::assembly::ref::AssemblySettings;

namespace
{
	__forceinline void transform_shapes(const AssemblyGraph& graph, std::vector<TopoDS_Shape>* output)
	{	
		// no need to transform root toolitem
		output->push_back(graph.shapes[0]);
		
		for (size_t i = 1; i < graph.shapes.size(); i++)
		{
			const occ::Shape& shape = graph.shapes[i];
			const occ::Transform& transform = graph.transforms[i];

			const TopLoc_Location loc(transform.Get());
			output->push_back(shape.Moved(loc, false));
		}
	}

	__forceinline void triangulate_shapes(const std::vector<TopoDS_Shape> shapes, const double coefficient, std::vector<occ::Mesh>* meshes)
	{
		for (const TopoDS_Shape& shape : shapes)
		{
			occ::Triangulation triangulation(shape, coefficient, true);
			meshes->push_back(triangulation.GetMesh());
		}
	}
}

core::ResultOr<Assembly> AssemblyBuilder::Build(const AssemblyGraph& graph, const AssemblySettings& settings)
{
	std::vector<TopoDS_Shape> transformedShapes;
	transformedShapes.reserve(graph.shapes.size());

	std::vector<occ::Mesh> meshes;
	meshes.reserve(graph.shapes.size());

	transform_shapes(graph, &transformedShapes);
	triangulate_shapes(transformedShapes, settings.triangulationCoefficient, &meshes);

	return Assembly(transformedShapes, meshes);
}
