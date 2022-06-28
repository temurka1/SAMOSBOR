#pragma once

namespace SAMOSBOR::core::occ
{
	/// <summary>
	/// Wrapper over TopoDS_Shape.
	/// Owns corresponding TopoDS_Shape
	/// </summary>
	class Shape final
	{
	public:
		[[nodiscard]] const TopoDS_Shape& GetShape() const;

		explicit Shape(TopoDS_Shape& shape);
		~Shape();
	private:
		TopoDS_Shape _shape;
	};
}