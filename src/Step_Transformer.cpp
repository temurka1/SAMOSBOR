#include "pch.h"
#include "Step_Transformer.h"
#include "Step_Data.h"
#include <Geom_Axis2Placement.hxx>
#include <StepToGeom.hxx>

using namespace std;

void Step_Transformer::Transform(vector<Step_Data>& data, float extensionLength)
{
	if (data.size() < 1)
		throw runtime_error("No files to export");

	for (auto it = data.begin(); it != data.end(); ++it)
	{
		if (!it->csw.IsNull())
		{
			auto axis = StepToGeom::MakeAxis2Placement(it->csw);

			std::cout << "CSW" << std::endl;

			std::cout << axis->XDirection().X() << axis->XDirection().Y() << axis->XDirection().Z() << std::endl;
			std::cout << axis->YDirection().X() << axis->YDirection().Y() << axis->YDirection().Z() << std::endl;

			std::cout << axis->Location().X() << axis->Location().Y() << axis->Location().Z() << std::endl;
			std::cout << "" << std::endl;
		}
		
		if (!it->mcs.IsNull())
		{
			auto axis = StepToGeom::MakeAxis2Placement(it->mcs);

			std::cout << "MCS" << std::endl;			
			
			std::cout << axis->XDirection().X() << axis->XDirection().Y() << axis->XDirection().Z() << std::endl;
			std::cout << axis->YDirection().X() << axis->YDirection().Y() << axis->YDirection().Z() << std::endl;

			std::cout << axis->Location().X() << axis->Location().Y() << axis->Location().Z() << std::endl;
			std::cout << "" << std::endl;
		}
	}

	auto csw = StepToGeom::MakeAxis2Placement(data[0].csw);
	if (csw.IsNull())
		throw runtime_error("CSW not found");

	for (int i = 1; i < data.size(); i++)
	{
		auto mcs = StepToGeom::MakeAxis2Placement(data[i].mcs);

		if (mcs.IsNull())
			break; 
		
		gp_Ax3 ax3Mcs(mcs->Ax2());
		gp_Ax3 ax3Csw(csw->Ax2());

		gp_Trsf displacement;
		displacement.SetDisplacement(ax3Csw, ax3Mcs);

		gp_Trsf translation;
		translation.SetTranslation(gp_Vec(0.0, 0.0, extensionLength));

		displacement.Multiply(translation);

		for (auto it = data[0].shapes->begin(); it != data[0].shapes->end(); ++it)
		{
			TopLoc_Location loc(displacement);
			it->Move(loc);
		}
	}
}
