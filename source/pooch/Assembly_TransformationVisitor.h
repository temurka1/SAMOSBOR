#pragma once

#include "Assembly_GraphBuilder.h"

namespace pooch::assembly
{
	class Assembly_TransformationVisitor : public boost::default_dfs_visitor
	{
	public:
		Assembly_TransformationVisitor(std::vector<std::shared_ptr<TopoDS_Shape>>* outShapes);

		void finish_vertex(vertex_t v, const AssemblyGraph& graph) const;
	private:
		std::vector<std::shared_ptr<TopoDS_Shape>>* _outShapes;
	};
}
