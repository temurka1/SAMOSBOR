// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here

#include <STEPControl_Reader.hxx>
#include <STEPControl_Writer.hxx>
#include <StepData_StepModel.hxx>
#include <StepGeom_Axis2Placement3d.hxx>
#include <StepGeom_CartesianPoint.hxx>
#include <StepRepr_Representation.hxx>
#include <Interface_InterfaceModel.hxx>
#include <TCollection_HAsciiString.hxx>
#include <Standard_Handle.hxx>
#include <Interface_Static.hxx>
#include <Geom_Axis2Placement.hxx>
#include <StepToGeom.hxx>
#include <BRepBuilderAPI_Copy.hxx>

#include <gp_Pnt2d.hxx>
#include <gp_Ax3.hxx>

#include <vector>
#include <map>
#include <string>
#include <string_view>
#include <charconv>

#include <boost/foreach.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#endif //PCH_H
