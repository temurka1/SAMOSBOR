#include <gtest/gtest.h>
#include <SAMOSBOR/GraphId.h>

using GraphId = SAMOSBOR::assembly::ref::GraphId;

TEST(GraphIdParsingTests, SimpleParsingCase)
{
	std::string graphStr("holder001_0h.turning001_1t.insert_2i#0+1.1+2");

	GraphId graphId(graphStr);

	EXPECT_EQ(3, graphId.Vertices().size());
	EXPECT_EQ(2, graphId.Edges().size());
}