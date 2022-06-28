#pragma once

#include "CoordinateSystem.h"

namespace SAMOSBOR::core::occ
{
	constexpr uint8_t MAX_CSW_COUNT = 24;

	/// <summary>
	/// Coordinate system workpiece-side
	/// </summary>
	struct Csw
	{
		std::array<CoordinateSystem, MAX_CSW_COUNT> cs;
		uint8_t count;
	};
}