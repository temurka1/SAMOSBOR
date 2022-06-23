#pragma once

namespace SAMOSBOR::assembly::ref
{
	/// <summary>
	/// The graph id is a set of vertices and/or edges encoded in string
    /// For more information about graph id design see https://github.com/temurka1/SAMOSBOR/issues/1
	/// </summary>
	struct GraphId
	{
	public:
		GraphId(const std::wstring_view graphString);
	};
}

