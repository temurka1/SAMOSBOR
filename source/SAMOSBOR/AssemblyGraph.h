#pragma once
#include "GraphId.h"

namespace SAMOSBOR::assembly::ref
{
	/// <summary>
	/// Represents tool items assembly graph in cache friendly way
	/// </summary>
	struct AssemblyGraph
	{
		GraphId graphId;

		std::vector<uint16_t> hierarchy;
		std::vector<gp_Trsf> transforms;
	};
}

