#pragma once
#include "Shape.hpp"
#include "CoordinateSystem.h"

class STEPControl_Reader;

namespace std::filesystem
{
	class path;
}

namespace SAMOSBOR::core
{
	template<class T> class ResultOr;
}

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
		StepData(const STEPControl_Reader& reader);
		StepData(const StepData& other);

		core::occ::Shape Shape() const;

		core::occ::CoordinateSystem Pcs() const;
		core::occ::CoordinateSystem Mcs() const;

		core::occ::Csw Csw() const;
	private:
		std::unordered_map<std::string, core::occ::CoordinateSystem> _csmap;
		core::occ::Shape _shape;
	};

	/// <summary>
	/// Reads STEP file
	/// </summary>
	class StepReader
	{
	public:
		core::ResultOr<StepData> Read(const std::filesystem::path& filepath, bool printCheckload = false);
	};
}
