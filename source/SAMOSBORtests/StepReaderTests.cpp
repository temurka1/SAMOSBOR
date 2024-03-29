#include <gtest/gtest.h>
#include <filesystem>
#include <SAMOSBOR/SAMOSBOR.h>

namespace fs = std::filesystem;

namespace core = SAMOSBOR::core;
namespace step = SAMOSBOR::step::ref;

using StepData = step::StepData;
using StepReader = step::StepReader;

using Csw = core::occ::Csw;
using Shape = core::occ::Shape;
using CuttingEdge = core::occ::CuttingEdge;
using CoordinateSystem = core::occ::CoordinateSystem;

TEST(StepReaderTests, OpenStepFile)
{
	std::filesystem::path filePath = fs::path("../../../data/tool_1/3.76045R028V.STP");

	StepReader reader;
	core::ResultOr<StepData> result = reader.Read(filePath);

	EXPECT_EQ(true, result.Ok());
}

TEST(StepReaderTests, CorrectCoordinateSystems)
{
	std::filesystem::path filePath = fs::path("../../../data/tool_1/5.34280R028080.STP");
	
	StepReader reader;
	core::ResultOr<StepData> result = reader.Read(filePath);

	Csw csw = result.Value().Csw();
	CoordinateSystem mcs = result.Value().Mcs();
	CoordinateSystem pcs = result.Value().Pcs();

	EXPECT_EQ(false, csw.cs.empty());
	EXPECT_EQ(false, csw.ports.empty());

	auto it = csw.ports.begin();
	Csw::Key cswkey = it->second;

	CoordinateSystem* cswCs = csw.cs.get(cswkey);

	EXPECT_EQ(1.7763568394002501e-15, cswCs->origin.X());
	EXPECT_EQ(0.0, cswCs->origin.Y());
	EXPECT_EQ(-89.975, cswCs->origin.Z());

	EXPECT_EQ(0.0, pcs.origin.X());
	EXPECT_EQ(0.0, pcs.origin.Y());
	EXPECT_EQ(0.0, pcs.origin.Z());

	EXPECT_EQ(0.0, mcs.origin.X());
	EXPECT_EQ(0.0, mcs.origin.Y());
	EXPECT_EQ(0.0, mcs.origin.Z());
}

TEST(StepReaderTests, CuttingEdgePartExists)
{
	std::filesystem::path filePath = fs::path("../../../data/tool_6/UKDV1000X4CV.STP");

	StepReader reader;
	core::ResultOr<StepData> result = reader.Read(filePath);

	EXPECT_TRUE(result.Ok());

	const CuttingEdge ce = result.Value().CuttingEdge();

	EXPECT_FALSE(ce.IsNull());
}