#pragma once

namespace SAMOSBOR::core::occ
{
	class Transform;

	/// <summary>
	/// Coordinate system in euclidean space
	/// </summary>
	struct CoordinateSystem
	{
		/// <summary>
		/// Computes transformation between this CS and another CS
		/// </summary>
		Transform GetTransformTo(const CoordinateSystem& cs) const;

		CoordinateSystem() = default;
		CoordinateSystem(const Transform& transform);

		gp_Vec origin;
		gp_Vec uAxis;
		gp_Vec vAxis;
	};
}