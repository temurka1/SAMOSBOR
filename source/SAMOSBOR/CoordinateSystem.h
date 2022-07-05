#pragma once
#include <gp_Vec.hxx>

namespace SAMOSBOR::core::occ
{
	class Transform;

	/// <summary>
	/// Coordinate system in euclidean space
	/// </summary>
	struct CoordinateSystem
	{
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

	/// <summary>
	/// Computes transformation between coordinate systems
	/// </summary>
	Transform GetTransform(const CoordinateSystem& first, const CoordinateSystem& second);
}