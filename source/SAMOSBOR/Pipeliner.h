#pragma once

namespace SAMOSBOR::assembly::ref
{
	class Assembly;
	struct AssemblySettings;
}

namespace SAMOSBOR::core
{
	template <typename T> class ResultOr;
	class Result;
}

namespace SAMOSBOR::workflow
{
	namespace core = SAMOSBOR::core;
	namespace assembly = SAMOSBOR::assembly::ref;

	struct PipelineOptions
	{
		static constexpr uint32_t READ_STEP = 1 << 0;
		static constexpr uint32_t BUILD_GRAPH = 1 << 1;
		static constexpr uint32_t BUILD_ASSEMBLY = 1 << 2;

		static constexpr uint32_t EXPORT_STEP = 1 << 3;
		static constexpr uint32_t EXPORT_GLTF = 1 << 4;
	};

	class Pipeliner
	{
	public:
		Pipeliner();

		core::ResultOr<assembly::Assembly> RunAssemblyPipeline(const assembly::AssemblySettings& settings);
		core::Result RunExportPipeline(const assembly::AssemblySettings& settings, const uint32_t pipelineOptions);

		template <typename T>
		core::ResultOr<T> RunConfigurable(const assembly::AssemblySettings& settings, const uint32_t pipelineOptions);
	};
}