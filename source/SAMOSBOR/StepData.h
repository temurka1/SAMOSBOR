#pragma once

#include "Shape.hpp"
#include "CoordinateSystem.h"

namespace SAMOSBOR::core::occ
{
	struct Csw;
}

namespace SAMOSBOR::step::ref
{
	namespace core = SAMOSBOR::core;

	/// <summary>
	/// Data contained in STEP file
	/// </summary>
	struct StepData
	{
	public:
		StepData() = default;
		StepData(const Handle(Interface_InterfaceModel)& model, const TopoDS_Shape& shape);
		StepData(const StepData& other);

		core::occ::Shape Shape() const;

		core::occ::CoordinateSystem Pcs() const;
		core::occ::CoordinateSystem Mcs() const;

		core::occ::Csw Csw() const;
	private:
		std::unordered_map<std::string, core::occ::CoordinateSystem> _csmap;
		core::occ::Shape _shape;
	};
}