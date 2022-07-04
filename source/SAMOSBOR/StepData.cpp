#include "pch.h"
#include "StepData.h"
#include "Csw.hpp"

using StepData = SAMOSBOR::step::ref::StepData;
using Shape = SAMOSBOR::core::occ::Shape;
using Csw = SAMOSBOR::core::occ::Csw;
using CoordinateSystem = SAMOSBOR::core::occ::CoordinateSystem;

StepData::StepData(const Handle(Interface_InterfaceModel)& model, const TopoDS_Shape& shape) : _shape(shape)
{
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

			_csmap[key] = CoordinateSystem
			{ 
				.origin = gp_Vec(axis.Location().XYZ()), 
				.uAxis = gp_Vec(axis.Direction().XYZ()), 
				.vAxis = gp_Vec(axis.XDirection().XYZ()) 
			};
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
	auto search = _csmap.find("PCS");
	return search != _csmap.end() ? search->second : CoordinateSystem();
}

CoordinateSystem StepData::Mcs() const
{
	auto search = _csmap.find("MCS");
	return search != _csmap.end() ? search->second : CoordinateSystem();
}

Csw StepData::Csw() const
{
	core::occ::Csw csw;

	for (const auto& cs : _csmap)
	{
		if (cs.first.starts_with("CSW"))
		{
			Csw::Key key = csw.cs.emplace(cs.second);
			Csw::Port port = ;

			key.set_tag(port);
			csw.ports.emplace(port, key);
		}
	}

	return csw;
}
