#include "pch.h"
#include "ReadStepStage.h"
#include "StepData.h"
#include "StepReader.h"
#include "AssemblySettings.hpp"
#include "GraphId.h"

using ReadStepStage = SAMOSBOR::workflow::stages::ReadStepStage;

using GraphId = SAMOSBOR::assembly::ref::GraphId;
using AssemblySettings = SAMOSBOR::assembly::ref::AssemblySettings;

using StepData = SAMOSBOR::step::ref::StepData;
using StepReader = SAMOSBOR::step::ref::StepReader;

namespace fs = std::filesystem;
namespace core = SAMOSBOR::core;

namespace
{
	fs::path get_step_file_name(fs::path inputPath, const std::string_view& fileId)
	{
		fs::path path = inputPath / fileId;
		return path.string() + ".stp";
	}
}

ReadStepStage::ReadStepStage(const AssemblySettings& settings) : 
	Stage<std::vector<StepData>, GraphId>([&settings](const GraphId& graphId) -> core::ResultOr<std::vector<StepData>>
	{
		const std::vector<GraphId::Vertex>& vertices = graphId.Vertices();
		const std::vector<GraphId::Edge>& edges = graphId.Edges();

		std::vector<StepData> data;
		data.reserve(vertices.size());

		StepReader stepReader;

		for (size_t i = 0; i < vertices.size(); ++i)
		{
			const GraphId::Vertex& vertex = vertices[i];

			const fs::path filename = get_step_file_name(settings.dataPath.empty() ? fs::current_path() : settings.dataPath, vertex.fileId);

			ASSIGN_OR_RETURN_T(const StepData step, stepReader.Read(filename), std::vector<StepData>);
			data.push_back(step);
		}

		return core::ResultOr<std::vector<StepData>>(std::move(data));
	})
{
}