#include <gtest/gtest.h>
#include <SAMOSBOR/ResultOr.hpp>
#include <SAMOSBOR/GraphId.h>
#include <SAMOSBOR/AssemblyGraph.h>
#include <SAMOSBOR/AssemblyGraphBuilder.h>

namespace core = SAMOSBOR::core;
namespace assembly = SAMOSBOR::assembly::ref;

using GraphId = assembly::GraphId;
using AssemblyGraph = assembly::AssemblyGraph;
using AssemblyGraphBuilder = assembly::AssemblyGraphBuilder;
using AssemblyGraphSettings = assembly::AssemblyGraphSettings;

TEST(AssemblyGraphBuilderTests, AssemblyTwoNodes)
{
	GraphId graphId("15107_0.20_1#0+1");

	AssemblyGraphSettings graphSettings
	{
		.inputPath = std::filesystem::path("../../../data/tool_3"),
		.extensionLength = 0
	};

	AssemblyGraphBuilder builder;
	core::ResultOr<AssemblyGraph> result = builder.Build(graphId, graphSettings);

	EXPECT_TRUE(result.Ok());

	const AssemblyGraph& graph = result.Value();

	EXPECT_EQ(0, graph.hierarchy[0]);
	EXPECT_EQ(0, graph.hierarchy[1]);

	EXPECT_EQ(0, graph.ports[0]);
	EXPECT_EQ(0, graph.ports[1]);
}

TEST(AssemblyGraphBuilderTests, AssemblyWithMultipleCsw)
{
	GraphId graphId("avtA_0.Qlwi_1.LoOYVa_2.wX_3.ZfTr_4.wNC_5#0+1.1+2@3235633.1+3@3235634.1+4@49.2+5@3235633");

	AssemblyGraphSettings graphSettings
	{
		.inputPath = std::filesystem::path("../../../data/tool_5"),
		.extensionLength = 0
	};

	AssemblyGraphBuilder builder;
	core::ResultOr<AssemblyGraph> result = builder.Build(graphId, graphSettings);

	EXPECT_TRUE(result.Ok());

	const AssemblyGraph& graph = result.Value();

	EXPECT_EQ(0, graph.hierarchy[0]);
	EXPECT_EQ(0, graph.hierarchy[1]);
	EXPECT_EQ(1, graph.hierarchy[2]);
	EXPECT_EQ(1, graph.hierarchy[3]);
	EXPECT_EQ(1, graph.hierarchy[4]);
	EXPECT_EQ(2, graph.hierarchy[5]);

	EXPECT_EQ(0, graph.ports[0]);
	EXPECT_EQ(0, graph.ports[1]);
	EXPECT_EQ(3235633, graph.ports[2]);
	EXPECT_EQ(3235634, graph.ports[3]);
	EXPECT_EQ(49, graph.ports[4]);
	EXPECT_EQ(3235633, graph.ports[5]);
}