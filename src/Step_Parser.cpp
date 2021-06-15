#include "pch.h"
#include "Step_Data.h"
#include "Step_Parser.h"

using namespace std;

vector<Step_Data> Step_Parser::Parse(const vector<Standard_CString>& filenames)
{
	vector<Step_Data> result;

	for (auto it = filenames.begin(); it != filenames.end(); it++)
	{
		auto status = ReadStep(*it, result);

		switch (status)
		{
		case IFSelect_RetError:
			throw runtime_error("Not a valid Step file");
			break;

		case IFSelect_RetFail:
			throw runtime_error("Reading has failed");
			break;

		case IFSelect_RetVoid:
			throw runtime_error("Nothing to transfer");
			break;
		}
	}

	return result;
}

IFSelect_ReturnStatus Step_Parser::ReadStep(const Standard_CString& fileName, vector<Step_Data>& output)
{
	Step_Data result;

	STEPControl_Reader reader;
	IFSelect_ReturnStatus status = reader.ReadFile(fileName);

	if (status != IFSelect_RetDone)
		return status;

	reader.PrintCheckLoad(false, IFSelect_ItemsByEntity);

	auto numberOfRoots = reader.NbRootsForTransfer();
	reader.PrintCheckTransfer(false, IFSelect_ItemsByEntity);

	for (auto n = 1; n <= numberOfRoots; n++)
	{
		reader.TransferRoot(n);
	}

	auto numberOfShapes = reader.NbShapes();
	if (numberOfShapes == 0)
	{
		return IFSelect_RetVoid;
	}

	for (auto i = 1; i <= numberOfShapes; i++)
	{
		result.shapes->push_back(reader.Shape(i));
	}

	auto model = Handle(StepData_StepModel)::DownCast(reader.Model());
	auto numberOfEntities = model->NbEntities();

	for (auto i = 1; i <= numberOfEntities; i++)
	{
		auto entity = model->Entity(i);

		if (entity.IsNull() || !entity->IsKind(STANDARD_TYPE(StepGeom_Axis2Placement3d)))
			continue;

		auto axis = Handle(StepGeom_Axis2Placement3d)::DownCast(entity);

		if (strcmp(axis->Name()->ToCString(), "MCS") == 0)
		{
			result.mcs = axis;
			continue;
		}

		if (strcmp(axis->Name()->ToCString(), "CSW") == 0)
		{
			result.csw = axis;
			continue;
		}
	}

	if (result.csw.IsNull() && result.mcs.IsNull())
		throw runtime_error("Mandatory CSW or MCS coordinate systems not found");

	output.push_back(result);

	return status;
}