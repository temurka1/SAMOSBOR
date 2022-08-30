#pragma once

#include "Shape.hpp"
#include "CuttingEdge.hpp"
#include "CoordinateSystem.h"

namespace SAMOSBOR::core::occ
{
	struct Csw;
}

class StepData_StepModel;

namespace SAMOSBOR::step::ref
{
	namespace core = SAMOSBOR::core;

	/// <summary>
	/// Data contained in STEP file
	/// </summary>
	class StepData
	{
	public:
		StepData() = default;
		StepData(const Handle(StepData_StepModel)& model, const TopoDS_Shape& shape);
		StepData(const StepData& other);

		core::occ::Shape Shape() const;
		core::occ::CuttingEdge CuttingEdge() const;

		core::occ::CoordinateSystem Pcs() const;
		core::occ::CoordinateSystem Mcs() const;

		core::occ::Csw Csw() const;
	private:
		std::unordered_map<std::string, core::occ::CoordinateSystem> _csmap;

		core::occ::Shape _shape;
		core::occ::CuttingEdge _cuttingEdge;
	};
}