#pragma once

namespace pooch::step
{
	struct Step_Data
	{
		Handle(StepGeom_Axis2Placement3d) mcs;
		std::shared_ptr<std::vector<Handle(StepGeom_Axis2Placement3d)>> csws;

		std::shared_ptr<TopoDS_Shape> shape;

		Step_Data();
		virtual ~Step_Data();
	};

	class Step_Reader
	{
	public:
		Step_Data Read(const std::string& filename);
	private:
		std::tuple<Step_Data, IFSelect_ReturnStatus> ReadStep(const std::string& fileName);
	};
}

