#include "pch.h"
#include "Transform.h"

using Transform = SAMOSBOR::core::occ::Transform;

Transform::Transform() : _transform(gp_Trsf()), _name("identity")
{

}

Transform::Transform(gp_Trsf transform, std::string name): _transform(std::move(transform)), _name(std::move(name))
{
}

Transform::Transform(const Transform& other): _transform(other.Get()), _name(other.GetName())
{
}

const gp_Trsf& Transform::Get() const
{
	return _transform;
}

const std::string& Transform::GetName() const
{
	return _name;
}

Transform& Transform::PreMultiply(const Transform& rhs)
{
	_transform.PreMultiply(rhs.Get());
	return *this;
}

Transform& Transform::Multiply(const Transform& rhs)
{
	_transform.Multiply(rhs.Get());
	return *this;
}

Transform& Transform::operator=(const Transform& rhs)
{
	if (this == &rhs)
	{
		return *this;
	}

	_transform = rhs.Get();
	_name = rhs.GetName();

	return *this;
}
