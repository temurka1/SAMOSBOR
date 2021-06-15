#pragma once

struct Step_Data;

class Step_Parser
{
public:
	std::vector<Step_Data> Parse(const std::vector<Standard_CString>& filenames);
private:
	IFSelect_ReturnStatus ReadStep(const Standard_CString& fileName, std::vector<Step_Data>& output);
};

