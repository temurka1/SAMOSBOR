#pragma once

namespace pooch::step
{
	class Step_Writer
	{
		void Export(const std::string& filename, std::vector<std::shared_ptr<TopoDS_Shape>>& data);
	};
}

