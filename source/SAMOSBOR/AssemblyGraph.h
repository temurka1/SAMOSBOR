#pragma once

#include "CoordinateSystem.hpp"
#include "Csw.hpp"

using Csw = SAMOSBOR::assembly::csys::Csw;
using CoordinateSystem = SAMOSBOR::assembly::csys::CoordinateSystem;

namespace SAMOSBOR::assembly::ref
{
	/// <summary>
	/// Represents tool items assembly graph
	/// </summary>
	struct AssemblyGraph
	{
		std::vector<uint16_t> hierarchy;
		
		std::vector<Shape> shapes;
		std::vector<Transform> transforms;

		std::vector<CoordinateSystem> pcs;
		std::vector<CoordinateSystem> mcs;
		std::vector<Csw> csw;
	};
}

