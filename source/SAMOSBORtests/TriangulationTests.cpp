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
using Mesh = core::occ::Mesh;
using CoordinateSystem = core::occ::CoordinateSystem;

using Triangulation = core::occ::Triangulation;

TEST(TriangulationTests, TriangulateShapeInStepFile)
{
	std::filesystem::path filePath = fs::path("../../../data/tool_1/3.76045R028V.STP");

	StepReader reader;
	core::ResultOr<StepData> result = reader.Read(filePath);

	Triangulation tri(result.Value().Shape(), 0.3, true);

	Mesh mesh = tri.GetMesh();

	EXPECT_EQ(1725, tri.GetTrianglesCount());
	EXPECT_EQ(1900, tri.GetVerticesCount());

	EXPECT_EQ(mesh.vertices.size(), 3 * tri.GetVerticesCount());
}