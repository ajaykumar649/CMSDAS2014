// @(#)root/io:$Id: TVirtualArray.h 25450 2008-09-18 21:13:42Z pcanal $
// Author: Philippe Canal July, 2008

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
  *************************************************************************/

#ifndef ROOT_TVirtualArray
#define ROOT_TVirtualArray


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TVirtualArray                                                       //
//                                                                      //
// Wrapper around an object and giving indirect access to its content    //
// even if the object is not of a class in the Cint/Reflex dictionary.  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TClassRef.h"

class TVirtualArray {
public:
   TClassRef  fClass;
   UInt_t     fSize; 
   char      *fArray; //[fSize] 

   TVirtualArray( TClass *cl, UInt_t size ) : fClass(cl), fSize(size), fArray( (char*)( cl ? cl->NewArray(size) : 0) ) {};
   ~TVirtualArray() { if (fClass) fClass->DeleteArray( fArray ); }

   TClass *GetClass() { return fClass; }
   char *operator[](UInt_t ind) const { return GetObjectAt(ind); }
   char *GetObjectAt(UInt_t ind) const { return fArray+fClass->Size()*ind; }

};

#endif // ROOT_TVirtualArray
