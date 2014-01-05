/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 * @(#)root/roofitcore:$Id: RooAddPdf.cxx 36230 2010-10-09 20:21:02Z wouter $
 * Authors:                                                                  *
 *   WV, Wouter Verkerke, UC Santa Barbara, verkerke@slac.stanford.edu       *
 *   DK, David Kirkby,    UC Irvine,         dkirkby@uci.edu                 *
 *                                                                           *
 * Copyright (c) 2000-2005, Regents of the University of California          *
 *                          and Stanford University. All rights reserved.    *
 *                                                                           *
 * Redistribution and use in source and binary forms,                        *
 * with or without modification, are permitted according to the terms        *
 * listed in LICENSE (http://roofit.sourceforge.net/license.txt)             *
 *****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
// 
// RooAddPdf is an efficient implementation of a sum of PDFs of the form 
//
//  c_1*PDF_1 + c_2*PDF_2 + ... c_n*PDF_n 
//
// or 
//
//  c_1*PDF_1 + c_2*PDF_2 + ... (1-sum(c_1...c_n-1))*PDF_n 
//
// The first form is for extended likelihood fits, where the
// expected number of events is Sum(i) c_i. The coefficients c_i
// can either be explicitly provided, or, if all components support
// extended likelihood fits, they can be calculated the contribution
// of each PDF to the total number of expected events.
//
// In the second form, the sum of the coefficients is enforced to be one,
// and the coefficient of the last PDF is calculated from that condition.
//
// It is also possible to parameterize the coefficients recursively
// 
// c1*PDF_1 + (1-c1)(c2*PDF_2 + (1-c2)*(c3*PDF_3 + ....))
//
// In this form the sum of the coefficients is always less than 1.0
// for all possible values of the individual coefficients between 0 and 1.
// 
// RooAddPdf relies on each component PDF to be normalized and will perform 
// no normalization other than calculating the proper last coefficient c_n, if requested.
// An (enforced) condition for this assuption is that each PDF_i is independent
// of each coefficient_i.
//
// 

#include "RooFit.h"
#include "RooMsgService.h"

#include "TIterator.h"
#include "TIterator.h"
#include "TList.h"
#include "RooAddPdf.h"
#include "RooDataSet.h"
#include "RooRealProxy.h"
#include "RooPlot.h"
#include "RooRealVar.h"
#include "RooAddGenContext.h"
#include "RooRealConstant.h"
#include "RooNameReg.h"
#include "RooMsgService.h"
#include "RooRecursiveFraction.h"
#include "RooGlobalFunc.h"
#include "RooRealIntegral.h"

#include "Riostream.h"
#include <algorithm>


ClassImp(RooAddPdf)
;


//_____________________________________________________________________________
RooAddPdf::RooAddPdf() :
  _refCoefNorm("!refCoefNorm","Reference coefficient normalization set",this,kFALSE,kFALSE),
  _refCoefRangeName(0),
  _codeReg(10),
  _snormList(0)
{
  // Default constructor used for persistence

  _pdfIter   = _pdfList.createIterator() ;
  _coefIter  = _coefList.createIterator() ;

  _coefCache = new Double_t[10] ;
  _coefErrCount = _errorCount ;
}



//_____________________________________________________________________________
RooAddPdf::RooAddPdf(const char *name, const char *title) :
  RooAbsPdf(name,title), 
  _refCoefNorm("!refCoefNorm","Reference coefficient normalization set",this,kFALSE,kFALSE),
  _refCoefRangeName(0),
  _projectCoefs(kFALSE),
  _projCacheMgr(this,10),
  _codeReg(10),
  _pdfList("!pdfs","List of PDFs",this),
  _coefList("!coefficients","List of coefficients",this),
  _snormList(0),
  _haveLastCoef(kFALSE),
  _allExtendable(kFALSE)
{
  // Dummy constructor 
  _pdfIter   = _pdfList.createIterator() ;
  _coefIter  = _coefList.createIterator() ;

  _coefCache = new Double_t[10] ;
  _coefErrCount = _errorCount ;

}



//_____________________________________________________________________________
RooAddPdf::RooAddPdf(const char *name, const char *title,
		     RooAbsPdf& pdf1, RooAbsPdf& pdf2, RooAbsReal& coef1) : 
  RooAbsPdf(name,title),
  _refCoefNorm("!refCoefNorm","Reference coefficient normalization set",this,kFALSE,kFALSE),
  _refCoefRangeName(0),
  _projectCoefs(kFALSE),
  _projCacheMgr(this,10),
  _codeReg(10),
  _pdfList("!pdfs","List of PDFs",this),
  _coefList("!coefficients","List of coefficients",this),
  _haveLastCoef(kFALSE),
  _allExtendable(kFALSE)
{
  // Constructor with two PDFs and one coefficient

  _pdfIter  = _pdfList.createIterator() ;
  _coefIter = _coefList.createIterator() ;

  _pdfList.add(pdf1) ;  
  _pdfList.add(pdf2) ;
  _coefList.add(coef1) ;

  _coefCache = new Double_t[_pdfList.getSize()] ;
  _coefErrCount = _errorCount ;
}



//_____________________________________________________________________________
RooAddPdf::RooAddPdf(const char *name, const char *title, const RooArgList& inPdfList, const RooArgList& inCoefList, Bool_t recursiveFractions) :
  RooAbsPdf(name,title),
  _refCoefNorm("!refCoefNorm","Reference coefficient normalization set",this,kFALSE,kFALSE),
  _refCoefRangeName(0),
  _projectCoefs(kFALSE),
  _projCacheMgr(this,10),
  _codeReg(10),
  _pdfList("!pdfs","List of PDFs",this),
  _coefList("!coefficients","List of coefficients",this),
  _haveLastCoef(kFALSE),
  _allExtendable(kFALSE)
{ 
  // Generic constructor from list of PDFs and list of coefficients.
  // Each pdf list element (i) is paired with coefficient list element (i).
  // The number of coefficients must be either equal to the number of PDFs,
  // in which case extended MLL fitting is enabled, or be one less.
  //
  // All PDFs must inherit from RooAbsPdf. All coefficients must inherit from RooAbsReal
  //
  // If the recursiveFraction flag is true, the coefficients are interpreted as recursive
  // coefficients as explained in the class description.

  if (inPdfList.getSize()>inCoefList.getSize()+1 || inPdfList.getSize()<inCoefList.getSize()) {
    coutE(InputArguments) << "RooAddPdf::RooAddPdf(" << GetName() 
			  << ") number of pdfs and coefficients inconsistent, must have Npdf=Ncoef or Npdf=Ncoef+1" << endl ;
    assert(0) ;
  }

  if (recursiveFractions && inPdfList.getSize()!=inCoefList.getSize()+1) {
    coutW(InputArguments) << "RooAddPdf::RooAddPdf(" << GetName() 
			  << ") WARNING inconsistent input: recursive fractions options can only be used if Npdf=Ncoef+1, ignoring recursive fraction setting" << endl ;
  }


  _pdfIter  = _pdfList.createIterator() ;
  _coefIter = _coefList.createIterator() ;
 
  // Constructor with N PDFs and N or N-1 coefs
  TIterator* pdfIter = inPdfList.createIterator() ;
  TIterator* coefIter = inCoefList.createIterator() ;
  RooAbsPdf* pdf ;
  RooAbsReal* coef ;

  RooArgList partinCoefList ;

  Bool_t first(kTRUE) ;

  while((coef = (RooAbsPdf*)coefIter->Next())) {
    pdf = (RooAbsPdf*) pdfIter->Next() ;
    if (!pdf) {
      coutE(InputArguments) << "RooAddPdf::RooAddPdf(" << GetName() 
			    << ") number of pdfs and coefficients inconsistent, must have Npdf=Ncoef or Npdf=Ncoef+1" << endl ;
      assert(0) ;
    }
    if (!dynamic_cast<RooAbsReal*>(coef)) {
      coutE(InputArguments) << "RooAddPdf::RooAddPdf(" << GetName() << ") coefficient " << coef->GetName() << " is not of type RooAbsReal, ignored" << endl ;
      continue ;
    }
    if (!dynamic_cast<RooAbsReal*>(pdf)) {
      coutE(InputArguments) << "RooAddPdf::RooAddPdf(" << GetName() << ") pdf " << pdf->GetName() << " is not of type RooAbsPdf, ignored" << endl ;
      continue ;
    }
    _pdfList.add(*pdf) ;

    // Process recursive fraction mode separately
    if (recursiveFractions) {
      partinCoefList.add(*coef) ;
      if (first) {	

	// The first fraction is the first plain fraction
	first = kFALSE ;
	_coefList.add(*coef) ;

      } else {

	// The i-th recursive fraction = (1-f1)*(1-f2)*...(fi) and is calculated from the list (f1,...,fi) by RooRecursiveFraction)
	RooAbsReal* rfrac = new RooRecursiveFraction(Form("%s_recursive_fraction_%s",GetName(),pdf->GetName()),"Recursive Fraction",partinCoefList) ;
	addOwnedComponents(*rfrac) ;
	_coefList.add(*rfrac) ;      	

      }

    } else {
      _coefList.add(*coef) ;    
    }
  }

  pdf = (RooAbsPdf*) pdfIter->Next() ;
  if (pdf) {
    if (!dynamic_cast<RooAbsReal*>(pdf)) {
      coutE(InputArguments) << "RooAddPdf::RooAddPdf(" << GetName() << ") last pdf " << coef->GetName() << " is not of type RooAbsPdf, fatal error" << endl ;
      assert(0) ;
    }
    _pdfList.add(*pdf) ;  

    // Process recursive fractions mode
    if (recursiveFractions) {

      // The last recursive fraction = (1-f1)*(1-f2)*...(1-fN) and is calculated from the list (f1,...,fN,1) by RooRecursiveFraction)
      partinCoefList.add(RooFit::RooConst(1)) ;
      RooAbsReal* rfrac = new RooRecursiveFraction(Form("%s_recursive_fraction_%s",GetName(),pdf->GetName()),"Recursive Fraction",partinCoefList) ;
      addOwnedComponents(*rfrac) ;
      _coefList.add(*rfrac) ;      	

      // In recursive mode we always have Ncoef=Npdf
      _haveLastCoef=kTRUE ;
    }

  } else {
    _haveLastCoef=kTRUE ;
  }

  delete pdfIter ;
  delete coefIter  ;

  _coefCache = new Double_t[_pdfList.getSize()] ;
  _coefErrCount = _errorCount ;
}



//_____________________________________________________________________________
RooAddPdf::RooAddPdf(const char *name, const char *title, const RooArgList& inPdfList) :
  RooAbsPdf(name,title),
  _refCoefNorm("!refCoefNorm","Reference coefficient normalization set",this,kFALSE,kFALSE),
  _refCoefRangeName(0),
  _projectCoefs(kFALSE),
  _projCacheMgr(this,10),
  _pdfList("!pdfs","List of PDFs",this),
  _coefList("!coefficients","List of coefficients",this),
  _haveLastCoef(kFALSE),
  _allExtendable(kTRUE)
{ 
  // Generic constructor from list of extended PDFs. There are no coefficients as the expected
  // number of events from each components determine the relative weight of the PDFs.
  // 
  // All PDFs must inherit from RooAbsPdf. 

  _pdfIter  = _pdfList.createIterator() ;
  _coefIter = _coefList.createIterator() ;
 
  // Constructor with N PDFs 
  TIterator* pdfIter = inPdfList.createIterator() ;
  RooAbsPdf* pdf ;
  while((pdf = (RooAbsPdf*) pdfIter->Next())) {
    
    if (!dynamic_cast<RooAbsReal*>(pdf)) {
      coutE(InputArguments) << "RooAddPdf::RooAddPdf(" << GetName() << ") pdf " << pdf->GetName() << " is not of type RooAbsPdf, ignored" << endl ;
      continue ;
    }
    if (!pdf->canBeExtended()) {
      coutE(InputArguments) << "RooAddPdf::RooAddPdf(" << GetName() << ") pdf " << pdf->GetName() << " is not extendable, ignored" << endl ;
      continue ;
    }
    _pdfList.add(*pdf) ;    
  }

  delete pdfIter ;

  _coefCache = new Double_t[_pdfList.getSize()] ;
  _coefErrCount = _errorCount ;
}




//_____________________________________________________________________________
RooAddPdf::RooAddPdf(const RooAddPdf& other, const char* name) :
  RooAbsPdf(other,name),
  _refCoefNorm("!refCoefNorm",this,other._refCoefNorm),
  _refCoefRangeName((TNamed*)other._refCoefRangeName),
  _projectCoefs(other._projectCoefs),
  _projCacheMgr(other._projCacheMgr,this),
  _codeReg(other._codeReg),
  _pdfList("!pdfs",this,other._pdfList),
  _coefList("!coefficients",this,other._coefList),
  _haveLastCoef(other._haveLastCoef),
  _allExtendable(other._allExtendable)
{
  // Copy constructor

  _pdfIter  = _pdfList.createIterator() ;
  _coefIter = _coefList.createIterator() ;
  _coefCache = new Double_t[_pdfList.getSize()] ;
  _coefErrCount = _errorCount ;
}



//_____________________________________________________________________________
RooAddPdf::~RooAddPdf()
{
  // Destructor

  delete _pdfIter ;
  delete _coefIter ;

  if (_coefCache) delete[] _coefCache ;
}



//_____________________________________________________________________________
void RooAddPdf::fixCoefNormalization(const RooArgSet& refCoefNorm) 
{
  // By default the interpretation of the fraction coefficients is
  // performed in the contextual choice of observables. This makes the
  // shape of the p.d.f explicitly dependent on the choice of
  // observables. This method instructs RooAddPdf to freeze the
  // interpretation of the coefficients to be done in the given set of
  // observables. If frozen, fractions are automatically transformed
  // from the reference normalization set to the contextual normalization
  // set by ratios of integrals

  if (refCoefNorm.getSize()==0) {
    _projectCoefs = kFALSE ;
    return ;
  }
  _projectCoefs = kTRUE ;  

  _refCoefNorm.removeAll() ;
  _refCoefNorm.add(refCoefNorm) ;

  _projCacheMgr.reset() ;
}



//_____________________________________________________________________________
void RooAddPdf::fixCoefRange(const char* rangeName)
{
  // By default the interpretation of the fraction coefficients is
  // performed in the default range. This make the shape of a RooAddPdf
  // explicitly dependent on the range of the observables. To allow
  // a range independent definition of the fraction this function
  // instructs RooAddPdf to freeze its interpretation in the given
  // named range. If the current normalization range is different
  // from the reference range, the appropriate fraction coefficients
  // are automically calculation from the reference fractions using
  // ratios if integrals

  _refCoefRangeName = (TNamed*)RooNameReg::ptr(rangeName) ;
  if (_refCoefRangeName) _projectCoefs = kTRUE ;
}



//_____________________________________________________________________________
RooAddPdf::CacheElem* RooAddPdf::getProjCache(const RooArgSet* nset, const RooArgSet* iset, const char* rangeName) const
{
  // Retrieve cache element with for calculation of p.d.f value with normalization set nset and integrated over iset
  // in range 'rangeName'. If cache element does not exist, create and fill it on the fly. The cache contains
  // suplemental normalization terms (in case not all added p.d.f.s have the same observables), projection
  // integrals to calculated transformed fraction coefficients when a frozen reference frame is provided
  // and projection integrals for similar transformations when a frozen reference range is provided.


  // Check if cache already exists 
  CacheElem* cache = (CacheElem*) _projCacheMgr.getObj(nset,iset,0,RooNameReg::ptr(rangeName)) ;
  if (cache) {
    return cache ;
  }

  //Create new cache 
  cache = new CacheElem ;

  // *** PART 1 : Create supplemental normalization list ***

  // Retrieve the combined set of dependents of this PDF ;
  RooArgSet *fullDepList = getObservables(nset) ;
  if (iset) {
    fullDepList->remove(*iset,kTRUE,kTRUE) ;
  }    

  // Fill with dummy unit RRVs for now
  _pdfIter->Reset() ;
  _coefIter->Reset() ;
  RooAbsPdf* pdf ;
  RooAbsReal* coef ;
  while((pdf=(RooAbsPdf*)_pdfIter->Next())) {    
    coef=(RooAbsPdf*)_coefIter->Next() ;

    // Start with full list of dependents
    RooArgSet supNSet(*fullDepList) ;

    // Remove PDF dependents
    RooArgSet* pdfDeps = pdf->getObservables(nset) ;
    if (pdfDeps) {
      supNSet.remove(*pdfDeps,kTRUE,kTRUE) ;
      delete pdfDeps ; 
    }

    // Remove coef dependents
    RooArgSet* coefDeps = coef ? coef->getObservables(nset) : 0 ;
    if (coefDeps) {
      supNSet.remove(*coefDeps,kTRUE,kTRUE) ;
      delete coefDeps ;
    }
    
    RooAbsReal* snorm ;
    TString name(GetName()) ;
    name.Append("_") ;
    name.Append(pdf->GetName()) ;
    name.Append("_SupNorm") ;
    if (supNSet.getSize()>0) {
      snorm = new RooRealIntegral(name,"Supplemental normalization integral",RooRealConstant::value(1.0),supNSet) ;
      cxcoutD(Caching) << "RooAddPdf " << GetName() << " making supplemental normalization set " << supNSet << " for pdf component " << pdf->GetName() << endl ;
    } else {
      snorm = new RooRealVar(name,"Unit Supplemental normalization integral",1.0) ;
    }
    cache->_suppNormList.addOwned(*snorm) ;
  }

  delete fullDepList ;
    
  if (_verboseEval>1) {
    cxcoutD(Caching) << "RooAddPdf::syncSuppNormList(" << GetName() << ") synching supplemental normalization list for norm" << (nset?*nset:RooArgSet()) << endl ;
    if dologD(Caching) {
      cache->_suppNormList.Print("v") ;
    }
  }


  // *** PART 2 : Create projection coefficients ***

//   cout << " this = " << this << " (" << GetName() << ")" << endl ;
//   cout << "projectCoefs = " << (_projectCoefs?"T":"F") << endl ;
//   cout << "_normRange.Length() = " << _normRange.Length() << endl ;

  // If no projections required stop here
  if (!_projectCoefs && !rangeName) {
    _projCacheMgr.setObj(nset,iset,cache,RooNameReg::ptr(rangeName)) ;
//     cout << " no projection required" << endl ;
    return cache ;
  }


//   cout << "calculating projection" << endl ;

  // Reduce iset/nset to actual dependents of this PDF
  RooArgSet* nset2 = nset ? getObservables(nset) : new RooArgSet() ;
  cxcoutD(Caching) << "RooAddPdf(" << GetName() << ")::getPC nset = " << (nset?*nset:RooArgSet()) << " nset2 = " << *nset2 << endl ;

  if (nset2->getSize()==0 && _refCoefNorm.getSize()!=0) {
    //cout << "WVE: evaluating RooAddPdf without normalization, but have reference normalization for coefficient definition" << endl ;
    
    nset2->add(_refCoefNorm) ;
    if (_refCoefRangeName) {
      rangeName = RooNameReg::str(_refCoefRangeName) ;
    }
  }


  // Check if requested transformation is not identity 
  if (!nset2->equals(_refCoefNorm) || _refCoefRangeName !=0 || rangeName !=0 || _normRange.Length()>0) {
   
    cxcoutD(Caching) << "ALEX:     RooAddPdf::syncCoefProjList(" << GetName() << ") projecting coefficients from "
		   << *nset2 << (rangeName?":":"") << (rangeName?rangeName:"") 
		   << " to "  << ((_refCoefNorm.getSize()>0)?_refCoefNorm:*nset2) << (_refCoefRangeName?":":"") << (_refCoefRangeName?RooNameReg::str(_refCoefRangeName):"") << endl ;
    
    // Recalculate projection integrals of PDFs 
    _pdfIter->Reset() ;
    RooAbsPdf* thePdf ;

    while((thePdf=(RooAbsPdf*)_pdfIter->Next())) {

      // Calculate projection integral
      RooAbsReal* pdfProj ;
      if (!nset2->equals(_refCoefNorm)) {
	pdfProj = thePdf->createIntegral(*nset2,_refCoefNorm,_normRange.Length()>0?_normRange.Data():0) ;
	pdfProj->setOperMode(operMode()) ;
	cxcoutD(Caching) << "RooAddPdf(" << GetName() << ")::getPC nset2(" << *nset2 << ")!=_refCoefNorm(" << _refCoefNorm << ") --> pdfProj = " << pdfProj->GetName() << endl ;
      } else {
	TString name(GetName()) ;
	name.Append("_") ;
	name.Append(thePdf->GetName()) ;
	name.Append("_ProjectNorm") ;
	pdfProj = new RooRealVar(name,"Unit Projection normalization integral",1.0) ;
	cxcoutD(Caching) << "RooAddPdf(" << GetName() << ")::getPC nset2(" << *nset2 << ")==_refCoefNorm(" << _refCoefNorm << ") --> pdfProj = " << pdfProj->GetName() << endl ;
      }

      cache->_projList.addOwned(*pdfProj) ;
      cxcoutD(Caching) << " RooAddPdf::syncCoefProjList(" << GetName() << ") PP = " << pdfProj->GetName() << endl ;

      // Calculation optional supplemental normalization term
      RooArgSet supNormSet(_refCoefNorm) ;
      RooArgSet* deps = thePdf->getParameters(RooArgSet()) ;
      supNormSet.remove(*deps,kTRUE,kTRUE) ;
      delete deps ;

      RooAbsReal* snorm ;
      TString name(GetName()) ;
      name.Append("_") ;
      name.Append(thePdf->GetName()) ;
      name.Append("_ProjSupNorm") ;
      if (supNormSet.getSize()>0 && !nset2->equals(_refCoefNorm) ) {
	snorm = new RooRealIntegral(name,"Projection Supplemental normalization integral",
				    RooRealConstant::value(1.0),supNormSet) ;
      } else {
	snorm = new RooRealVar(name,"Unit Projection Supplemental normalization integral",1.0) ;
      }
      cxcoutD(Caching) << " RooAddPdf::syncCoefProjList(" << GetName() << ") SN = " << snorm->GetName() << endl ;
      cache->_suppProjList.addOwned(*snorm) ;

      // Calculate reference range adjusted projection integral
      RooAbsReal* rangeProj1 ;

   //    cout << "ALEX >>>> RooAddPdf(" << GetName() << ")::getPC _refCoefRangeName WVE = " 	   
// 	   <<(_refCoefRangeName?":":"") << (_refCoefRangeName?RooNameReg::str(_refCoefRangeName):"")
// 	   <<" _refCoefRangeName AK = "  << (_refCoefRangeName?_refCoefRangeName->GetName():"")  	   
// 	   << " && _refCoefNorm" << _refCoefNorm << " with size = _refCoefNorm.getSize() " << _refCoefNorm.getSize() << endl ;

      if (_refCoefRangeName && _refCoefNorm.getSize()>0) {
	RooArgSet* tmp = thePdf->getObservables(_refCoefNorm) ;
	rangeProj1 = thePdf->createIntegral(*tmp,*tmp,RooNameReg::str(_refCoefRangeName)) ;
	//rangeProj1->setOperMode(operMode()) ;
	delete tmp ;
      } else {
	TString theName(GetName()) ;
	theName.Append("_") ;
	theName.Append(thePdf->GetName()) ;
	theName.Append("_RangeNorm1") ;
	rangeProj1 = new RooRealVar(theName,"Unit range normalization integral",1.0) ;
      }
      cxcoutD(Caching) << " RooAddPdf::syncCoefProjList(" << GetName() << ") R1 = " << rangeProj1->GetName() << endl ;
      cache->_refRangeProjList.addOwned(*rangeProj1) ;
      

      // Calculate range adjusted projection integral
      RooAbsReal* rangeProj2 ;
      cxcoutD(Caching) << "RooAddPdf::syncCoefProjList(" << GetName() << ") rangename = " << (rangeName?rangeName:"<null>") 
		       << " nset = " << (nset?*nset:RooArgSet()) << endl ;
      if (rangeName && _refCoefNorm.getSize()>0) {

	rangeProj2 = thePdf->createIntegral(_refCoefNorm,_refCoefNorm,rangeName) ;
	//rangeProj2->setOperMode(operMode()) ;

      } else if (_normRange.Length()>0) {

	RooArgSet* tmp = thePdf->getObservables(_refCoefNorm) ;
	rangeProj2 = thePdf->createIntegral(*tmp,*tmp,_normRange.Data()) ;
	delete tmp ;

      } else {

	TString theName(GetName()) ;
	theName.Append("_") ;
	theName.Append(thePdf->GetName()) ;
	theName.Append("_RangeNorm2") ;
	rangeProj2 = new RooRealVar(theName,"Unit range normalization integral",1.0) ;

      }
      cxcoutD(Caching) << " RooAddPdf::syncCoefProjList(" << GetName() << ") R2 = " << rangeProj2->GetName() << endl ;
      cache->_rangeProjList.addOwned(*rangeProj2) ;

    }               

  }

  delete nset2 ;

  _projCacheMgr.setObj(nset,iset,cache,RooNameReg::ptr(rangeName)) ;

  return cache ;
}


//_____________________________________________________________________________
void RooAddPdf::updateCoefficients(CacheElem& cache, const RooArgSet* nset) const 
{
  // Update the coefficient values in the given cache element: calculate new remainder
  // fraction, normalize fractions obtained from extended ML terms to unity and
  // multiply these the various range and dimensional corrections needed in the
  // current use context

  // cxcoutD(ChangeTracking) << "RooAddPdf::updateCoefficients(" << GetName() << ") update coefficients" << endl ;
  
  Int_t i ;

  // Straight coefficients
  if (_allExtendable) {
    
    // coef[i] = expectedEvents[i] / SUM(expectedEvents)
    Double_t coefSum(0) ;
    for (i=0 ; i<_pdfList.getSize() ; i++) {
      _coefCache[i] = ((RooAbsPdf*)_pdfList.at(i))->expectedEvents(_refCoefNorm.getSize()>0?&_refCoefNorm:nset) ;
      coefSum += _coefCache[i] ;
    }
    if (coefSum==0.) {
      coutW(Eval) << "RooAddPdf::updateCoefCache(" << GetName() << ") WARNING: total number of expected events is 0" << endl ;
    } else {
      for (i=0 ; i<_pdfList.getSize() ; i++) {
	_coefCache[i] /= coefSum ;
      }			            
    }
    
  } else {
    if (_haveLastCoef) {
      
      // coef[i] = coef[i] / SUM(coef)
      Double_t coefSum(0) ;
      for (i=0 ; i<_coefList.getSize() ; i++) {
	_coefCache[i] = ((RooAbsPdf*)_coefList.at(i))->getVal(nset) ;
	coefSum += _coefCache[i] ;
      }		
      if (coefSum==0.) {
	coutW(Eval) << "RooAddPdf::updateCoefCache(" << GetName() << ") WARNING: sum of coefficients is zero 0" << endl ;
      } else {	
	for (i=0 ; i<_coefList.getSize() ; i++) {
	  _coefCache[i] /= coefSum ;
	}			
      }
    } else {
      
      // coef[i] = coef[i] ; coef[n] = 1-SUM(coef[0...n-1])
      Double_t lastCoef(1) ;
      for (i=0 ; i<_coefList.getSize() ; i++) {
	_coefCache[i] = ((RooAbsPdf*)_coefList.at(i))->getVal(nset) ;
 	cxcoutD(Caching) << "SYNC: orig coef[" << i << "] = " << _coefCache[i] << endl ;
	lastCoef -= _coefCache[i] ;
      }			
      _coefCache[_coefList.getSize()] = lastCoef ;
      cxcoutD(Caching) << "SYNC: orig coef[" << _coefList.getSize() << "] = " << _coefCache[_coefList.getSize()] << endl ;
      
      
      // Warn about coefficient degeneration
      if ((lastCoef<-1e-05 || (lastCoef-1)>1e-5) && _coefErrCount-->0) {
	coutW(Eval) << "RooAddPdf::updateCoefCache(" << GetName() 
		    << " WARNING: sum of PDF coefficients not in range [0-1], value=" 
		    << 1-lastCoef ; 
	if (_coefErrCount==0) {
	  coutW(Eval) << " (no more will be printed)"  ;
	}
	coutW(Eval) << endl ;
      } 
    }
  }

  

//    cout << "XXXX" << GetName() << "updateCoefs _projectCoefs = " << (_projectCoefs?"T":"F") << " cache._projList.getSize()= " << cache._projList.getSize() << endl ;

  // Stop here if not projection is required or needed
  if ((!_projectCoefs && _normRange.Length()==0) || cache._projList.getSize()==0) {
    //if (cache._projList.getSize()==0) {
//     cout << GetName() << " SYNC no projection required rangeName = " << (_normRange.Length()>0?_normRange.Data():"<none>") << endl ;
    return ;
  }

//    cout << "XXXX" << GetName() << " updateCoefs, applying correction" << endl ;
//    cout << "PROJLIST = " << endl ;
//    cache._projList.Print("v") ;
   
   
  // Adjust coefficients for given projection
  Double_t coefSum(0) ;
  for (i=0 ; i<_pdfList.getSize() ; i++) {
    Bool_t _tmp = _globalSelectComp ;
    RooAbsPdf::globalSelectComp(kTRUE) ;    

    RooAbsReal* pp = ((RooAbsReal*)cache._projList.at(i)) ; 
    RooAbsReal* sn = ((RooAbsReal*)cache._suppProjList.at(i)) ; 
    RooAbsReal* r1 = ((RooAbsReal*)cache._refRangeProjList.at(i)) ;
    RooAbsReal* r2 = ((RooAbsReal*)cache._rangeProjList.at(i)) ;

    Double_t proj = pp->getVal()/sn->getVal()*(r2->getVal()/r1->getVal()) ;  
    
    cxcoutD(Caching) << "ALEX:    RooAddPdf::updateCoef(" << GetName() << ") with nset = " << (nset?*nset:RooArgSet()) << "for pdf component #" << i << " = " << _pdfList.at(i)->GetName() << endl
	 << "ALEX:   pp = " << pp->GetName() << " = " << pp->getVal() << endl 
	 << "ALEX:   sn = " << sn->GetName() << " = " << sn->getVal() <<  endl 
	 << "ALEX:   r1 = " << r1->GetName() << " = " << r1->getVal() <<  endl 
	 << "ALEX:   r2 = " << r2->GetName() << " = " << r2->getVal() <<  endl 
	 << "ALEX: proj = (" << pp->getVal() << "/" << sn->getVal() << ")*(" << r2->getVal() << "/" << r1->getVal() << ") = " << proj << endl ;
    
    
    
    RooAbsPdf::globalSelectComp(_tmp) ;

    _coefCache[i] *= proj ;
    coefSum += _coefCache[i] ;
  }

  for (i=0 ; i<_pdfList.getSize() ; i++) {
    _coefCache[i] /= coefSum ;
//    _coefCache[i] *= rfrac ;
//       cout << "POST-SYNC coef[" << i << "] = " << _coefCache[i] << endl ;
     cxcoutD(Caching) << " ALEX:   POST-SYNC coef[" << i << "] = " << _coefCache[i]  
 	 << " ( _coefCache[i]/coefSum = " << _coefCache[i]*coefSum << "/" << coefSum << " ) "<< endl ;
  }
   

  
}



//_____________________________________________________________________________
Double_t RooAddPdf::evaluate() const 
{
  // Calculate and return the current value

  const RooArgSet* nset = _normSet ; 
  cxcoutD(Caching) << "RooAddPdf::evaluate(" << GetName() << ") calling getProjCache with nset = " << nset << " = " << (nset?*nset:RooArgSet()) << endl ;

  if (nset==0 || nset->getSize()==0) {
    if (_refCoefNorm.getSize()!=0) {
      //cout << "RooAddPdf::eval(" << GetName() << ") WARNING NSET IS NULL, but have reference normalization, using that" << endl ;
      nset = &_refCoefNorm ;
    }
  }

  CacheElem* cache = getProjCache(nset) ;
  updateCoefficients(*cache,nset) ;
  
  // Do running sum of coef/pdf pairs, calculate lastCoef.
  _pdfIter->Reset() ;
  _coefIter->Reset() ;
  RooAbsPdf* pdf ;

  Double_t snormVal ;
  Double_t value(0) ;
  Int_t i(0) ;
  while((pdf = (RooAbsPdf*)_pdfIter->Next())) {
    if (_coefCache[i]!=0.) {
      snormVal = nset ? ((RooAbsReal*)cache->_suppNormList.at(i))->getVal() : 1.0 ;
      Double_t pdfVal = pdf->getVal(nset) ;
      // Double_t pdfNorm = pdf->getNorm(nset) ;
      if (pdf->isSelectedComp()) {
	value += pdfVal*_coefCache[i]/snormVal ;
// 	cout << "RooAddPdf::EVALUATE(" << GetName() << ") adding pdf " << pdf->GetName() << " value = " << pdfVal << " coef = " << _coefCache[i] << endl ;
      }
    }
    i++ ;
  }

  return value ;
}


//_____________________________________________________________________________
void RooAddPdf::resetErrorCounters(Int_t resetValue)
{
  // Reset error counter to given value, limiting the number
  // of future error messages for this pdf to 'resetValue'

  RooAbsPdf::resetErrorCounters(resetValue) ;
  _coefErrCount = resetValue ;
}



//_____________________________________________________________________________
Bool_t RooAddPdf::checkObservables(const RooArgSet* nset) const 
{
  // Check if PDF is valid for given normalization set.
  // Coeffient and PDF must be non-overlapping, but pdf-coefficient 
  // pairs may overlap each other

  Bool_t ret(kFALSE) ;

  _pdfIter->Reset() ;
  _coefIter->Reset() ;
  RooAbsReal* coef ;
  RooAbsReal* pdf ;
  while((coef=(RooAbsReal*)_coefIter->Next())) {
    pdf = (RooAbsReal*)_pdfIter->Next() ;
    if (pdf->observableOverlaps(nset,*coef)) {
      coutE(InputArguments) << "RooAddPdf::checkObservables(" << GetName() << "): ERROR: coefficient " << coef->GetName() 
			    << " and PDF " << pdf->GetName() << " have one or more dependents in common" << endl ;
      ret = kTRUE ;
    }
  }
  
  return ret ;
}


//_____________________________________________________________________________
Int_t RooAddPdf::getAnalyticalIntegralWN(RooArgSet& allVars, RooArgSet& analVars, 
					 const RooArgSet* normSet, const char* rangeName) const 
{
  // Determine which part (if any) of given integral can be performed analytically.
  // If any analytical integration is possible, return integration scenario code
  //
  // RooAddPdf queries each component PDF for its analytical integration capability of the requested
  // set ('allVars'). It finds the largest common set of variables that can be integrated
  // by all components. If such a set exists, it reconfirms that each component is capable of
  // analytically integrating the common set, and combines the components individual integration
  // codes into a single integration code valid for RooAddPdf.


  RooArgSet* allDepVars = getObservables(allVars) ;
  RooArgSet allAnalVars(*allDepVars) ;
  delete allDepVars ;

  TIterator* avIter = allVars.createIterator() ;

  Int_t n(0) ;

  // First iteration, determine what each component can integrate analytically
  _pdfIter->Reset() ;
  RooAbsPdf* pdf ;
  while((pdf=(RooAbsPdf*)_pdfIter->Next())) {
    RooArgSet subAnalVars ;
    pdf->getAnalyticalIntegralWN(allVars,subAnalVars,normSet,rangeName) ;

    // Observables that cannot be integrated analytically by this component are dropped from the common list
    avIter->Reset() ;
    RooAbsArg* arg ;
    while((arg=(RooAbsArg*)avIter->Next())) {
      if (!subAnalVars.find(arg->GetName()) && pdf->dependsOn(*arg)) {
	allAnalVars.remove(*arg,kTRUE,kTRUE) ;
      }	
    }
    n++ ;
  }

  // If no observables can be integrated analytically, return code 0 here
  if (allAnalVars.getSize()==0) {
    delete avIter ;
    return 0 ;
  }


  // Now retrieve codes for integration over common set of analytically integrable observables for each component
  _pdfIter->Reset() ;
  n=0 ;
  Int_t* subCode = new Int_t[_pdfList.getSize()] ;
  Bool_t allOK(kTRUE) ;
  while((pdf=(RooAbsPdf*)_pdfIter->Next())) {
    RooArgSet subAnalVars ;
    RooArgSet* allAnalVars2 = pdf->getObservables(allAnalVars) ;
    subCode[n] = pdf->getAnalyticalIntegralWN(*allAnalVars2,subAnalVars,normSet,rangeName) ;
    if (subCode[n]==0 && allAnalVars2->getSize()>0) {
      coutE(InputArguments) << "RooAddPdf::getAnalyticalIntegral(" << GetName() << ") WARNING: component PDF " << pdf->GetName() 
			    << "   advertises inconsistent set of integrals (e.g. (X,Y) but not X or Y individually."
			    << "   Distributed analytical integration disabled. Please fix PDF" << endl ;
      allOK = kFALSE ;
    }
    delete allAnalVars2 ; 
    n++ ;
  }  
  if (!allOK) {
    delete[] subCode ;
    delete avIter ;
    return 0 ;
  }

  // Mare all analytically integrated observables as such
  analVars.add(allAnalVars) ;

  // Store set of variables analytically integrated
  RooArgSet* intSet = new RooArgSet(allAnalVars) ;
  Int_t masterCode = _codeReg.store(subCode,_pdfList.getSize(),intSet)+1 ;

  delete[] subCode ;
  delete avIter ;

  return masterCode ;
}



//_____________________________________________________________________________
Double_t RooAddPdf::analyticalIntegralWN(Int_t code, const RooArgSet* normSet, const char* rangeName) const 
{
  // Return analytical integral defined by given scenario code

  // WVE needs adaptation to handle new rangeName feature
  if (code==0) {
    return getVal(normSet) ;
  }

  // Retrieve analytical integration subCodes and set of observabels integrated over
  RooArgSet* intSet ;
  const Int_t* subCode = _codeReg.retrieve(code-1,intSet) ;
  if (!subCode) {
    coutE(InputArguments) << "RooAddPdf::analyticalIntegral(" << GetName() << "): ERROR unrecognized integration code, " << code << endl ;
    assert(0) ;    
  }

  cxcoutD(Caching) << "RooAddPdf::aiWN(" << GetName() << ") calling getProjCache with nset = " << (normSet?*normSet:RooArgSet()) << endl ;

  if ((normSet==0 || normSet->getSize()==0) && _refCoefNorm.getSize()>0) {
//     cout << "WVE integration of RooAddPdf without normalization, but have reference set, using ref set for normalization" << endl ;
    normSet = &_refCoefNorm ;
  }

  CacheElem* cache = getProjCache(normSet,intSet,0) ; // WVE rangename here?
  updateCoefficients(*cache,normSet) ;

  // Calculate the current value of this object  
  Double_t value(0) ;

  // Do running sum of coef/pdf pairs, calculate lastCoef.
  _pdfIter->Reset() ;
  _coefIter->Reset() ;
  RooAbsPdf* pdf ;
  Double_t snormVal ;
  Int_t i(0) ;

  //cout << "ROP::aIWN updateCoefCache with rangeName = " << (rangeName?rangeName:"<null>") << endl ;
  RooArgList* snormSet = (cache->_suppNormList.getSize()>0) ? &cache->_suppNormList : 0 ;
  while((pdf = (RooAbsPdf*)_pdfIter->Next())) {
    if (_coefCache[i]) {
      snormVal = snormSet ? ((RooAbsReal*) cache->_suppNormList.at(i))->getVal() : 1.0 ;
      
      // WVE swap this?
      Double_t val = pdf->analyticalIntegralWN(subCode[i],normSet,rangeName) ;
      if (pdf->isSelectedComp()) {
	
	value += val*_coefCache[i]/snormVal ;
      }
    }    
    i++ ;
  }

  return value ;
}



//_____________________________________________________________________________
Double_t RooAddPdf::expectedEvents(const RooArgSet* nset) const 
{  
  // Return the number of expected events, which is either the sum of all coefficients
  // or the sum of the components extended terms, multiplied with the fraction that
  // is in the current range w.r.t the reference range

  Double_t expectedTotal(0.0);

  cxcoutD(Caching) << "RooAddPdf::expectedEvents(" << GetName() << ") calling getProjCache with nset = " << (nset?*nset:RooArgSet()) << endl ;
  CacheElem* cache = getProjCache(nset) ;
  updateCoefficients(*cache,nset) ;

  Int_t i(0) ;
  for (i=0 ; i<_pdfList.getSize() ; i++) {

    Double_t proj(1) ;
    RooAbsReal* r1 = ((RooAbsReal*)cache->_refRangeProjList.at(i)) ;
    RooAbsReal* r2 = ((RooAbsReal*)cache->_rangeProjList.at(i)) ;    
    if (r1 && r2) {
      proj = (r2->getVal()/r1->getVal()) ;      
    }

    Double_t ncomp =  _allExtendable ? ((RooAbsPdf*)_pdfList.at(i))->expectedEvents(nset) :  ((RooAbsReal*)_coefList.at(i))->getVal(nset) ;
    expectedTotal += proj*ncomp ;
  }
  return expectedTotal ;  
}



//_____________________________________________________________________________
void RooAddPdf::selectNormalization(const RooArgSet* depSet, Bool_t force) 
{
  // Interface function used by test statistics to freeze choice of observables
  // for interpretation of fraction coefficients


  if (!force && _refCoefNorm.getSize()!=0) {
    return ;
  }

  if (!depSet) {
    fixCoefNormalization(RooArgSet()) ;
    return ;
  }

  RooArgSet* myDepSet = getObservables(depSet) ;
  fixCoefNormalization(*myDepSet) ;
  delete myDepSet ;
}



//_____________________________________________________________________________
void RooAddPdf::selectNormalizationRange(const char* rangeName, Bool_t force) 
{
  // Interface function used by test statistics to freeze choice of range
  // for interpretation of fraction coefficients

  if (!force && _refCoefRangeName) {
    return ;
  }

  fixCoefRange(rangeName) ;
}



//_____________________________________________________________________________
RooAbsGenContext* RooAddPdf::genContext(const RooArgSet &vars, const RooDataSet *prototype, 
					const RooArgSet* auxProto, Bool_t verbose) const 
{
  // Return specialized context to efficiently generate toy events from RooAddPdfs
  // return RooAbsPdf::genContext(vars,prototype,auxProto,verbose) ; // WVE DEBUG

  return new RooAddGenContext(*this,vars,prototype,auxProto,verbose) ;
}



//_____________________________________________________________________________
RooArgList RooAddPdf::CacheElem::containedArgs(Action) 
{
  // List all RooAbsArg derived contents in this cache element

  RooArgList allNodes;
  allNodes.add(_projList) ;
  allNodes.add(_suppProjList) ;
  allNodes.add(_refRangeProjList) ;
  allNodes.add(_rangeProjList) ;

  return allNodes ;
}



//_____________________________________________________________________________
std::list<Double_t>* RooAddPdf::plotSamplingHint(RooAbsRealLValue& obs, Double_t xlo, Double_t xhi) const 
{
  // Loop over components for plot sampling hints and merge them if there are multiple

  list<Double_t>* sumHint = 0 ;

  _pdfIter->Reset() ;
  RooAbsPdf* pdf ;
  // Loop over components pdf
  while((pdf=(RooAbsPdf*)_pdfIter->Next())) {

    list<Double_t>* pdfHint = pdf->plotSamplingHint(obs,xlo,xhi) ;

    // Process hint
    if (pdfHint) {
      if (!sumHint) {

	// If this is the first hint, then just save it
	sumHint = pdfHint ;

      } else {

	list<Double_t>* newSumHint = new list<Double_t>(sumHint->size()+pdfHint->size()) ;

	// Merge hints into temporary array
	merge(pdfHint->begin(),pdfHint->end(),sumHint->begin(),sumHint->end(),newSumHint->begin()) ;

	// Copy merged array without duplicates to new sumHintArrau
	delete sumHint ;
	sumHint = newSumHint ;
	
      }
    }
  }

  return sumHint ;
}


//_____________________________________________________________________________
void RooAddPdf::printMetaArgs(ostream& os) const 
{
  // Customized printing of arguments of a RooAddPdf to more intuitively reflect the contents of the
  // product operator construction

  _pdfIter->Reset() ;
  _coefIter->Reset() ;

  Bool_t first(kTRUE) ;
    
  RooAbsArg* coef, *pdf ;
  if (_coefList.getSize()!=0) { 
    while((coef=(RooAbsArg*)_coefIter->Next())) {
      if (!first) {
	os << " + " ;
      } else {
	first = kFALSE ;
      }
      pdf=(RooAbsArg*)_pdfIter->Next() ;
      os << coef->GetName() << " * " << pdf->GetName() ;
    }
    pdf = (RooAbsArg*) _pdfIter->Next() ;
    if (pdf) {
      os << " + [%] * " << pdf->GetName() ;
    }
  } else {
    
    while((pdf=(RooAbsArg*)_pdfIter->Next())) {
      if (!first) {
	os << " + " ;
      } else {
	first = kFALSE ;
      }
      os << pdf->GetName() ; 
    }  
  }

  os << " " ;    
}
