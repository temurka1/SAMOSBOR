#pragma once

struct Step_Data;

class Step_Exporter
{
public:
	void Export(const Standard_CString& filename, std::vector<Step_Data>& data);
};