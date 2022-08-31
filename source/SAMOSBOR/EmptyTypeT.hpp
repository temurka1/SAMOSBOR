#pragma once

namespace SAMOSBOR::core
{
	struct EmptyTypeT
	{
		EmptyTypeT() { }
		EmptyTypeT(const EmptyTypeT& rhs) { }
		EmptyTypeT(EmptyTypeT&& rhs) noexcept { }
	};
}