#include "pch.h"
#include "Step_Writer.h"

using namespace std;

namespace pooch::step
{
	void Step_Writer::Export(const std::string& filename, std::vector<std::shared_ptr<TopoDS_Shape>>& data)
	{
		STEPControl_Writer writer;

		if (!Interface_Static::SetIVal("write.step.assembly", 1))
			throw runtime_error("Can't set write.step.assembly");

		for (auto &shape : data)
		{
			writer.Transfer(*shape, STEPControl_AsIs);
		}

		writer.Write(filename.c_str());
	}
}