#pragma once
#include <filesystem>

namespace SAMOSBOR::assembly::ref
{
	/// <summary>
	/// Settings for assembly
	/// </summary>
	struct AssemblySettings
	{
		/// <summary>
		/// Graph Id string
		/// </summary>
		std::string graphId;

		/// <summary>
		/// Path to folder with STEP files
		/// </summary>
		std::filesystem::path dataPath;

		/// <summary>
		/// Path to folder with output assembly
		/// </summary>
		std::filesystem::path outputPath;

		/// <summary>
		/// Tool items extension length
		/// </summary>
		float extensionLength;

		/// <summary>
		/// Triangulation coefficient
		/// </summary>
		double triangulationCoefficient;
	};
}

