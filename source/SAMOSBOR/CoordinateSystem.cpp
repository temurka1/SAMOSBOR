#include "pch.h"
#include "Transform.h"
#include "CoordinateSystem.h"

using Transform = SAMOSBOR::core::occ::Transform;
using CoordinateSystem = SAMOSBOR::core::occ::CoordinateSystem;

namespace
{
	gp_Ax3 toAx3(const CoordinateSystem& cs)
	{
		return gp_Ax3(gp_Pnt(cs.origin.XYZ()), gp_Dir(cs.uAxis), gp_Dir(cs.vAxis));
	}
}

namespace SAMOSBOR::core::occ
{
	Transform GetTransform(const CoordinateSystem& first, const CoordinateSystem& second)
	{
		gp_Trsf trsf;
		trsf.SetTransformation(toAx3(first), toAx3(second));

		return Transform(trsf);
	}
}
