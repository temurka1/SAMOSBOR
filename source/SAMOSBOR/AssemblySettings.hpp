#pragma once
#include <filesystem>

namespace SAMOSBOR::assembly::ref
{
	/// <summary>
	/// Settings for assembly graph
	/// </summary>
	struct AssemblyGraphSettings
	{
		float extensionLength;
	};

	/// <summary>
	/// Settings for assembly
	/// </summary>
	struct AssemblySettings
	{
		/// <summary>
		/// Path to folder with STEP files
		/// </summary>
		std::filesystem::path dataPath;

		/// <summary>
		/// Path to folder with output assembly
		/// </summary>
		std::filesystem::path outputPath;

		/// <summary>
		/// Graph settings
		/// </summary>
		AssemblyGraphSettings graphSettings;
	};
}

