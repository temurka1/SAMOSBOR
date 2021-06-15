#include "pch.h"
#include "Step_Exporter.h"
#include "Step_Data.h"

using namespace std;

void Step_Exporter::Export(const Standard_CString& filename, std::vector<Step_Data>& data)
{
	STEPControl_Writer writer;

	if (!Interface_Static::SetIVal("write.step.assembly", 1))
		throw runtime_error("Can't set write.step.assembly");

	for (auto it = data.begin(); it != data.end(); ++it)
	{
		for (auto shapesIt = it->shapes->begin(); shapesIt != it->shapes->end(); ++shapesIt)
		{
			writer.Transfer(*shapesIt, STEPControl_AsIs);
		}
	}

	writer.Write(filename);
}
