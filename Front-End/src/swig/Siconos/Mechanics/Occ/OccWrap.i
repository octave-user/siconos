// -*- c++ -*-
// SWIG interface for Siconos Mechanics/Occ
%module(directors="1", allprotected="1") OccWrap

%include start.i

#undef WITH_IO
#undef WITH_SERIALIZATION

%include sharedPointers.i

%{
#include <MechanicsFwd.hpp>
%}
%include <MechanicsFwd.hpp>

#ifdef WITH_IO
%{
#include <SiconosFull.hpp>
%}
#endif

%include picklable.i

%include path.i

%{
#include <SiconosKernel.hpp>
%}

%include handleException.i
%include KernelTypes.i

%import Kernel/Kernel.i

%{
#include <ExternalBody.hpp>
%}
%import ../ContactDetection/Base.i

%include pyRegister.i

// do not wrap visitor visit : this leads to a huge amount of wrapper
// code generation and this fails at compile time on shared_ptr freearg
%ignore visit;

%import <Standard_Macro.hxx>
%import <Standard_Real.hxx>
#define DEFINE_STANDARD_ALLOC

%{
#include <TopoDS_Shape.hxx>
%}
%typecheck(SWIG_TYPECHECK_INTEGER) (TopoDS_Shape & shape) ()
%{
  int res;
  res = SWIG_ConvertPtr($input, 0, SWIGTYPE_p_std11__shared_ptrT_TopoDS_Shape_t, 0);
  _v = SWIG_CheckState(res);
  if(!_v)
  {
    res = SWIG_ConvertPtr($input, 0, SWIGTYPE_p_TopoDS_Shape, 0);
  }
  _v = SWIG_CheckState(res);
%}

%typemap(in) (TopoDS_Shape & shape) (void * argp1, int res)
%{
  {
    int newmem = 0;
    res = SWIG_ConvertPtrAndOwn(obj0, &argp1, SWIGTYPE_p_TopoDS_Shape,  0 , &newmem);
    if (!SWIG_IsOK(res)) {
      SWIG_exception_fail(SWIG_ArgError(res), "in method '" "new_OccContactShape" "', argument " "1"" of type '" "TopoDS_Shape const &""'"); 
    }
  }
%}

%shared_ptr(TopoDS_Shape)
%include <TopoDS_Shape.hxx>
TYPEDEF_SPTR(TopoDS_Shape)



%extend TopoDS_Shape {
%pythoncode {
	def __getstate__(self):
		from OCC.BRepTools import BRepTools_ShapeSet
		ss = BRepTools_ShapeSet()
		ss.Add(self)
		str_shape = ss.WriteToString()
		indx = ss.Locations().Index(self.Location())
		return str_shape, indx
	def __setstate__(self, state):
		from OCC.BRepTools import BRepTools_ShapeSet
		topods_str, indx = state
		ss = BRepTools_ShapeSet()
		ss.ReadFromString(topods_str)
		the_shape = ss.Shape(ss.NbShapes())
		location = ss.Locations().Location(indx)
		the_shape.Location(location)
		self.this = the_shape.this
	}
};

%{
#include <TopoDS_Face.hxx>
%}
%shared_ptr(TopoDS_Face)
%include <TopoDS_Face.hxx>

%{
#include <TopoDS_Edge.hxx>
%}
%shared_ptr(TopoDS_Edge)
%include <TopoDS_Edge.hxx>


// force the definition of SWIGTYPE_p_Interaction...
typedef Interaction Interaction;

// due to undefined private copy constructors
%feature("notabstract") OccTimeStepping;

%typecheck(SWIG_TYPECHECK_INTEGER) (const OccContactFace & sh2) ()
%{
  // director mess
  int res;
  res = SWIG_ConvertPtr(argv[1], 0, SWIGTYPE_p_std11__shared_ptrT_OccContactFace_t, 0);
  _v = SWIG_CheckState(res);
  if(!_v)
  {
    res = SWIG_ConvertPtr(argv[1], 0, SWIGTYPE_p_OccContactFace, 0);
  }
%}

%typecheck(SWIG_TYPECHECK_INTEGER) (const OccContactShape & sh2) ()
%{
  // director mess
  int res;
  res = SWIG_ConvertPtr(argv[1], 0, SWIGTYPE_p_std11__shared_ptrT_OccContactShape_t, 0);
  _v = SWIG_CheckState(res);
  if(!_v)
  {
    res = SWIG_ConvertPtr(argv[1], 0, SWIGTYPE_p_OccContactShape, 0);
  }
%}

%typecheck(SWIG_TYPECHECK_INTEGER) (SPC::OccContactShape psh2) ()
%{
  // director mess
  int res;
  res = SWIG_ConvertPtr(argv[1], 0, SWIGTYPE_p_std11__shared_ptrT_OccContactShape_const_t, 0);
  _v = SWIG_CheckState(res);
  if(!_v)
  {
    res = SWIG_ConvertPtr(argv[1], 0, SWIGTYPE_p_OccContactShape, 0);
  }
%}

%typecheck(SWIG_TYPECHECK_INTEGER) (SPC::OccContactFace psh2) ()
%{
  // director mess
  int res;
  res = SWIG_ConvertPtr(argv[1], 0, SWIGTYPE_p_std11__shared_ptrT_OccContactFace_const_t, 0);
  _v = SWIG_CheckState(res);
  if(!_v)
  {
    res = SWIG_ConvertPtr(argv[1], 0, SWIGTYPE_p_OccContactFace, 0);
  }
%}

%feature("nodirector") ContactShapeDistance;
PY_FULL_REGISTER(ContactShapeDistance);

%feature("nodirector") OccContactShape;
PY_FULL_REGISTER(OccContactShape);

%feature("nodirector") OccContactFace;
PY_FULL_REGISTER(OccContactFace);

%feature("nodirector") OccContactEdge;
PY_FULL_REGISTER(OccContactEdge);

%feature("nodirector") Geometer::visit;
PY_FULL_REGISTER(Geometer);

%feature("nodirector") ContactPoint;
PY_FULL_REGISTER(ContactPoint);

%feature("nodirector") OccBody;
PY_FULL_REGISTER(OccBody);

%feature("nodirector") OccR;
PY_FULL_REGISTER(OccR);

%feature("nodirector") OccTimeStepping;
PY_FULL_REGISTER(OccTimeStepping);

%feature("nodirector") OccSpaceFilter;
PY_FULL_REGISTER(OccSpaceFilter);

%{
#include <cadmbtb.hpp>
%}

%include <cadmbtb.hpp>

%inline
%{
  #include <BRepTools.hxx>
  #include <BRep_Builder.hxx>
  #include <BRepAdaptor_Surface.hxx>

  /* fix: use generated dynamic casting instead! */
  SP::OccBody cast_OccBody(SP::DynamicalSystem ds)
  {
    return std11::dynamic_pointer_cast<OccBody>(ds);
  };

  SP::SiconosVector facePoint(const TopoDS_Face &face,
                              double u, double v)
  {
    SP::SiconosVector presult(new SiconosVector(3));
    SiconosVector& result = *presult;

    BRepAdaptor_Surface SF(face);
    gp_Pnt aPaux;
    SF.D0((Standard_Real) u, (Standard_Real) v,aPaux);
    result.setValue(0, aPaux.X());
    result.setValue(1, aPaux.Y());
    result.setValue(2, aPaux.Z());

    return presult;
  }

%}
