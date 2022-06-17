#include "pch.h"
#include "Assembly_TransformationVisitor.h"

using namespace std;
using namespace SAMOSBOR::assembly;

Assembly_TransformationVisitor::Assembly_TransformationVisitor(vector<shared_ptr<TopoDS_Shape>>* outShapes) : _outShapes(outShapes)
{
}

void Assembly_TransformationVisitor::finish_vertex(vertex_t v, const AssemblyGraph& graph) const
{
    auto shape = graph[v].shape;

#ifdef _DEBUG
    cout << "vertex = " << v << " " << graph[v].id << endl;
#endif

    // root node
    if (v == boost::vertex(0, graph))
    {
        _outShapes->push_back(shape);
        return;
    }

    auto it = boost::in_edges(v, graph);
    for (in_edge_iterator_t ei = it.first, ei_end = it.second; ei != ei_end; ++ei)
    {
        auto target = boost::target(*ei, graph);
        auto source = boost::source(*ei, graph);

        if (source < v)
        {
#ifdef _DEBUG
            cout << "edge = source: " << source << " target: " << target << endl;
#endif

            auto transform = graph[*ei].transform;

            TopLoc_Location loc(*transform);
            shape->Move(loc);

            _outShapes->push_back(shape);
        }
    }
}