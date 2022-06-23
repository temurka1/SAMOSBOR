#include "pch.h"
#include "GraphId.h"
#include "AssemblyGraph.h"
#include "AssemblyGraphBuilder.h"
#include "Step_Reader.h"

using AssemblyGraphBuilder = SAMOSBOR::assembly::ref::AssemblyGraphBuilder;
using Step_Reader = SAMOSBOR::step::Step_Reader;

AssemblyGraphBuilder::AssemblyGraphBuilder(): _reader(new Step_Reader())
{

}

AssemblyGraphBuilder::~AssemblyGraphBuilder()
{
	delete _reader;
}
