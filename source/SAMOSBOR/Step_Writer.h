#pragma once

namespace SAMOSBOR::step
{
	class Step_Writer
	{
	public:
		void Write(const std::string& filename, const std::vector<std::shared_ptr<TopoDS_Shape>>& data);
	};
}

