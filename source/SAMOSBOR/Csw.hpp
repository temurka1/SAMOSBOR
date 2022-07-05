#pragma once
#include <slot_map/slot_map.h>
#include "CoordinateSystem.h"

namespace SAMOSBOR::core::occ
{
	/// <summary>
	/// Coordinate system workpiece-side
	/// </summary>
	struct Csw
	{
		typedef dod::slot_map_key64<CoordinateSystem> Key;
		typedef dod::slot_map <CoordinateSystem, Key> Map;

		typedef Key::tag_t Port;
		typedef std::unordered_map<Port, Key> PortMap;

		Map cs;
		PortMap ports;
	};
}