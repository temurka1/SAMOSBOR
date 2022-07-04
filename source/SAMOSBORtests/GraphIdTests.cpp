#include <gtest/gtest.h>
#include <SAMOSBOR/GraphId.h>

using GraphId = SAMOSBOR::assembly::ref::GraphId;

TEST(GraphIdParsingTests, SimpleParsingCase)
{
	GraphId graphId("holder001_0h.turning001_1t.insert_2i#0+1.1+2");

	EXPECT_EQ(3, graphId.Vertices().size());
	EXPECT_EQ(2, graphId.Edges().size());
}

TEST(GraphIdParsingTests, ParseNoEdges)
{
	GraphId graphId("holder001_0h");

	EXPECT_EQ(1, graphId.Vertices().size());
	EXPECT_EQ(0, graphId.Edges().size());
}

TEST(GraphIdParsingTests, TwoInserts)
{
	GraphId graphId("holder001_0h.turning001_1t.insert_2i.insert_3i#0+1.1+2@31.1+3@32");

	EXPECT_EQ(4, graphId.Vertices().size());
	EXPECT_EQ(3, graphId.Edges().size());
}