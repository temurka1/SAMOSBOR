#include "pch.h"
#include "StepData.h"
#include "Csw.hpp"
#include "StringUtils.h"

namespace utils = SAMOSBOR::core::utils;

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
			Csw::Port port = 0;
			Csw::Key key = csw.cs.emplace(cs.second);

			// convert CSW number string (could be "1", "1_1", "2_1", etc.) into hexademical number string
			// and then convert to decimal - this will be our port identifier futher on

			const std::string portStr = utils::string_to_hex(cs.first.substr(3, cs.first.length() - 3));
			if (!portStr.empty())
			{
				std::from_chars(portStr.data(), portStr.data() + portStr.size(), port, 16);
			}

			key.set_tag(port);
			csw.ports.emplace(port, key);
		}
	}

	return csw;
}
