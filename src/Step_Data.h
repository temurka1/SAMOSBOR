#pragma once

struct Step_Data
{
	Handle(StepGeom_Axis2Placement3d) mcs;
	Handle(StepGeom_Axis2Placement3d) csw;

	std::shared_ptr<std::vector<TopoDS_Shape>> shapes;

	Step_Data();
	virtual ~Step_Data();
};

