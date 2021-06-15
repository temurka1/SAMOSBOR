#include "pch.h"
#include "Step_Transformer.h"
#include "Step_Data.h"

using namespace std;

void Step_Transformer::Transform(vector<Step_Data>& data, float extensionLength)
{
	if (data.size() < 1)
		throw runtime_error("No files to export");

	auto csws = data[0].csws;

	for (int i = 1; i < data.size(); i++)
	{
		auto mcs = StepToGeom::MakeAxis2Placement(data[i].mcs);
		if (mcs.IsNull())
			break;

		for (int j = 0; j < csws->size(); j++)
		{
			auto csw = StepToGeom::MakeAxis2Placement(csws->at(j));
			if (csw.IsNull())
				break;

			gp_Ax3 ax3Csw(csw->Ax2());
			gp_Ax3 ax3Mcs(mcs->Ax2());

			gp_Trsf displacement;
			displacement.SetDisplacement(ax3Mcs, ax3Csw);

			gp_Trsf translation;
			translation.SetTranslation(gp_Vec(0.0, 0.0, extensionLength));

			displacement.Multiply(translation);

			TopLoc_Location loc(displacement);
			data[i].shapes->at(j).Move(loc);
		}

		csws = data[i].csws;
	}
}
