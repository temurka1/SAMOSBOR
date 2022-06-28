#include "pch.h"
#include "Shape.h"

using Shape = SAMOSBOR::core::occ::Shape;

Shape::Shape(TopoDS_Shape& shape): _shape(std::move(shape))
{
}

Shape::~Shape()
{
	if (!_shape.IsNull())
	{
		_shape.Nullify();
	}
}

const TopoDS_Shape& Shape::GetShape() const
{
	return _shape;
}
