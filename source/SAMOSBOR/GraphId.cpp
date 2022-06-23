#include "pch.h"
#include "ResultOr.hpp"
#include "GraphId.h"

using namespace SAMOSBOR::core;
using namespace SAMOSBOR::assembly::ref;

const wchar_t GRAPH_SEPARATOR = L'#';
const wchar_t EDGE_SEPARATOR = L'+';
const wchar_t VERTEX_SEPARATOR = L'.';
const wchar_t FILE_ID_SEPARATOR = L'_';

const wchar_t INSERT_CODE = L'i';
const wchar_t HOLDER_CODE = L'h';
const wchar_t ADAPTER_CODE = L'a';
const wchar_t TURNING_CODE = L't';

namespace
{
	ResultOr<size_t> HasEdges(const std::wstring_view graphString)
	{
		const size_t edgesStart = graphString.find(GRAPH_SEPARATOR);
		return ResultOr<size_t>(edgesStart != std::wstring::npos ? Result::OK : Result::NOT_OK, edgesStart);
	}
}

GraphId::GraphId(const std::wstring_view graphString)
{
	ResultOr<size_t> hasEdges = HasEdges(graphString);

	const std::wstring_view vertexListString = graphString.substr(0, graphString.find(GRAPH_SEPARATOR));
	const std::wstring_view edgesListString = hasEdges.Ok() ? graphString.substr(hasEdges.Value(), std::wstring::npos) : std::wstring();


}
