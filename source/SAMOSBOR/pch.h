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
#include <Interface_Static.hxx>
#include <Interface_EntityIterator.hxx>
#include <TCollection_HAsciiString.hxx>
#include <TColgp_Array1OfDir.hxx>
#include <Standard_Handle.hxx>
#include <Geom_Axis2Placement.hxx>
#include <StepToGeom.hxx>
#include <Message.hxx>
#include <Message_Messenger.hxx>
#include <Message_PrinterOStream.hxx>
#include <BRepBndLib.hxx>
#include <BRep_Tool.hxx>
#include <BRep_Builder.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRepBuilderAPI_Copy.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Compound.hxx>
#include <Poly_Triangulation.hxx>
#include <Bnd_Box.hxx>
#include <Precision.hxx>

#include <Poly_Connect.hxx>
#include <GeomLib.hxx>

#include <gp_Pnt2d.hxx>
#include <gp_Ax3.hxx>

#include <array>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <string_view>
#include <charconv>
#include <filesystem>
#include <algorithm>

#endif //PCH_H
