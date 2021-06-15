#pragma once

struct Step_Data
{
	Handle(StepGeom_Axis2Placement3d) mcs;
	std::shared_ptr<std::vector<Handle(StepGeom_Axis2Placement3d)>> csws;

	std::shared_ptr<std::vector<TopoDS_Shape>> shapes;

	Step_Data();
	virtual ~Step_Data();
};

