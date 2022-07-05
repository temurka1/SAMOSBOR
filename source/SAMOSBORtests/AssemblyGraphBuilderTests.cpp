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
	GraphId graphId("3.77000R051V.STP");
	AssemblyGraphBuilder builder;

	core::ResultOr<AssemblyGraph> graph = builder.Build(graphId, AssemblyGraphSettings{ .extensionLength = 0 });
}

TEST(AssemblyGraphBuilderTests, AssemblyThreeNodes)
{

}

TEST(AssemblyGraphBuilderTests, AssemblyWithMultipleCsw)
{

}

TEST(AssemblyGraphBuilderTests, AssemblyWithExtensionLengthSet)
{

}