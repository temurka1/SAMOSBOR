#include "pch.h"
#include "Step_Data.h"

using namespace std;

Step_Data::Step_Data() : mcs(nullptr)
{
	this->shapes = make_shared<vector<TopoDS_Shape>>();
	this->csws = make_shared<vector<Handle(StepGeom_Axis2Placement3d)>>();
}

Step_Data::~Step_Data()
{
	this->shapes = nullptr;
	this->csws = nullptr;
}
