#pragma once

#include "Step_Reader.h"

namespace pooch::assembly
{
	struct Assembly_ParsedItem;

	struct Assembly_Node
	{
		boost::uuids::uuid id;
		std::shared_ptr<TopoDS_Shape> shape;

		Assembly_Node();
		~Assembly_Node();
	};

	struct Assembly_Edge
	{
		std::shared_ptr<gp_Trsf> transform;

		Assembly_Edge();
		~Assembly_Edge();
	};

	typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, Assembly_Node, Assembly_Edge> AssemblyGraph;
	typedef boost::graph_traits<AssemblyGraph>::vertex_descriptor vertex_t;
	typedef boost::graph_traits<AssemblyGraph>::edge_descriptor edge_t;
	typedef boost::graph_traits<AssemblyGraph>::in_edge_iterator in_edge_iterator_t;

	class Assembly_GraphBuilder
	{
	public:
		AssemblyGraph Build(const std::vector<std::shared_ptr<Assembly_ParsedItem>>& parsedStructure, const float extensionLength);

		Assembly_GraphBuilder(const pooch::step::Step_Reader& stepReader);
		virtual ~Assembly_GraphBuilder();
	private:
		void BuildInternal(
			AssemblyGraph& graph, 
			const vertex_t& rootNode, 
			const pooch::step::Step_Data& rootStepData, 
			const std::shared_ptr<gp_Trsf>& rootTransform,
			const std::shared_ptr<Assembly_ParsedItem>& root,
			const std::vector<std::shared_ptr<Assembly_ParsedItem>>& parsed,
			const float extensionLength);

		std::shared_ptr<Assembly_ParsedItem> GetRoot(const std::vector<std::shared_ptr<Assembly_ParsedItem>>& parsed) const;

		std::vector<std::shared_ptr<Assembly_ParsedItem>> GetChildren(const std::shared_ptr<Assembly_ParsedItem>& parent, const std::vector<std::shared_ptr<Assembly_ParsedItem>>& parsed) const;

		std::shared_ptr<std::map<std::string, Handle(Geom_Axis2Placement)>> GetCsws(const pooch::step::Step_Data& stepData) const;
	private:
		pooch::step::Step_Reader _stepReader;
	};
}

