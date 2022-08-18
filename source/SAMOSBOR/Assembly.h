#pragma once
#include "Shape.hpp"
#include "Mesh.hpp"

namespace SAMOSBOR::assembly::ref
{
	namespace occ = SAMOSBOR::core::occ;

	class Assembly
	{
	public:
		Assembly() = default;
		Assembly(const std::vector<occ::Shape>& shapes, std::vector<occ::Mesh>& meshes);

		const std::vector<occ::Shape>& GetShapes() const;
		const std::vector<occ::Mesh>& GetMeshes() const;
	private:
		std::vector<occ::Shape> _shapes;
		std::vector<occ::Mesh> _meshes;
	};
}
