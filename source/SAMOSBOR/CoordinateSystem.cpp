#include "pch.h"
#include "Transform.h"
#include "CoordinateSystem.h"

using Transform = SAMOSBOR::core::occ::Transform;
using CoordinateSystem = SAMOSBOR::core::occ::CoordinateSystem;

namespace
{
	void set(gp_Vec& dest, const gp_XYZ& value)
	{
		dest.SetCoord(value.X(), value.Y(), value.Z());
	}

	gp_Ax3 toAx3(const CoordinateSystem& cs)
	{
		return gp_Ax3(gp_Pnt(cs.origin.XYZ()), gp_Dir(cs.uAxis), gp_Dir(cs.vAxis));
	}
}

Transform CoordinateSystem::TransformTo(const CoordinateSystem& cs) const
{
	gp_Trsf trsf;
	trsf.SetTransformation(toAx3(*this), toAx3(cs));

	return Transform(trsf);
}
