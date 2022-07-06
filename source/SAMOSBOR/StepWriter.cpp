#include "pch.h"
#include "StepWriter.h"
#include "Result.hpp"

namespace fs = std::filesystem;
namespace core = SAMOSBOR::core;

using StepWriter = SAMOSBOR::step::ref::StepWriter;

core::Result StepWriter::Write(const fs::path& output, const std::vector<TopoDS_Shape>& data)
{
	STEPControl_Writer writer;

	if (!Interface_Static::SetIVal("write.step.assembly", 1))
	{
		return core::Result(core::Result::StatusCode::ERROR, "Can't set write.step.assembly flag");
	}

	for (const TopoDS_Shape& shape : data)
	{
		writer.Transfer(shape, STEPControl_AsIs);
	}

	IFSelect_ReturnStatus status = writer.Write(output.string().c_str());

	return core::Result(status == IFSelect_ReturnStatus::IFSelect_RetDone ? core::Result::OK : core::Result::ERROR);
}