#pragma once

#include "CoordinateSystem.hpp"

namespace SAMOSBOR::assembly::csys
{
	struct Csw
	{
		std::array<CoordinateSystem, 64> cs;
		uint8_t count;
	};
}