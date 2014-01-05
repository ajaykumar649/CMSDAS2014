// -*- mode: C++ -*-

#ifndef RooWjjMjjFitter_h
#define RooWjjMjjFitter_h

#include "RooAbsData.h"


#include "TString.h"
#include "RooWorkspace.h"
#include "RooHist.h"
#include "RooAbsPdf.h"
#include "RooRealVar.h"
#include "RooHistPdf.h"
#include "RooAddPdf.h"


#include "RooWorkspace.h"

class RooFitResult;

class RooWjjMjjFitter {
public:

  RooWjjMjjFitter();
  virtual ~RooWjjMjjFitter() { }

  RooFitResult * fit();
  RooPlot * computeChi2(int& ndf);

  RooAbsPdf * makeFitter();
  RooAbsData * loadData(bool trunc = false);

  RooAbsPdf * makeWWPdf();
  RooAbsPdf * makeWpJPdf();
  RooAbsPdf * makeMixedPdf();

  RooWorkspace& getWorkSpace() { return WorkSpace_; }

  RooRealVar * getVar() { return WorkSpace_.var("Mass2j_PFCor"); }

  void setFitPdfYieldsAndErrors();

  static void activateBranches(TTree & t, bool isElectron = false);
  TString fullCuts(bool trunc = false) const;

  TH1 * CreateHistogram(TString fname, TString histName, bool isElectron, TString CutString, bool implementEfficiencyCorrections=true) const;
  RooAbsPdf * ConvertHistogram2Pdf(TH1 * hist, TString pdfName, RooWorkspace& ws) const;

protected:


private:

  RooAbsReal * massVar_;
  RooWorkspace WorkSpace_;
  int NBins_;
  double * binEdgeArray;
  int NJets_;

  double minMass_;
  double maxMass_;
  double minTrunc_;
  double maxTrunc_;

  double initWW_;
  double initWjets_;
  double initMixed_;
  RooAbsData::ErrorType errorType_;

  TString rangeString_;

  static double sig2(RooAddPdf& pdf, RooRealVar& obs, double Nbin);
  static double sig2(RooHistPdf& pdf, RooRealVar& obs, double Nbin);

};

#endif
