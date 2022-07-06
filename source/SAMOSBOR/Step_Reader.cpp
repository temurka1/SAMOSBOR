#include "pch.h"
#include "Step_Reader.h"

using namespace std;
using namespace SAMOSBOR::step;

Step_Data::Step_Data() : mcs(nullptr)
{
	this->shape = make_shared<TopoDS_Shape>();
	this->csws = make_shared<map<string, Handle(StepGeom_Axis2Placement3d)>>();
}

Step_Data::~Step_Data()
{
	this->shape = nullptr;
	this->csws = nullptr;
}

Step_Data Step_Reader::Read(const std::string& filename)
{
	auto [result, status] = ReadStep(filename);

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

	return result;
}

tuple<Step_Data, IFSelect_ReturnStatus> Step_Reader::ReadStep(const string& fileName)
{
	Step_Data result;

	STEPControl_Reader reader;
	IFSelect_ReturnStatus status = reader.ReadFile(fileName.c_str());

	if (status != IFSelect_RetDone)
		return make_tuple(result, status);

	auto numberOfRoots = reader.NbRootsForTransfer();

	for (auto n = 1; n <= numberOfRoots; n++)
	{
		reader.TransferRoot(n);
	}

	auto numberOfShapes = reader.NbShapes();
	if (numberOfShapes == 0)
	{
		return make_tuple(result, IFSelect_RetVoid);
	}

	result.shape = make_shared<TopoDS_Shape>(reader.Shape());

	auto model = Handle(StepData_StepModel)::DownCast(reader.Model());
	auto numberOfEntities = model->NbEntities();

	for (auto i = 1; i <= numberOfEntities; i++)
	{
		auto entity = model->Entity(i);

		if (entity.IsNull() || !entity->IsKind(STANDARD_TYPE(StepGeom_Axis2Placement3d)))
			continue;

		auto axis = Handle(StepGeom_Axis2Placement3d)::DownCast(entity);
		auto axisName = axis->Name()->ToCString();

		if (strcmp(axisName, "MCS") == 0)
		{
			result.mcs = axis;
			continue;
		}

		if (strncmp(axisName, "CSW", 3) == 0)
		{
			// CSW, CSW1, CSW1_2, CSW_1_1, etc
			//
			result.csws->insert(make_pair(string(axisName), axis));
			continue;
		}
	}

	if (result.mcs.IsNull())
		throw runtime_error("Mandatory CSW or MCS coordinate systems not found");

	return make_tuple(result, status);
}