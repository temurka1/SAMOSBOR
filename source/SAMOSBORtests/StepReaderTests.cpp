#include <gtest/gtest.h>
#include <filesystem>
#include <SAMOSBOR/ResultOr.hpp>
#include <SAMOSBOR/StepReader.h>
#include <SAMOSBOR/CoordinateSystem.h>
#include <SAMOSBOR/Shape.hpp>
#include <SAMOSBOR/Csw.hpp>

namespace core = SAMOSBOR::core;
namespace step = SAMOSBOR::step::ref;

using StepData = step::StepData;
using StepReader = step::StepReader;

using Csw = core::occ::Csw;
using Shape = core::occ::Shape;
using CoordinateSystem = core::occ::CoordinateSystem;

TEST(StepReaderTests, OpenStepFile)
{
	std::filesystem::path filePath = std::filesystem::path("../../../data/tool_1/3.76045R028V.STP");

	StepReader reader;
	core::ResultOr<StepData> result = reader.Read(filePath);

	EXPECT_EQ(true, result.Ok());
}

TEST(StepReaderTests, CorrectCoordinateSystems)
{
	std::filesystem::path filePath = std::filesystem::path("../../../data/tool_1/5.34280R028080.STP");
	
	StepReader reader;
	core::ResultOr<StepData> result = reader.Read(filePath);

	Csw csw = result.Value().Csw();
	CoordinateSystem mcs = result.Value().Mcs();
	CoordinateSystem pcs = result.Value().Pcs();

	EXPECT_EQ(1, csw.count);
	EXPECT_EQ(1.7763568394002501e-15, csw.cs[0].origin.X());
	EXPECT_EQ(0.0, csw.cs[0].origin.Y());
	EXPECT_EQ(-89.975, csw.cs[0].origin.Z());

	EXPECT_EQ(0.0, pcs.origin.X());
	EXPECT_EQ(0.0, pcs.origin.Y());
	EXPECT_EQ(0.0, pcs.origin.Z());

	EXPECT_EQ(0.0, mcs.origin.X());
	EXPECT_EQ(0.0, mcs.origin.Y());
	EXPECT_EQ(0.0, mcs.origin.Z());
}