#include <gtest/gtest.h>
#include <SAMOSBOR/GraphId.h>

using GraphId = SAMOSBOR::assembly::ref::GraphId;

TEST(GraphIdParsingTests, SimpleParsingCase)
{
	GraphId graphId("holder001_0h.turning001_1t.insert_2i#0+1.1+2");

	const std::vector<GraphId::Vertex> vertices = graphId.Vertices();
	const std::vector<GraphId::Edge> edges = graphId.Edges();

	EXPECT_EQ(3, vertices.size());
	EXPECT_EQ(2, edges.size());

	EXPECT_EQ(0, edges[0].port);
	EXPECT_EQ(0, edges[0].from);
	EXPECT_EQ(1, edges[0].to);

	EXPECT_EQ(0, edges[1].port);
	EXPECT_EQ(1, edges[1].from);
	EXPECT_EQ(2, edges[1].to);
}

TEST(GraphIdParsingTests, ParseNoEdges)
{
	GraphId graphId("holder001_0h");

	const std::vector<GraphId::Vertex> vertices = graphId.Vertices();
	const std::vector<GraphId::Edge> edges = graphId.Edges();

	EXPECT_EQ(1, vertices.size());
	EXPECT_EQ(0, edges.size());
}

TEST(GraphIdParsingTests, TwoInserts)
{
	GraphId graphId("holder001_0h.turning001_1t.insert_2i.insert_3i#0+1.1+2@31.1+3@32");

	const std::vector<GraphId::Vertex> vertices = graphId.Vertices();
	const std::vector<GraphId::Edge> edges = graphId.Edges();

	EXPECT_EQ(4, vertices.size());
	EXPECT_EQ(3, edges.size());

	EXPECT_EQ(0, edges[0].port);
	EXPECT_EQ(0, edges[0].from);
	EXPECT_EQ(1, edges[0].to);

	EXPECT_EQ(31, edges[1].port);
	EXPECT_EQ(1, edges[1].from);
	EXPECT_EQ(2, edges[1].to);

	EXPECT_EQ(32, edges[2].port);	
	EXPECT_EQ(1, edges[2].from);
	EXPECT_EQ(3, edges[2].to);
}