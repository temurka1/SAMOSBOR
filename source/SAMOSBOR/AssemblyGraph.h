#pragma once

#include "Transform.h"
#include "CoordinateSystem.h"
#include "Shape.hpp"
#include "Csw.hpp"

using Csw = SAMOSBOR::core::occ::Csw;
using Shape = SAMOSBOR::core::occ::Shape;
using Transform = SAMOSBOR::core::occ::Transform;
using CoordinateSystem = SAMOSBOR::core::occ::CoordinateSystem;

namespace SAMOSBOR::assembly::ref
{
	/// <summary>
	/// Represents tool items assembly graph
	/// </summary>
	struct AssemblyGraph
	{
		std::vector<uint8_t> hierarchy;
		std::vector<uint32_t> ports;

		std::vector<Shape> shapes;
		std::vector<Transform> transforms;

		std::vector<CoordinateSystem> pcs;
		std::vector<CoordinateSystem> mcs;
		std::vector<Csw> csw;

		AssemblyGraph() = default;
		AssemblyGraph(const size_t toolItemsCount);
	};
}

