#include "pch.h"
#include "StepData.h"
#include "Csw.hpp"
#include "StringUtils.h"

namespace utils = SAMOSBOR::core::utils;

using StepData = SAMOSBOR::step::ref::StepData;
using Shape = SAMOSBOR::core::occ::Shape;
using Csw = SAMOSBOR::core::occ::Csw;
using CuttingEdge = SAMOSBOR::core::occ::CuttingEdge;
using CoordinateSystem = SAMOSBOR::core::occ::CoordinateSystem;

using EdgeT = std::pair<TopoDS_Edge, Standard_Integer>;
using WireOrEdges = std::variant<std::vector<EdgeT>, TopoDS_Wire>;

namespace
{
	/// <summary>
	/// Parses STEP model for CUTTING LINE PART/CUTTING EDGE PART entities.
	/// Could return either wire (when composite curves were used) or list of wire's edges
	/// </summary>
	WireOrEdges parse_cutting_lines(const Handle(StepData_StepModel)& model)
	{
		std::vector<EdgeT> edges;
		std::string searchStr("CUTTING");

		Handle(Transfer_TransientProcess) tp = new Transfer_TransientProcess();

		Interface_EntityIterator shapeIterator(model->Entities());
		shapeIterator.SelectType(STANDARD_TYPE(StepRepr_ShapeRepresentationRelationship), Standard_True);

		for (; shapeIterator.More(); shapeIterator.Next())
		{
			auto shapeRelRepr = Handle(StepRepr_ShapeRepresentationRelationship)::DownCast(shapeIterator.Value());
			auto shapeRepr = Handle(StepShape_ShapeRepresentation)::DownCast(shapeRelRepr->Rep2());

			for (Standard_Integer i = 1; i <= shapeRepr->NbItems(); ++i)
			{
				auto subShapeItem = shapeRepr->ItemsValue(i);

				if (subShapeItem.IsNull() || !subShapeItem->IsKind(STANDARD_TYPE(StepShape_GeometricSet))) 
				{
					continue;
				}

				auto curveSet = Handle(StepShape_GeometricSet)::DownCast(subShapeItem);

				for (Standard_Integer j = 1; j <= curveSet->NbElements(); ++j)
				{
					auto setSelect = curveSet->ElementsValue(j);
					if (setSelect.IsNull())
					{
						continue;
					}

					auto curve = setSelect.Curve();
					if (curve.IsNull())
					{
						continue;
					}

					bool isSupportedCurve = curve->IsKind(STANDARD_TYPE(StepGeom_TrimmedCurve)) || curve->IsKind(STANDARD_TYPE(StepGeom_Line));
					if (!isSupportedCurve)
					{
						continue;
					}

					std::string name = curve->Name()->ToCString();
					Standard_Integer id = model->IdentLabel(curve);

					auto search = std::search(name.begin(), name.end(), searchStr.begin(), searchStr.end(), [](char ch1, char ch2)
					{
						return std::toupper(ch1) == ch2;
					});

					if (name.end() == search)
					{
						continue;
					}

					if (auto compositeCurve = Handle(StepGeom_CompositeCurve)::DownCast(curve); !compositeCurve.IsNull())
					{
						StepToTopoDS_TranslateCompositeCurve translate(compositeCurve, tp);
						return translate.Value();
					}

					if (auto trimmedCurve = Handle(StepGeom_TrimmedCurve)::DownCast(curve); !trimmedCurve.IsNull())
					{
						auto geomTrimmedCurve = StepToGeom::MakeTrimmedCurve(trimmedCurve);						
						
						BRepBuilderAPI_MakeEdge edgeMaker(geomTrimmedCurve);
						TopoDS_Edge edge = edgeMaker.Edge();

						edges.push_back(EdgeT(edge, id));
					}
				}
			}
		}

		return edges;
	}

	/// <summary>
	/// Builds cutting edge wire
	/// </summary>
	CuttingEdge build_cutting_edge(const WireOrEdges& wireOrEdges)
	{
		if (const TopoDS_Wire* wire = std::get_if<TopoDS_Wire>(&wireOrEdges))
		{
			return *wire;
		}
		
		if (const std::vector<EdgeT>* edges = std::get_if<std::vector<EdgeT>>(&wireOrEdges))
		{
			TopTools_ListOfShape edgesList;
			for (auto it = edges->begin(); it != edges->end(); ++it)
			{
				edgesList.Append(it->first);
			}

			BRepBuilderAPI_MakeWire wireMaker;
			wireMaker.Add(edgesList);
			
			return wireMaker.IsDone() ? wireMaker.Wire() : TopoDS_Wire();
		}
	}
}

StepData::StepData(const Handle(StepData_StepModel)& model, const TopoDS_Shape& shape) : _shape(shape)
{
	// parse coordinate systems from STEP
	//
	Interface_EntityIterator csIterator(model->Entities());
	csIterator.SelectType(STANDARD_TYPE(StepGeom_Axis2Placement3d), Standard_True);

	for (; csIterator.More(); csIterator.Next())
	{
		auto stepAxis = Handle(StepGeom_Axis2Placement3d)::DownCast(csIterator.Value());
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

	// parse CUTTING EDGE LINE/CUTTING EDGE PART from STEP
	//
	_cuttingEdge = build_cutting_edge(parse_cutting_lines(model));
}

StepData::StepData(const StepData& other) : _shape(other._shape), _csmap(other._csmap), _cuttingEdge(other._cuttingEdge)
{
}

Shape StepData::Shape() const
{
	return _shape;
}

CuttingEdge StepData::CuttingEdge() const
{
	return _cuttingEdge;
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

			// CSW index => hex string => decimal number (port)
			// no CSW index => 0 (port)

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
