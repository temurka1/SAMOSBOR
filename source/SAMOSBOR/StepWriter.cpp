#include "pch.h"
#include "StepWriter.h"
#include "Result.hpp"

namespace fs = std::filesystem;
namespace occt = opencascade;
namespace core = SAMOSBOR::core;

using StepWriter = SAMOSBOR::step::ref::StepWriter;

StepWriter::StepWriter() : _writer(new STEPControl_Writer)
{
	// disable parametric curves
	Interface_Static::SetIVal("write.surfacecurve.mode", 0);
	Interface_Static::SetIVal("write.step.assembly", 1);
}

StepWriter::~StepWriter()
{
	delete _writer;
}

core::Result StepWriter::Write(const fs::path& output, const std::vector<TopoDS_Shape>& data)
{
	for (const TopoDS_Shape& shape : data)
	{
		_writer->Transfer(shape, STEPControl_ManifoldSolidBrep);
	}

	IFSelect_ReturnStatus status = _writer->Write(output.string().c_str());

	return core::Result(status == IFSelect_ReturnStatus::IFSelect_RetDone ? core::Result::OK : core::Result::ERROR);
}