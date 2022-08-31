#include <gtest/gtest.h>
#include <SAMOSBOR/SAMOSBOR.h>

namespace fs = std::filesystem;

namespace core = SAMOSBOR::core;
namespace step = SAMOSBOR::step::ref;
namespace assembly = SAMOSBOR::assembly::ref;

using StepData = step::StepData;

using GraphId = assembly::GraphId;
using AssemblyGraph = assembly::AssemblyGraph;
using AssemblyGraphBuilder = assembly::AssemblyGraphBuilder;
using AssemblySettings = assembly::AssemblySettings;

TEST(AssemblyGraphBuilderTests, AssemblyTwoNodes)
{
	GraphId graphId("15107_0.20_1#0+1");

	AssemblySettings graphSettings
	{
		.graphId = "15107_0.20_1#0+1",
		.dataPath = fs::path("../../../data/tool_3"),
		.outputPath = fs::path(""),
		.extensionLength = 0,
	};

	AssemblyGraphBuilder builder;
	core::ResultOr<AssemblyGraph> result = builder.Build(graphId, data);

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

	AssemblySettings graphSettings
	{
		.graphId = "avtA_0.Qlwi_1.LoOYVa_2.wX_3.ZfTr_4.wNC_5#0+1.1+2@3235633.1+3@3235634.1+4@49.2+5@3235633",
		.dataPath = fs::path("../../../data/tool_5"),
		.outputPath = fs::path(""),
		.extensionLength = 0,
	};

	AssemblyGraphBuilder builder;
	core::ResultOr<AssemblyGraph> result = builder.Build(graphId, );

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