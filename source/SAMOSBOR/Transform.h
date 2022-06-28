#pragma once

namespace SAMOSBOR::core::occ
{
	class Transform final
	{
	public:
		[[nodiscard]] const gp_Trsf& GetTransform() const;
		[[nodiscard]] const std::string& GetName() const;

		Transform operator*(const Transform& rhs);
		Transform& operator=(const Transform& rhs);

		Transform(gp_Trsf transform, std::string name = "");
		Transform(const Transform& other);
	private:
		gp_Trsf _transform;
		std::string _name;
	};
}