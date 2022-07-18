#pragma once
#include <Standard_Handle.hxx>
#include "Shape.hpp"

class Poly_Triangulation;

namespace SAMOSBOR::core::occ
{
	struct Mesh;

	/// <summary>
	/// Triangulates given shape
	/// </summary>
	class Triangulation
	{
	public:
		Triangulation(const Shape& shape, double coefficient, bool parallelMeshGeneration);
		~Triangulation();

		uint32_t GetTrianglesCount() const;
		uint32_t GetVerticesCount() const;

		Mesh GetMesh();
	private:
		opencascade::handle<Poly_Triangulation> _shapeTriangulation;
	};
}
