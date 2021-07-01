#include "pch.h"
#include "Step_Reader.h"

using namespace std;

namespace pooch::step
{
	Step_Data::Step_Data() : mcs(nullptr)
	{
		this->shape = make_shared<TopoDS_Shape>();
		this->csws = make_shared<vector<Handle(StepGeom_Axis2Placement3d)>>();
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

			if (strcmp(axis->Name()->ToCString(), "MCS") == 0)
			{
				result.mcs = axis;
				continue;
			}

			if (strcmp(axis->Name()->ToCString(), "CSW") == 0)
			{
				result.csws->push_back(axis);
				continue;
			}

			if (strncmp(axis->Name()->ToCString(), "CSW_", 4) == 0)
			{
				result.csws->push_back(axis);
				continue;
			}
		}

		if (result.mcs.IsNull())
			throw runtime_error("Mandatory CSW or MCS coordinate systems not found");

		return make_tuple(result, status);
	}
}
