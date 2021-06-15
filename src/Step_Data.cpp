#include "pch.h"
#include "Step_Data.h"

using namespace std;

Step_Data::Step_Data() : mcs(nullptr), csw(nullptr)
{
	this->shapes = make_shared<vector<TopoDS_Shape>>();
}

Step_Data::~Step_Data()
{
	this->shapes = nullptr;
}
