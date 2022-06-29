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
		Transform TransformTo(const CoordinateSystem& cs) const;

		/// <summary>
		/// Origin
		/// </summary>
		gp_Vec origin;

		/// <summary>
		/// Z axis
		/// </summary>
		gp_Vec uAxis;

		/// <summary>
		/// X axis
		/// </summary>
		gp_Vec vAxis;
	};
}