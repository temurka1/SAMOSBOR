#include "pch.h"
#include "ResultOr.hpp"
#include "StringUtils.h"
#include "GraphId.h"

namespace core = SAMOSBOR::core;
namespace utils = SAMOSBOR::core::utils;

using GraphId = SAMOSBOR::assembly::ref::GraphId;

namespace
{
	inline constexpr char GRAPH_SEPARATOR = '#';
	inline constexpr char EDGE_SEPARATOR = '+';
	inline constexpr char VERTEX_SEPARATOR = '.';
	inline constexpr char FILE_ID_SEPARATOR = '_';

	inline constexpr char INSERT_CODE = 'i';
	inline constexpr char HOLDER_CODE = 'h';
	inline constexpr char ADAPTER_CODE = 'a';
	inline constexpr char TURNING_CODE = 't';

	core::ResultOr<size_t> has_edges(const std::string_view graphString)
	{
		const size_t edgesStart = graphString.find(GRAPH_SEPARATOR);
		return core::ResultOr<size_t>(edgesStart != std::string::npos ? core::Result::OK : core::Result::NOT_OK, edgesStart);
	}

	uint8_t string_to_tooltype(const std::string_view str)
	{
		switch (str.data()[0])
		{
			case 'a':
				return 1;
			case 'h':
				return 2;
			case 't':
				return 3;
			case 'i':
				return 4;
		}

		return 0;
	}

	std::vector<GraphId::Vertex> build_vertex_list(const std::string_view graphString, const core::ResultOr<size_t>& hasEdges)
	{
		std::string_view vertexStr = graphString.substr(0, hasEdges.Ok() ? hasEdges.Value() : graphString.size());
		std::vector<std::string_view> vertexTokens = utils::str_split(vertexStr, VERTEX_SEPARATOR);

		std::vector<GraphId::Vertex> vertexList(vertexTokens.size());
		std::vector<std::string_view> vertexComponents;

		vertexComponents.reserve(2);

		for (size_t i = 0; i < vertexTokens.size(); ++i)
		{
			utils::str_split_inplace(vertexTokens[i], FILE_ID_SEPARATOR, &vertexComponents);

			std::string_view fileIdStr = vertexComponents[0];
			std::string_view indexStr = vertexComponents[1].substr(0, vertexComponents[1].size() - 1);
			std::string_view toolTypeStr = vertexComponents[1].substr(vertexComponents[1].size() - 1, 1);

			uint8_t index;
			std::from_chars(indexStr.data(), indexStr.data() + indexStr.size(), index);

			uint8_t toolType = string_to_tooltype(toolTypeStr);

			vertexList[i] = GraphId::Vertex{ .fileId = vertexComponents[0], .index = index, .toolType = toolType };

			vertexComponents.clear();
		}

		return vertexList;
	}

	std::vector<GraphId::Edge> build_edge_list(const std::string_view graphString, const core::ResultOr<size_t>& hasEdges)
	{
		if (!hasEdges.Ok())
		{
			return std::vector<GraphId::Edge>();
		}

		std::string_view edgeStr = graphString.substr(hasEdges.Value() + 1, graphString.size() - hasEdges.Value());
		std::vector<std::string_view> edgeTokens = utils::str_split(edgeStr, VERTEX_SEPARATOR);

		std::vector<GraphId::Edge> edgeList(edgeTokens.size());
		std::vector<std::string_view> edgeComponents;

		edgeComponents.reserve(2);

		for (size_t i = 0; i < edgeTokens.size(); ++i)
		{
			utils::str_split_inplace(edgeTokens[i], EDGE_SEPARATOR, &edgeComponents);

			uint8_t first;
			std::from_chars(edgeComponents[0].data(), edgeComponents[0].data() + edgeComponents[0].size(), first);

			uint8_t second;
			std::from_chars(edgeComponents[1].data(), edgeComponents[1].data() + edgeComponents[1].size(), second);

			edgeList[i] = GraphId::Edge { .from = first, .to = second };

			edgeComponents.clear();
		}

		return edgeList;
	}
}

GraphId::GraphId(const std::string graphString) : _graphString(graphString)
{
	const core::ResultOr<size_t> hasEdges = has_edges(graphString);

	_vertices = build_vertex_list(_graphString, hasEdges);
	_edges = build_edge_list(_graphString, hasEdges);
}

const std::vector<GraphId::Vertex>& GraphId::Vertices() const
{
	return _vertices;
}

const std::vector<GraphId::Edge>& GraphId::Edges() const
{
	return _edges;
}
