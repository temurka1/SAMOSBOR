#include "pch.h"
#include "Step_Transformer.h"
#include "Step_Data.h"

using namespace std;

void Step_Transformer::Transform(vector<Step_Data>& data, float extensionLength)
{
	if (data.size() < 1)
		throw runtime_error("No files to export");

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
