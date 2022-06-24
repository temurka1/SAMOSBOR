#pragma once

namespace SAMOSBOR::assembly::ref
{
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

		GraphId(const std::string graphString);

		std::vector<Vertex> Vertices() const;
		std::vector<Edge> Edges() const;
	private:
		std::vector<Vertex> _vertices;
		std::vector<Edge> _edges;

		std::string _graphString;
	};
}

