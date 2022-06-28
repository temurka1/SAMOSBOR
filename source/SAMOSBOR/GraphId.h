#pragma once

namespace SAMOSBOR::assembly::ref
{
	/// <summary>
	/// The graph id is a set of vertices and/or edges encoded in string. 
	/// Used to represent structure of tool assembly in short and clear way.
	/// For more info take a look at https://github.com/temurka1/SAMOSBOR/issues/1
	/// </summary>
	struct GraphId
	{
	public:
		struct Vertex
		{
			std::string_view fileId;
			uint8_t index;
			uint8_t toolType;
		};

		struct Edge
		{
			uint8_t from;
			uint8_t to;
		};

		[[nodiscard]] const std::vector<Vertex>& Vertices() const;
		[[nodiscard]] const std::vector<Edge>& Edges() const;

		GraphId(const std::string graphString);
	private:
		std::vector<Vertex> _vertices;
		std::vector<Edge> _edges;

		std::string _graphString;
	};
}

