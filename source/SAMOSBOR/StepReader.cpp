#include "pch.h"
#include "StepData.h"
#include "StepReader.h"
#include "ResultOr.hpp"

namespace core = SAMOSBOR::core;

using StepData = SAMOSBOR::step::ref::StepData;
using StepReader = SAMOSBOR::step::ref::StepReader;

core::ResultOr<StepData> StepReader::Read(const std::filesystem::path& filepath, bool printCheckload)
{
	std::string modelFileName = filepath.string();

	STEPControl_Reader reader;
	IFSelect_ReturnStatus status = reader.ReadFile(modelFileName.c_str());

	if (status != IFSelect_RetDone)
	{
		return core::ResultOr<StepData>(core::Result(core::Result::StatusCode::ERROR));
	}

	Standard_Integer numberOfRoots = reader.NbRootsForTransfer();

	if (printCheckload)
	{
		reader.PrintCheckLoad(false, IFSelect_ItemsByEntity);
		reader.PrintCheckTransfer(false, IFSelect_ItemsByEntity);
	}

	for (auto n = 1; n <= numberOfRoots; n++)
	{
		reader.TransferRoot(n);
	}

	Standard_Integer numberOfShapes = reader.NbShapes();	
	if (numberOfShapes == 0)
	{
		return core::ResultOr<StepData>(core::Result(core::Result::StatusCode::ERROR));
	}

	StepData stepData(reader.Model(), reader.Shape());
	return core::ResultOr<StepData>(std::move(stepData));
}