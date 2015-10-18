// File generated by CPPExt (Value)
//
//                     Copyright (C) 1991 - 2000 by  
//                      Matra Datavision SA.  All rights reserved.
//  
//                     Copyright (C) 2001 - 2004 by
//                     Open CASCADE SA.  All rights reserved.
// 
// This file is part of the Open CASCADE Technology software.
//
// This software may be distributed and/or modified under the terms and
// conditions of the Open CASCADE Public License as defined by Open CASCADE SA
// and appearing in the file LICENSE included in the packaging of this file.
//  
// This software is distributed on an "AS IS" basis, without warranty of any
// kind, and Open CASCADE SA hereby disclaims all such warranties,
// including without limitation, any warranties of merchantability, fitness
// for a particular purpose or non-infringement. Please see the License for
// the specific terms and conditions governing rights and limitations under the
// License.

#ifndef _myBRepExtrema_ExtFF_HeaderFile
#define _myBRepExtrema_ExtFF_HeaderFile

#ifndef _myExtrema_ExtSS_HeaderFile
#include "myExtrema_ExtSS.hxx"
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _TColStd_SequenceOfReal_HeaderFile
#include <TColStd_SequenceOfReal.hxx>
#endif
#ifndef _Extrema_SequenceOfPOnSurf_HeaderFile
#include <Extrema_SequenceOfPOnSurf.hxx>
#endif
#ifndef _Handle_BRepAdaptor_HSurface_HeaderFile
#include <Handle_BRepAdaptor_HSurface.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
class BRepAdaptor_HSurface;
class StdFail_NotDone;
class Standard_OutOfRange;
class Standard_TypeMismatch;
class TopoDS_Face;
class gp_Pnt;


#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

/** 
 * \brief This class has been built from OCC in view of overloding the distance computation between CAD objects.
 */

class myBRepExtrema_ExtFF  {

public:

    void* operator new(size_t,void* anAddress) 
      {
        return anAddress;
      }
    void* operator new(size_t size) 
      { 
        return Standard::Allocate(size); 
      }
    void  operator delete(void *anAddress) 
      { 
        if (anAddress) Standard::Free((Standard_Address&)anAddress); 
      }
 // Methods PUBLIC
 // 


Standard_EXPORT myBRepExtrema_ExtFF();

//! It calculates all the distances. <br>
Standard_EXPORT myBRepExtrema_ExtFF(const TopoDS_Face& F1,const TopoDS_Face& F2);

Standard_EXPORT myBRepExtrema_ExtFF(const TopoDS_Face& F1,const TopoDS_Face& F2,
				    int id1,
				    int id2);
Standard_EXPORT   void Initialize(const TopoDS_Face& F2) ;

//! An exception is raised if the fields have not been <br>
//!          initialized. <br>
//!          Be careful: this method uses the Face F2 only for <br>
//!          classify, not for the fields. <br>
Standard_EXPORT   void Perform(const TopoDS_Face& F1,const TopoDS_Face& F2) ;

//! True if the distances are found. <br>
Standard_EXPORT   Standard_Boolean IsDone() const;

//! Returns True if the surfaces are parallel. <br>
Standard_EXPORT   Standard_Boolean IsParallel() const;

//! Returns the number of extremum distances. <br>
Standard_EXPORT   Standard_Integer NbExt() const;

//! Returns the value of the <N>th extremum distance. <br>
Standard_EXPORT   Standard_Real Value(const Standard_Integer N) const;

//! Returns the parameters on the  Face F1 of the  <N>th <br>
//!          extremum distance. <br>
Standard_EXPORT   void ParameterOnFace1(const Standard_Integer N,Standard_Real& U,Standard_Real& V) const;

//! Returns the parameters on the  Face F2 of the  <N>th <br>
//!          extremum distance. <br>
Standard_EXPORT   void ParameterOnFace2(const Standard_Integer N,Standard_Real& U,Standard_Real& V) const;

//! Returns the Point of the <N>th extremum distance. <br>
Standard_EXPORT   gp_Pnt PointOnFace1(const Standard_Integer N) const;

//! Returns the Point of the <N>th extremum distance. <br>
Standard_EXPORT   gp_Pnt PointOnFace2(const Standard_Integer N) const;

  int _id1;
  int _id2;



protected:

 // Methods PROTECTED
 // 


 // Fields PROTECTED
 //


private: 

 // Methods PRIVATE
 // 


 // Fields PRIVATE
 //
myExtrema_ExtSS myExtrem;
Standard_Integer mynbext;
TColStd_SequenceOfReal mydist;
Extrema_SequenceOfPOnSurf myPointsOnS1;
Extrema_SequenceOfPOnSurf myPointsOnS2;
Handle_BRepAdaptor_HSurface myHS;


};





// other Inline functions and methods (like "C++: function call" methods)
//


#endif