#include "pch.h"
#include "Assembly_Parser.h"
#include "Assembly_GraphBuilder.h"

#include "Step_Reader.h"
#include <filesystem>

using namespace std;
using namespace SAMOSBOR::step;
using namespace SAMOSBOR::assembly;

namespace fs = std::filesystem;

Assembly_Node::Assembly_Node()
{
	this->shape = make_shared<TopoDS_Shape>();
}

Assembly_Node::~Assembly_Node()
{
	this->shape = nullptr;
}

Assembly_Edge::Assembly_Edge()
{
	this->transform = make_shared<gp_Trsf>();
}

Assembly_Edge::~Assembly_Edge()
{
	this->transform = nullptr;
}

Assembly_GraphBuilder::Assembly_GraphBuilder(Step_Reader* stepReader) : _stepReader(stepReader)
{
}

Assembly_GraphBuilder::~Assembly_GraphBuilder()
{
}

AssemblyGraph Assembly_GraphBuilder::Build(const vector<shared_ptr<Assembly_ParsedItem>>& parsed, const float extensionLength)
{
	AssemblyGraph graph;

	auto root = this->GetRoot(parsed);
	auto children = this->GetChildren(root, parsed);

	vertex_t rootNode = boost::add_vertex(graph);

	fs::path path("data/tool_5");
	path /= root->filename;

	auto rootStepData = _stepReader->Read(path.string().c_str());

	graph[rootNode].shape = rootStepData.shape;
	graph[rootNode].id = root->id;

	BuildInternal(graph, rootNode, rootStepData, nullptr, root, parsed, extensionLength);

	cout << flush;

	return graph;
}

void Assembly_GraphBuilder::BuildInternal(
	AssemblyGraph& graph,
	const vertex_t& rootNode,
	const Step_Data& rootStepData,
	const shared_ptr<gp_Trsf>& rootTransform,
	const shared_ptr<Assembly_ParsedItem>& root,
	const vector<shared_ptr<Assembly_ParsedItem>>& parsed,
	const float extensionLength)
{
	fs::path path("data/tool_5");

	auto rootCsws = this->GetCsws(rootStepData);
	auto children = this->GetChildren(root, parsed);

	if (children.empty())
		return;

	for (auto child : children)
	{
		fs::path filePath = path / child->filename;

		auto childStepData = _stepReader->Read(filePath.string().c_str());
		auto childMcs = StepToGeom::MakeAxis2Placement(childStepData.mcs);

		// mcs
		gp_Ax3 ax3Mcs(childMcs->Ax2());

		// csw's
		//
		for (auto cswSlot : child->cswSlots)
		{
			// calculate csw -> mcs transformation matrix
			//
			gp_Ax3 ax3Csw(rootCsws->find(cswSlot)->second->Ax2());

			auto displacement = make_shared<gp_Trsf>();
			displacement->SetDisplacement(ax3Mcs, ax3Csw);

			gp_Trsf translation;
			translation.SetTranslation(gp_Vec(0.0, 0.0, extensionLength));

			if (rootTransform != nullptr)
			{
				displacement->PreMultiply(*rootTransform);
			}

			displacement->Multiply(translation);

			// fill graph nodes/edges
			//
			vertex_t childNode = boost::add_vertex(graph);

			bool b;
			edge_t edge;
			boost::tie(edge, b) = boost::add_edge(rootNode, childNode, graph);

			BRepBuilderAPI_Copy copyApi(*childStepData.shape);

			graph[childNode].shape = make_shared<TopoDS_Shape>(copyApi.Shape());
			graph[childNode].id = child->id;
			graph[edge].transform = displacement;

			BuildInternal(graph, childNode, childStepData, displacement, child, parsed, extensionLength);
		}
	}
}

shared_ptr<Assembly_ParsedItem> Assembly_GraphBuilder::GetRoot(const vector<shared_ptr<Assembly_ParsedItem>>& parsedStructure) const
{
	auto it = std::find_if(parsedStructure.begin(), parsedStructure.end(), [&](const shared_ptr<Assembly_ParsedItem> item)
		{
			bool isnull = item->parentId.is_nil();
			return isnull;
		});

	return (*it);
}

vector<shared_ptr<Assembly_ParsedItem>> Assembly_GraphBuilder::GetChildren(const shared_ptr<Assembly_ParsedItem>& parent, const vector<shared_ptr<Assembly_ParsedItem>>& parsedStructure) const
{
	std::vector<shared_ptr<Assembly_ParsedItem>> children;

	std::copy_if(parsedStructure.begin(), parsedStructure.end(), std::back_inserter(children), [&](const shared_ptr<Assembly_ParsedItem> item)
		{
			return item->parentId == parent->id;
		});

	return children;
}

shared_ptr<map<string, Handle(Geom_Axis2Placement)>> Assembly_GraphBuilder::GetCsws(const Step_Data& stepData) const
{
	auto result = make_shared<map<string, Handle(Geom_Axis2Placement)>>();

	for (auto csw : *(stepData.csws))
	{
		result->insert(make_pair(csw.first, StepToGeom::MakeAxis2Placement(csw.second)));
	}

	return result;
}