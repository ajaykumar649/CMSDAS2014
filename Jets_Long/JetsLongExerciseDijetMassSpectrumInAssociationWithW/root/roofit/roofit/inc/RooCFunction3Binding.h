/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 *    File: $Id: RooCFunction3Binding.h 36207 2010-10-08 19:00:29Z wouter $
 * Authors:                                                                   *   WV, Wouter Verkerke, NIKHEF, verkerke@nikhef.nl                         *
 *                                                                           *
 * Copyright (c) 2000-2008, NIKHEF, Regents of the University of California  *
 *                          and Stanford University. All rights reserved.    *
 *                                                                           *
 *****************************************************************************/

#ifndef ROOCFUNCTION3BINDING
#define ROOCFUNCTION3BINDING

#include "TString.h"
#include "RooAbsReal.h"
#include "RooRealProxy.h"
#include "RooMsgService.h"
#include "RooAbsPdf.h"
#include <string>
#include <map>
#include <vector>


namespace RooFit {

typedef Double_t (*CFUNCD3DDD)(Double_t,Double_t,Double_t) ;
typedef Double_t (*CFUNCD3DDB)(Double_t,Double_t,Bool_t) ;
typedef Double_t (*CFUNCD3DII)(Double_t,Int_t,Int_t) ;
typedef Double_t (*CFUNCD3UDU)(UInt_t,Double_t,UInt_t) ;
typedef Double_t (*CFUNCD3UDD)(UInt_t,Double_t,Double_t) ;
typedef Double_t (*CFUNCD3UUD)(UInt_t,UInt_t,Double_t) ;

RooAbsReal* bindFunction(const char* name,void* func,RooAbsReal& x, RooAbsReal& y, RooAbsReal& z) ;
RooAbsPdf* bindPdf(const char* name,void* func,RooAbsReal& x, RooAbsReal& y, RooAbsReal& z) ;
#ifndef __CINT__
RooAbsReal* bindFunction(const char* name,CFUNCD3DDD func,RooAbsReal& x, RooAbsReal& y, RooAbsReal& z) ;
RooAbsReal* bindFunction(const char* name,CFUNCD3DDB func,RooAbsReal& x, RooAbsReal& y, RooAbsReal& z) ;
RooAbsReal* bindFunction(const char* name,CFUNCD3DII func,RooAbsReal& x, RooAbsReal& y, RooAbsReal& z) ;
RooAbsReal* bindFunction(const char* name,CFUNCD3UDU func,RooAbsReal& x, RooAbsReal& y, RooAbsReal& z) ;
RooAbsReal* bindFunction(const char* name,CFUNCD3UDD func,RooAbsReal& x, RooAbsReal& y, RooAbsReal& z) ;
RooAbsReal* bindFunction(const char* name,CFUNCD3UUD func,RooAbsReal& x, RooAbsReal& y, RooAbsReal& z) ;
RooAbsPdf* bindPdf(const char* name,CFUNCD3DDD func,RooAbsReal& x, RooAbsReal& y, RooAbsReal& z) ;
RooAbsPdf* bindPdf(const char* name,CFUNCD3DDB func,RooAbsReal& x, RooAbsReal& y, RooAbsReal& z) ;
RooAbsPdf* bindPdf(const char* name,CFUNCD3DII func,RooAbsReal& x, RooAbsReal& y, RooAbsReal& z) ;
RooAbsPdf* bindPdf(const char* name,CFUNCD3UDU func,RooAbsReal& x, RooAbsReal& y, RooAbsReal& z) ;
RooAbsPdf* bindPdf(const char* name,CFUNCD3UDD func,RooAbsReal& x, RooAbsReal& y, RooAbsReal& z) ;
RooAbsPdf* bindPdf(const char* name,CFUNCD3UUD func,RooAbsReal& x, RooAbsReal& y, RooAbsReal& z) ;
#endif

}

template<class VO, class VI1, class VI2, class VI3>
class RooCFunction3Map {
 public:
  RooCFunction3Map() {} ;

  void add(const char* name, VO (*ptr)(VI1,VI2,VI3), const char* arg1name="x", const char* arg2name="y", const char* arg3name="z") {
    // Register function with given name and argument name
    _ptrmap[name] = ptr ;
    _namemap[ptr] = name ;
    _argnamemap[ptr].push_back(arg1name) ;
    _argnamemap[ptr].push_back(arg2name) ;
    _argnamemap[ptr].push_back(arg3name) ;
  }
  

  const char* lookupName(VO (*ptr)(VI1,VI2,VI3)) {
    // Return name of function given by pointer
    return _namemap[ptr].c_str() ;
  }
    
  VO (*lookupPtr(const char* name))(VI1,VI2,VI3) {
    // Return pointer of function given by name
    return _ptrmap[name] ;
  }

  const char* lookupArgName(VO (*ptr)(VI1,VI2,VI3), UInt_t iarg) {
    // Return name of i-th argument of function. If function is
    // not registered, argument names 0,1,2 are x,y,z
    if (iarg<_argnamemap[ptr].size()) {
      return (_argnamemap[ptr])[iarg].c_str() ;
    }
    switch (iarg) {
    case 0: return "x" ;
    case 1: return "y" ;
    case 2: return "z" ;
    }
    return "w" ;
  }
  
 private:

#ifndef __CINT__
  std::map<std::string,VO (*)(VI1,VI2,VI3)> _ptrmap ; // Pointer-to-name map
  std::map<VO (*)(VI1,VI2,VI3),std::string> _namemap ; // Name-to-pointer map
  std::map<VO (*)(VI1,VI2,VI3),std::vector<std::string> > _argnamemap ; // Pointer-to-argnamelist map
#endif
} ;



template<class VO, class VI1, class VI2, class VI3>
class RooCFunction3Ref : public TObject {
 public:
  RooCFunction3Ref(VO (*ptr)(VI1,VI2,VI3)=0) : _ptr(ptr) {
    // Constructor of persistable function reference
  } ;
  ~RooCFunction3Ref() {} ;

  VO operator()(VI1 x,VI2 y, VI3 z) const {
    // Evaluate embedded function
    return (*_ptr)(x,y,z) ;
  }

  const char* name() const {
    // Return registered name of embedded function. If function
    // is not registered return string with hex presentation
    // of function pointer value
    const char* result = fmap().lookupName(_ptr) ;
    if (result && strlen(result)) {
      return result ;
    } 
    // This union is to avoid a warning message:
    union { 
       void *_ptr;
       func_t _funcptr;
    } temp;
    temp._funcptr = _ptr;
    return Form("(%p)",temp._ptr) ;
  }

  const char* argName(Int_t iarg) {
    // Return suggested name for i-th argument
    return fmap().lookupArgName(_ptr,iarg) ;    
  }

  static RooCFunction3Map<VO,VI1,VI2,VI3>& fmap() {
    // Return reference to function pointer-to-name mapping service
    if (!_fmap) {
      _fmap = new RooCFunction3Map<VO,VI1,VI2,VI3> ;
    }
    return *_fmap ;
  }

 private:

  static VO dummyFunction(VI1,VI2,VI3) {
    // Dummy function used when registered function was not
    // found in un-persisting object
    return 0 ;
  }


  typedef VO (*func_t)(VI1,VI2,VI3) ; //! Pointer to embedded function
  func_t _ptr; //! Pointer to embedded function

  static RooCFunction3Map<VO,VI1,VI2,VI3>* _fmap ; // Pointer to mapping service object

  ClassDef(RooCFunction3Ref,1) // Persistable reference to C function pointer
} ;



template<class VO, class VI1, class VI2, class VI3>
void RooCFunction3Ref<VO,VI1,VI2,VI3>::Streamer(TBuffer &R__b)
{
  // Custom streamer for function pointer reference object. When writing,
  // the function pointer is substituted by its registerd name. When function
  // is unregistered name 'UNKNOWN' is written and a warning is issues. When
  // reading back, the embedded name is converted back to a function pointer
  // using the mapping service. When name UNKNOWN is encountered a warning is
  // issues and a dummy null function is substituted. When the registered function
  // name can not be mapped to a function pointer an ERROR is issued and a pointer
  // to the dummy null function is substituted

  typedef ::RooCFunction3Ref<VO,VI1,VI2,VI3> thisClass;

   // Stream an object of class RooCFunction3Ref
   if (R__b.IsReading()) {

     UInt_t R__s, R__c;
     Version_t R__v = R__b.ReadVersion(&R__s, &R__c);      

     // Read name from file
     TString tmpName ;
     tmpName.Streamer(R__b) ;       

     if (tmpName=="UNKNOWN" && R__v>0) {

       coutW(ObjectHandling) << "WARNING: Objected embeds function pointer to unknown function, object will not be functional" << endl ;
       _ptr = dummyFunction ;

     } else {
     
       // Lookup pointer to C function wih given name
       _ptr = fmap().lookupPtr(tmpName.Data()) ;

       if (_ptr==0) {
	 coutW(ObjectHandling) << "ERROR: Objected embeds pointer to function named " << tmpName 
			       << " but no such function is registered, object will not be functional" << endl ;
       }
     }


     R__b.CheckByteCount(R__s, R__c, thisClass::IsA());

   } else {
     
     UInt_t R__c;
     R__c = R__b.WriteVersion(thisClass::IsA(), kTRUE);    

     // Lookup name of reference C function
     TString tmpName = fmap().lookupName(_ptr) ;
     if (tmpName.Length()==0) {
       // This union is to avoid a warning message:
       union { 
          void *_ptr;
          func_t _funcptr;
       } temp;
       temp._funcptr = _ptr;
       coutW(ObjectHandling) << "WARNING: Cannot persist unknown function pointer " << Form("%p",temp._ptr) 
			     << " written object will not be functional when read back" <<  endl ;
       tmpName="UNKNOWN" ;
     } 
     
     // Persist the name
     tmpName.Streamer(R__b) ;            

     R__b.SetByteCount(R__c, kTRUE);
     
   }
}



template<class VO,class VI1, class VI2, class VI3>
class RooCFunction3Binding : public RooAbsReal {
public:
  RooCFunction3Binding() {
    // Default constructor
  } ; 
  RooCFunction3Binding(const char *name, const char *title, VO (*_func)(VI1,VI2,VI3), RooAbsReal& _x, RooAbsReal& _y, RooAbsReal& _z);
  RooCFunction3Binding(const RooCFunction3Binding& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooCFunction3Binding(*this,newname); }
  inline virtual ~RooCFunction3Binding() { }

  void printArgs(ostream& os) const {
    // Print object arguments and name/address of function pointer
    os << "[ function=" << func.name() << " " ;    
    for (Int_t i=0 ; i<numProxies() ; i++) {
      RooAbsProxy* p = getProxy(i) ;
      if (!TString(p->name()).BeginsWith("!")) {
	p->print(os) ;
	os << " " ;
      }
    }    
    os << "]" ;  
  }

protected:

  RooCFunction3Ref<VO,VI1,VI2,VI3> func ; // Function pointer reference
  RooRealProxy x ;              // Argument reference
  RooRealProxy y ;              // Argument reference
  RooRealProxy z ;              // Argument reference
  
  Double_t evaluate() const {
    // Return value of embedded function using value of referenced variable x
    return func(x,y,z) ;
  }

private:

  ClassDef(RooCFunction3Binding,1) // RooAbsReal binding to external C functions
};



template<class VO,class VI1, class VI2, class VI3>
RooCFunction3Binding<VO,VI1,VI2,VI3>::RooCFunction3Binding(const char *name, const char *title, VO (*_func)(VI1,VI2,VI3), 
						       RooAbsReal& _x, RooAbsReal& _y, RooAbsReal& _z) :
  RooAbsReal(name,title), 
  func(_func),
  x(func.argName(0),func.argName(0),this,_x),
  y(func.argName(1),func.argName(1),this,_y),
  z(func.argName(2),func.argName(2),this,_z)
{ 
  // Constructor of C function binding object given a pointer to a function and a RooRealVar to which the function
  // argument should be bound. This object is fully functional as a RooFit function object. The only restriction is
  // if the referenced function is _not_ a standard ROOT TMath or MathCore function it can not be persisted in a
  // a RooWorkspace
} 


template<class VO,class VI1, class VI2, class VI3>
RooCFunction3Binding<VO,VI1,VI2,VI3>::RooCFunction3Binding(const RooCFunction3Binding& other, const char* name) :  
  RooAbsReal(other,name), 
  func(other.func),
  x("x",this,other.x),
  y("y",this,other.y),
  z("z",this,other.z)
{ 
  // Copy constructor
} 


template<class VO,class VI1, class VI2, class VI3>
class RooCFunction3PdfBinding : public RooAbsPdf {
public:
  RooCFunction3PdfBinding() {
    // Default constructor
  } ; 
  RooCFunction3PdfBinding(const char *name, const char *title, VO (*_func)(VI1,VI2,VI3), RooAbsReal& _x, RooAbsReal& _y, RooAbsReal& _z);
  RooCFunction3PdfBinding(const RooCFunction3PdfBinding& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooCFunction3PdfBinding(*this,newname); }
  inline virtual ~RooCFunction3PdfBinding() { }

  void printArgs(ostream& os) const {
    // Print object arguments and name/address of function pointer
    os << "[ function=" << func.name() << " " ;    
    for (Int_t i=0 ; i<numProxies() ; i++) {
      RooAbsProxy* p = getProxy(i) ;
      if (!TString(p->name()).BeginsWith("!")) {
	p->print(os) ;
	os << " " ;
      }
    }    
    os << "]" ;  
  }

protected:

  RooCFunction3Ref<VO,VI1,VI2,VI3> func ; // Function pointer reference
  RooRealProxy x ;              // Argument reference
  RooRealProxy y ;              // Argument reference
  RooRealProxy z ;              // Argument reference
  
  Double_t evaluate() const {
    // Return value of embedded function using value of referenced variable x
    return func(x,y,z) ;
  }

private:

  ClassDef(RooCFunction3PdfBinding,1) // RooAbsReal binding to external C functions
};



template<class VO,class VI1, class VI2, class VI3>
RooCFunction3PdfBinding<VO,VI1,VI2,VI3>::RooCFunction3PdfBinding(const char *name, const char *title, VO (*_func)(VI1,VI2,VI3), 
						       RooAbsReal& _x, RooAbsReal& _y, RooAbsReal& _z) :
  RooAbsPdf(name,title), 
  func(_func),
  x(func.argName(0),func.argName(0),this,_x),
  y(func.argName(1),func.argName(1),this,_y),
  z(func.argName(2),func.argName(2),this,_z)
{ 
  // Constructor of C function binding object given a pointer to a function and a RooRealVar to which the function
  // argument should be bound. This object is fully functional as a RooFit function object. The only restriction is
  // if the referenced function is _not_ a standard ROOT TMath or MathCore function it can not be persisted in a
  // a RooWorkspace
} 


template<class VO,class VI1, class VI2, class VI3>
RooCFunction3PdfBinding<VO,VI1,VI2,VI3>::RooCFunction3PdfBinding(const RooCFunction3PdfBinding& other, const char* name) :  
  RooAbsPdf(other,name), 
  func(other.func),
  x("x",this,other.x),
  y("y",this,other.y),
  z("z",this,other.z)
{ 
  // Copy constructor
} 

#endif