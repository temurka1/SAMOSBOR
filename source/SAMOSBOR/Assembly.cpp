#include "pch.h"
#include "Assembly.h"

using Assembly = SAMOSBOR::assembly::ref::Assembly;

using Mesh = SAMOSBOR::core::occ::Mesh;
using Shape = SAMOSBOR::core::occ::Shape;

Assembly::Assembly(const std::vector<Shape>& shapes, std::vector<Mesh>& meshes): _shapes(std::move(shapes)), _meshes(std::move(meshes))
{

}

const std::vector<Shape>& Assembly::GetShapes() const
{
	return _shapes;
}

const std::vector<Mesh>& Assembly::GetMeshes() const
{
	return _meshes;
}