#include "pch.h"
#include "Transform.h"

using Transform = SAMOSBOR::core::occ::Transform;

Transform::Transform(gp_Trsf transform, std::string name): _transform(std::move(transform)), _name(std::move(name))
{
}

Transform::Transform(const Transform& other): _transform(other.GetTransform()), _name(other.GetName())
{
}

const gp_Trsf& Transform::GetTransform() const
{
	return _transform;
}

const std::string& Transform::GetName() const
{
	return _name;
}

Transform Transform::operator*(const Transform& rhs)
{
	const gp_Trsf transform = GetTransform() * rhs.GetTransform();
	return Transform(transform, "");
}

Transform& Transform::operator=(const Transform& rhs)
{
	if (this == &rhs)
	{
		return *this;
	}

	_transform = rhs.GetTransform();
	_name = rhs.GetName();

	return *this;
}
