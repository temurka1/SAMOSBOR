#include <gtest/gtest.h>
#include <SAMOSBOR/SAMOSBOR.h>

namespace fs = std::filesystem;
namespace core = SAMOSBOR::core;
namespace assembly = SAMOSBOR::assembly::ref;

using AssemblySettings = assembly::AssemblySettings;
using AssemblyGraphSettings = assembly::AssemblyGraphSettings;
using AssemblyBuilder = assembly::AssemblyBuilder;

class AssemblyBuilderTests : public ::testing::Test
{
protected:
	void SetUp() override
	{
		outputDirPath = fs::path("../../../data/output/");

		if (!fs::exists(outputDirPath))
		{
			fs::create_directory(outputDirPath);
		}
		else
		{
			for (auto& path : fs::directory_iterator(outputDirPath))
			{
				fs::remove_all(path);
			}
		}
	}

	fs::path outputDirPath;
};
TEST_F(AssemblyBuilderTests, AssemblyTwoNodes)
{
	const std::string graphId("15107_0h.20_1t#0+1");

	AssemblySettings settings
	{
		.dataPath = fs::path("../../../data/tool_3"),
		.outputPath = outputDirPath / "assembly_3.stp",
		.graphSettings = AssemblyGraphSettings
		{
			.extensionLength = 0
		}
	};

	AssemblyBuilder builder;
	core::Result result = builder.Build(graphId, settings);

	EXPECT_TRUE(result.Ok());
	EXPECT_TRUE(fs::exists(settings.outputPath));
	EXPECT_TRUE(fs::file_size(settings.outputPath) > 0);
}

TEST_F(AssemblyBuilderTests, AssemblyWithMultipleCsw)
{
	const std::string graphId("avtA_0.Qlwi_1.LoOYVa_2.wX_3.ZfTr_4.wNC_5#0+1.1+2@3235633.1+3@3235634.1+4@49.2+5@3235633");

	AssemblySettings settings
	{
		.dataPath = fs::path("../../../data/tool_5"),
		.outputPath = outputDirPath / "assembly_5.stp",
		.graphSettings = AssemblyGraphSettings
		{
			.extensionLength = 0
		}
	};

	AssemblyBuilder builder;
	core::Result result = builder.Build(graphId, settings);

	EXPECT_TRUE(result.Ok());
	EXPECT_TRUE(fs::exists(settings.outputPath));
	EXPECT_TRUE(fs::file_size(settings.outputPath) > 0);
}