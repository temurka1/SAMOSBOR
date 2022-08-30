#include "pch.h"
#include "StepData.h"
#include "StepReader.h"
#include "ResultOr.hpp"

namespace core = SAMOSBOR::core;

using StepData = SAMOSBOR::step::ref::StepData;
using StepReader = SAMOSBOR::step::ref::StepReader;

StepReader::StepReader()
{
	Interface_Static::SetIVal("read.step.root.transformation", 0);

	//// disable parametric curves
	Interface_Static::SetIVal("write.surfacecurve.mode", 0);
	Interface_Static::SetIVal("write.step.assembly", 1);
}

StepReader::~StepReader()
{
}

core::ResultOr<StepData> StepReader::Read(const std::filesystem::path& filepath, bool printCheckload)
{
	std::string modelFileName = filepath.string();

	STEPControl_Reader reader;
	IFSelect_ReturnStatus status = reader.ReadFile(modelFileName.c_str());

	if (status != IFSelect_RetDone)
	{
		return core::ResultOr<StepData>(core::Result(core::Result::StatusCode::ERROR));
	}

	reader.TransferRoots();

	if (printCheckload)
	{
		reader.PrintCheckLoad(true, IFSelect_ItemsByEntity);
		reader.PrintCheckTransfer(true, IFSelect_ItemsByEntity);
	}

	Standard_Integer numberOfShapes = reader.NbShapes();
	if (numberOfShapes == 0)
	{
		return core::ResultOr<StepData>(core::Result(core::Result::StatusCode::ERROR, "No shapes in STEP file"));
	}

	StepData stepData(reader.StepModel(), reader.OneShape());
	return core::ResultOr<StepData>(std::move(stepData));
}