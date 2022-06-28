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

CoordinateSystem::CoordinateSystem(const Transform& transform)
{
	const gp_Trsf trsf = transform.GetTransform();

	set(origin, trsf.TranslationPart());
	set(uAxis, gp_XYZ(0.0, 0.0, 1.0) * trsf.HVectorialPart());
	set(vAxis, gp_XYZ(1.0, 0.0, 0.0) * trsf.HVectorialPart());
}

Transform CoordinateSystem::GetTransformTo(const CoordinateSystem& cs) const
{
	gp_Trsf trsf;
	trsf.SetTransformation(toAx3(*this), toAx3(cs));

	return Transform(trsf);
}
