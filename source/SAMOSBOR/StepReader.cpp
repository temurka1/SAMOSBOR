#include "pch.h"
#include "StepReader.h"

namespace core = SAMOSBOR::core;

using StepReader = SAMOSBOR::step::ref::StepReader;
using StepFileData = SAMOSBOR::step::ref::StepFileData;

core::ResultOr<StepFileData> StepReader::Read(const std::string& filename)
{

}

StepFileData::StepFileData(const STEPControl_Reader& reader): _reader(std::move(reader))
{
}