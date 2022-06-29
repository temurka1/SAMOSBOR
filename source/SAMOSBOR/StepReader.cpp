#include "pch.h"
#include "StepReader.h"
#include "Csw.hpp"
#include "ResultOr.hpp"

namespace core = SAMOSBOR::core;

using StatusCode = core::Result::StatusCode;

using Csw = core::occ::Csw;
using Shape = core::occ::Shape;
using CoordinateSystem = core::occ::CoordinateSystem;

using StepData = SAMOSBOR::step::ref::StepData;
using StepReader = SAMOSBOR::step::ref::StepReader;

StepData::StepData(const STEPControl_Reader& reader): _shape(reader.Shape())
{
	auto model = reader.StepModel();
	auto iterator = model->Entities();

	for (; iterator.More(); iterator.Next())
	{
		const auto& entity = iterator.Value();

		if (entity->IsKind(STANDARD_TYPE(StepGeom_Axis2Placement3d)))
		{
			auto stepAxis = Handle(StepGeom_Axis2Placement3d)::DownCast(entity);
			auto geomAxis = StepToGeom::MakeAxis2Placement(stepAxis);

			gp_Ax3 axis(geomAxis->Ax2());
			std::string key(stepAxis->Name()->ToCString());

			_csmap[key] = CoordinateSystem{ .origin = gp_Vec(axis.Location().XYZ()), .uAxis = gp_Vec(axis.Direction().XYZ()), .vAxis = gp_Vec(axis.XDirection().XYZ()) };
		}
	}
}

StepData::StepData(const StepData& other) : _shape(other._shape), _csmap(other._csmap)
{
}

Shape StepData::Shape() const
{
	return _shape;
}

CoordinateSystem StepData::Pcs() const
{
	std::string key("PCS");
	return _csmap.contains(key) ? _csmap.at(key) : CoordinateSystem();
}

CoordinateSystem StepData::Mcs() const
{
	std::string key("MCS");
	return _csmap.contains(key) ? _csmap.at(key) : CoordinateSystem();
}

Csw StepData::Csw() const
{
	uint8_t count = 0;
	std::array<CoordinateSystem, core::occ::MAX_CSW_COUNT> csws;

	for (const auto& cs : _csmap)
	{
		if (cs.first.starts_with("CSW"))
		{
			csws[count++] = cs.second;
		}
	}

	return core::occ::Csw{ .cs = std::move(csws), .count = count };
}

core::ResultOr<StepData> StepReader::Read(const std::filesystem::path& filepath, bool printCheckload)
{
	std::string modelFileName = filepath.string();

	STEPControl_Reader reader;
	IFSelect_ReturnStatus status = reader.ReadFile(modelFileName.c_str());

	if (status != IFSelect_RetDone)
	{
		return core::ResultOr<StepData>(core::Result(StatusCode::ERROR));
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
		return core::ResultOr<StepData>(core::Result(StatusCode::ERROR));
	}

	StepData stepData(reader);
	return core::ResultOr<StepData>(std::move(stepData));
}

