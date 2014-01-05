#include "RooWjjMjjFitter.h"

#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TMath.h"
#include "TLine.h"
#include "TLegend.h"
#include "TRandom3.h"
#include "TEventList.h"
#include "TTreeFormula.h"

#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif

#include "RooWorkspace.h"
#include "RooDataSet.h"
#include "RooArgSet.h"
#include "RooArgList.h"
#include "RooFormulaVar.h"
#include "RooRealVar.h"
#include "RooAddPdf.h"
#include "RooFitResult.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooGaussian.h"
#include "RooConstVar.h"
#include "RooPlot.h"
#include "RooBinning.h"
#include "RooCurve.h"
#include "RooHist.h"
#include "RooProdPdf.h"
#include "RooRandom.h"
#include "RooAbsBinning.h"
#include "RooTreeDataStore.h"

#include "TPad.h"


//// Define Some Global Parameters
const TString StorageDirPrefix = "dcap://cmsdca.fnal.gov:24125/pnfs/fnal.gov/usr/cms/WAX/11/store/user/cmsdas/2013/JetsLongExerciseDijetMassSpectrumInAssociationWithW/";//At FNAL
const TString MCDir = StorageDirPrefix+"ReducedTrees/";
RooRealVar *mjj_ = new RooRealVar("Mass2j_PFCor","Mjj",60.0,300.0, "GeV");
const TString selectionCuts = "&&(W_muon_pt>25)&&(abs(W_muon_eta)<2.1)&&(event_met_pfmet>25)&&(JetPFCor_Pt[1]>30)&&(JetPFCor_Pt[2]<30)&&(abs(JetPFCor_Eta[0])<2.4)&&(abs(JetPFCor_Eta[1])<2.4)&&(abs(JetPFCor_Eta[0]-JetPFCor_Eta[1])<1.2)&&(JetPFCor_Pt[0]>40)&&(JetPFCor_Pt[1]/Mass2j_PFCor>0.3)&&(JetPFCor_Pt[1]/Mass2j_PFCor<0.7)&&(sqrt(JetPFCor_Pt[0]**2+JetPFCor_Pt[1]**2+2*JetPFCor_Pt[0]*JetPFCor_Pt[1]*cos(JetPFCor_Phi[0]-JetPFCor_Phi[1]))>45.)&&(abs(JetPFCor_dphiMET[0])>0.4)&&(W_mt>50.)&&(Mass2j_PFCor<300)&&(Mass2j_PFCor>60)";
const bool ImplementReweightingCorr=false;
double intLumi = 19300.0;
static const unsigned int maxJets = 6;


using namespace RooFit;


RooWjjMjjFitter::RooWjjMjjFitter() :
  WorkSpace_("WorkSpace","WorkSpace"),
  initWW_(0.), initWjets_(0.), initMixed_(0.), errorType_(RooAbsData::SumW2)
{
  massVar_ = new RooFormulaVar("mass", "@0", RooArgList( *mjj_));
  WorkSpace_.import(*massVar_, RooFit::RecycleConflictNodes(), RooFit::Silence());
  RooRealVar * mass = WorkSpace_.var("Mass2j_PFCor");

  ///Define the binning
  NBins_=17;
  binEdgeArray = new double[NBins_+1];
  binEdgeArray[0]=60.0;
  binEdgeArray[1]=66.0;
  binEdgeArray[2]=73.0;
  binEdgeArray[3]=80.0;
  binEdgeArray[4]=88.0;
  binEdgeArray[5]=97.0;
  binEdgeArray[6]=107.0;
  binEdgeArray[7]=118.0;
  binEdgeArray[8]=130.0;
  binEdgeArray[9]=143.0;
  binEdgeArray[10]=157.0;
  binEdgeArray[11]=173.0;
  binEdgeArray[12]=190.0;
  binEdgeArray[13]=209.0;
  binEdgeArray[14]=230.0;
  binEdgeArray[15]=253.0;
  binEdgeArray[16]=278.0;
  binEdgeArray[17]=300.0;

  ///Define the range of the fit and region to be excluded from it (due to the potential presence of the signal)
  minMass_=60.0;
  maxMass_=300.0;
  minTrunc_=130.0;
  maxTrunc_=173.0;	     
  mass->setRange("lowSideBand", minMass_, minTrunc_);
  mass->setRange("highSideBand", maxTrunc_, maxMass_);
  rangeString_ = "lowSideBand,highSideBand";

  ///Define the number of jets per event
  NJets_=2;
}

// ***** Function to make the total and individual Pdfs ***** //
RooAbsPdf * RooWjjMjjFitter::makeFitter() {
  if (WorkSpace_.pdf("totalPdf"))
    return WorkSpace_.pdf("totalPdf");

  ///Define The individual PDFs, their ranges and normalizations. Note that the normalizations and errors in the fit are set by the function setFitPdfYieldsAndErrors()
  RooAbsPdf * mixedPdf = makeMixedPdf();
//   /// begin: to be uncommented during the "Fit" Exercise
   RooAbsPdf * wwPdf = makeWWPdf();
   RooAbsPdf * WpJPdf =  makeWpJPdf();
//   /// end: to be uncommented during the "Fit" Exercise

  ///Initialize the yield values and the corresponding errors. The starting values for the fit are later set by setFitPdfYieldsAndErrors().
  RooRealVar nMixed("nMixed","", initMixed_, 0.0, 100000.);
  nMixed.setError(initMixed_*0.15);
//   /// begin: to be uncommented during the "Fit" Exercise
   RooRealVar nWW("nWW","nWW",  initWW_, 0.0, 10000.);
   nWW.setError(initWW_*0.15);
   RooRealVar nWjets("nWjets","nWjets", initWjets_, 0.0, 1000000.);
   nWjets.setError(TMath::Sqrt(initWjets_));
//   /// end: to be uncommented during the "Fit" Exercise


  RooArgList components(*wwPdf);
  RooArgList yields(nWW);
  components.add(RooArgList(*mixedPdf, *WpJPdf));
  yields.add(RooArgList(nMixed, nWjets));
  RooAddPdf totalPdf("totalPdf","extended sum pdf", components, yields);
  WorkSpace_.import(totalPdf);

  return WorkSpace_.pdf("totalPdf");

}

// ***** Function to run the fit ***** //
RooFitResult * RooWjjMjjFitter::fit() {
  /// Make the fitter and load the data
  RooAbsPdf * totalPdf = makeFitter();
  RooAbsData * data = loadData();
  cout << "Made dataset" << endl;

  RooFitResult *fitResult = 0;
  
  /// Set the starting values of the yields and constraints on the errors
  setFitPdfYieldsAndErrors();

  /// Load the constraints
  RooArgSet * params = totalPdf->getParameters(data);

  RooRealVar * nWW = WorkSpace_.var("nWW");
  RooGaussian constWW("constWW", "constWW", *nWW, RooConst(nWW->getVal()), RooConst(nWW->getError()));
  RooRealVar * nWjets = WorkSpace_.var("nWjets");
  RooGaussian constWpJ("constWpJ", "constWpJ", *nWjets, RooConst(nWjets->getVal()), RooConst(nWjets->getError()));
  RooRealVar * nMixed = WorkSpace_.var("nMixed");
  RooGaussian constMixed("constMixed","constMixed", *nMixed, RooConst(nMixed->getVal()), RooConst(nMixed->getError())) ;
  
  RooArgList ConstrainedVars;
  RooArgList Constraints;
  
  Constraints.add(constWW);
  ConstrainedVars.add(*nWW);
  Constraints.add(constMixed);
  ConstrainedVars.add(*nMixed);
  Constraints.add(constWpJ);
  ConstrainedVars.add(*nWjets);
  
  WorkSpace_.Print();
  std::cout << "\n***constraints***\n";
  TIter con(Constraints.createIterator());
  RooGaussian * tc;
  while ((tc = (RooGaussian *)con()))
    tc->Print();
  std::cout << "*** ***\n\n";
  
  RooAbsPdf * fitPdf = totalPdf;

  /// Perform the fit
  fitResult = fitPdf->fitTo(*data, Save(true), 
			    ( true ? ExternalConstraints(Constraints) : Constrained() ),
			    RooFit::Extended(true), 
			    RooFit::Minos(false), 
			    RooFit::Hesse(true),
			    PrintEvalErrors(-1),
			    RooFit::Range(rangeString_),
			    Warnings(false) 
			    );
  
  fitResult->Print("v");
  
  delete params;
  return fitResult;
}


void RooWjjMjjFitter::setFitPdfYieldsAndErrors() {
  WorkSpace_.var("nWW")->setVal(initWW_);
  WorkSpace_.var("nMixed")->setVal(initMixed_);
  WorkSpace_.var("nWjets")->setVal(initWjets_);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////********   Functions to convert Data & MC files to histograms and PDFs.   ********/////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ***** Function to store data in WorkSpace_ **** //
RooAbsData * RooWjjMjjFitter::loadData(bool trunc) {

  RooRealVar * mass = WorkSpace_.var("Mass2j_PFCor");
  TString dataName("data");
  RooDataSet data(dataName, dataName, RooArgSet(*mass));

  TFile * treeFile = TFile::Open(MCDir+"/RD_WmunuJets_DataAll_GoldenJSON_19p3invfb.root");
  TTree * theTree;
  treeFile->GetObject("WJet", theTree);
  TTree * reducedTree = theTree;

  TFile holder("holder_DELETE_ME.root", "recreate");
  activateBranches(*theTree,false);
  reducedTree = theTree->CopyTree( fullCuts(trunc) );
  delete theTree;

  RooDataSet * mds = new RooDataSet("data_muon","data_muon", reducedTree, RooArgSet(*mjj_));

  delete reducedTree;
  delete treeFile;

  //RooDataSet * mds = ds;

  mds->Print();
  data.append(*mds);
  delete mds;

  WorkSpace_.import(data);

  return WorkSpace_.data(dataName);
}

// ***** Function to return the Mixed Pdf **** //
RooAbsPdf * RooWjjMjjFitter::makeMixedPdf() {

  if (WorkSpace_.pdf("mixedPdf"))
    return WorkSpace_.pdf("mixedPdf");

  ///Scaling factors for each component histogram: CrossSection/NGenerated
  double TTbarWeight = 225.197/6893735;
  double STopT_TWeight = 55.531/3758221;
  double STopT_TbarWeight = 30.0042/1935066;
  double STopS_TWeight = 3.89394/259960;
  double STopS_TbarWeight = 1.75776/139974;
  double STopTW_TWeight = 11.1773/497657;
  double STopTW_TbarWeight = 11.1773/493458;
  double ZpJWeight = 3503.71/30209426;
 
  TH1D * th1mixed = new TH1D("th1mixed","th1mixed",NBins_, binEdgeArray);
  th1mixed->Sumw2();

  ///Add all of the mixed modes
  TH1 * tmpHist;
  tmpHist = CreateHistogram(MCDir + "RD_mu_TTbar_CMSSW532.root", "hist_ttbar_mu",false,fullCuts(),ImplementReweightingCorr);
  th1mixed->Add(tmpHist, TTbarWeight);
  delete tmpHist;
  tmpHist = CreateHistogram(MCDir + "RD_mu_STopT_Tbar_CMSSW532.root", "hist_stbar_mu",false,fullCuts(),ImplementReweightingCorr);
  th1mixed->Add(tmpHist, STopT_TbarWeight);
  delete tmpHist;
  tmpHist = CreateHistogram(MCDir + "RD_mu_STopT_T_CMSSW532.root", "hist_st_mu",false,fullCuts(),ImplementReweightingCorr);
  th1mixed->Add(tmpHist, STopT_TWeight);
  delete tmpHist;
  tmpHist = CreateHistogram(MCDir + "RD_mu_STopS_Tbar_CMSSW532.root", "hist_stbar_mu",false,fullCuts(),ImplementReweightingCorr);
  th1mixed->Add(tmpHist, STopS_TbarWeight);
  delete tmpHist;
  tmpHist = CreateHistogram(MCDir + "RD_mu_STopS_T_CMSSW532.root", "hist_st_mu",false,fullCuts(),ImplementReweightingCorr);
  th1mixed->Add(tmpHist, STopS_TWeight);
  delete tmpHist;
  tmpHist = CreateHistogram(MCDir + "RD_mu_STopTW_Tbar_CMSSW532.root", "hist_stbar_mu",false,fullCuts(),ImplementReweightingCorr);
  th1mixed->Add(tmpHist, STopTW_TbarWeight);
  delete tmpHist;
  tmpHist = CreateHistogram(MCDir + "RD_mu_STopTW_T_CMSSW532.root", "hist_st_mu",false,fullCuts(),ImplementReweightingCorr);
  th1mixed->Add(tmpHist, STopTW_TWeight);
  delete tmpHist;
  tmpHist = CreateHistogram(MCDir + "RD_mu_ZpJ_CMSSW532.root", "hist_zpj_mu",false,fullCuts(),ImplementReweightingCorr);
  th1mixed->Add(tmpHist, ZpJWeight);
  delete tmpHist;


  initMixed_ = th1mixed->Integral() * intLumi;

  cout << "-------- Number of expected mixed events = " << 
    th1mixed->Integral() << " x " << intLumi << " = " << initMixed_ << endl;

  th1mixed->Scale(1., "width");
  RooAbsPdf * mixedPdf = ConvertHistogram2Pdf(th1mixed, "mixedPdf", WorkSpace_);
  delete th1mixed;
  return mixedPdf;

}

// ***** Function to return the WW Pdf **** //
RooAbsPdf * RooWjjMjjFitter::makeWWPdf() {
  if (WorkSpace_.pdf("wwPdf"))
    return WorkSpace_.pdf("wwPdf");

  TH1D * th1ww = new TH1D("th1ww","th1ww",NBins_, binEdgeArray);
  th1ww->Sumw2();

  TH1 * tmpHist;
  tmpHist=CreateHistogram(MCDir+"RD_mu_WW_CMSSW532.root","hist_ww_mu",false, fullCuts() ,ImplementReweightingCorr);
  th1ww->Add(tmpHist);
  delete tmpHist;

  initWW_ = (57.1097/9450414) * th1ww->Integral() * intLumi;

  cout << "-------- Number of expected WW events = " << " = " <<  initWW_ << endl;

  th1ww->Scale(1., "width");
  RooAbsPdf * wwPdf = ConvertHistogram2Pdf(th1ww, "wwPdf", WorkSpace_);
  delete th1ww;
  return wwPdf;
}

// ***** Function to return the W+jets Pdf **** //
RooAbsPdf * RooWjjMjjFitter::makeWpJPdf() {  
  if (WorkSpace_.pdf("WpJPdf"))
    return WorkSpace_.pdf("WpJPdf");
  
  TH1D * th1WpJ = new TH1D("th1WpJ","th1WpJ",NBins_, binEdgeArray);
  th1WpJ->Sumw2();
  TH1 * tmpHist;

  tmpHist = CreateHistogram(MCDir + "RD_mu_WpJ_CMSSW532.root","hist_wpj_mu",false,fullCuts(),ImplementReweightingCorr);
  th1WpJ->Add(tmpHist);
  delete tmpHist;

  initWjets_ = (36257.2/69122011) * th1WpJ->Integral() * intLumi;
  cout << "-------- Number of expected Wjets events = " << initWjets_ << endl;

  th1WpJ->Scale(1., "width");

  RooAbsPdf * WpJPdf = ConvertHistogram2Pdf(th1WpJ, "WpJPdf", WorkSpace_);
  delete th1WpJ;
  return WpJPdf;
}


// **** Utility functions for histogram and pdf construction **** //
TH1 * RooWjjMjjFitter::CreateHistogram(TString fname, TString histName, bool isElectron, TString cutString, bool implementReweightingCorrections) const {

  TFile * treeFile = TFile::Open(fname);
  TTree * theTree;
  treeFile->GetObject("WJet", theTree);
  if (!theTree) {
    std::cout << "failed to find WJet tree in file " << fname 
	      << '\n';
    return 0;
  }

  ///Create an empty histogram
  TH1D * theHist = new TH1D(histName,histName,NBins_, binEdgeArray);
  theHist->Sumw2();

  ///Create and fill the corresponding event list
  theTree->Draw(">>" + histName + "_evtList",cutString, "goff");
  TEventList * list = (TEventList *)gDirectory->Get(histName + "_evtList");

  ///Activate the (potentially) needed branches
  activateBranches(*theTree, isElectron);
  Float_t         Mass2j_PFCor;
  Float_t         effwt;


  TTreeFormula poi("poi", "Mass2j_PFCor", theTree);
  theTree->SetBranchAddress("Mass2j_PFCor", &Mass2j_PFCor);


  ///Set the branch addresses, when needed to perform efficiency corrections
  if (implementReweightingCorrections) {
    theTree->SetBranchAddress("effwt", &effwt);
  }


  double evtWgt = 1.0;
  for (int event = 0; event < list->GetN(); ++event) {
    theTree->GetEntry(list->GetEntry(event));
    evtWgt = 1.0;
    if (implementReweightingCorrections) {
      //evtWgt = effWeight(lepton_pt, lepton_eta, W_mt, JetPFCor_Pt,JetPFCor_Eta,NJets_, event_met_pfmet,isElectron);
      evtWgt = effwt;
    }
    theHist->Fill(poi.EvalInstance(), evtWgt);
  }


  delete theTree;

  theHist->SetDirectory(0);

  delete treeFile;
  return theHist;
}


RooAbsPdf * RooWjjMjjFitter::ConvertHistogram2Pdf(TH1 * hist, TString pdfName, RooWorkspace& WorkSpace) const {
  if (WorkSpace.pdf(pdfName))
    return WorkSpace.pdf(pdfName);

  RooDataHist newHist(pdfName + "_hist", pdfName + "_hist", RooArgList(*mjj_), hist);
  WorkSpace.import(newHist);

  RooHistPdf thePdf = RooHistPdf(pdfName, pdfName, RooArgSet(*massVar_),RooArgSet(*mjj_), newHist);
  //thePdf.Print();
  WorkSpace.import(thePdf, RooFit::RecycleConflictNodes(), RooFit::Silence());

  return WorkSpace.pdf(pdfName);
}

TString RooWjjMjjFitter::fullCuts(bool trunc) const {

  /// Add the range cuts and exclude the signal region when trunc is set to true
  TString theCut = TString::Format("((%s > %0.3f) && (%s < %0.3f))", 
				    "Mass2j_PFCor", minMass_, 
				    "Mass2j_PFCor", maxMass_);
  if (trunc) {
    theCut = TString::Format("(((%s>%0.3f) && (%s<%0.3f)) || "
			     "((%s>%0.3f) && (%s<%0.3f)))",
			     "Mass2j_PFCor", minMass_,
			     "Mass2j_PFCor", minTrunc_,
			     "Mass2j_PFCor", maxTrunc_,
			     "Mass2j_PFCor", maxMass_);
  }

  /// Add the predefined selection cuts
  theCut = theCut + selectionCuts;

  return "(" + theCut + ")";
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////********   Functions to make plots   ********//////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
   RooPlot * RooWjjMjjFitter::stackedPlot(bool logy) {
   RooRealVar * mass = WorkSpace_.var("Mass2j_PFCor");
   mass->setRange("RangeForPlot", minMass_, 
 				 maxMass_);
   RooAbsBinning& plotBins =  mass->getBinning("plotBinning");
   mass->setBinning(plotBins);
    WorkSpace_.var(params_.var)->Print("v");
   RooPlot * sframe = mass->frame();
   sframe->SetName("mass_stacked");
   RooAbsData * data = WorkSpace_.data("data");

  TString plotHistName(TString::Format("dataHistPlot_%i", logy));
   TH1 * dataHist = utils_.newEmptyHist(plotHistName);
   TH1D * dataHist = new TH1D(plotHistName,plotHistName,NBins_,binEdgeArray);
   dataHist->Sumw2();

   RooTreeDataStore * dataStore = 
     dynamic_cast<RooTreeDataStore *>(data->store());
   dataStore->tree().Draw(TString::Format("%s>>%s", mass->GetName(),
 					 plotHistName.Data()),
 			 "", "goff");
   dataHist->SetTitle("data");

  
   dataHist->Draw();
    gPad->Update();
   gPad->WaitPrimitive();
   dataHist->Scale(1., "width");
   RooHist * h_data = new RooHist(*dataHist, 0., 1, errorType_, 1.0,
 				 false);
   RooHist * theData = new RooHist(*h_data);
    dataHist->Draw();
    gPad->Update();
   gPad->WaitPrimitive();
   delete dataHist;

   RooAddPdf * totalPdf = dynamic_cast<RooAddPdf *>(WorkSpace_.pdf("totalPdf"));
   RooArgList components(totalPdf->pdfList());

   components.Print("v");
   h_data->SetName("h_data");
   sframe->addPlotable(h_data, "pe", true, true);
   data->plotOn(sframe, RooFit::DataError(errorType_), 
 	       RooFit::Binning(plotBins),
 	       RooFit::Name("h_data"),
 	       RooFit::Invisible());

   int comp(1);
   double nexp(totalPdf->expectedEvents(RooArgSet(*(WorkSpace_.var("Mass2j_PFCor")))));
   std::cout << "totalPdf expected: " << nexp << '\n'
 	    << "frame NEvt: " << sframe->getFitRangeNEvt() << '\n'
 	    << "frame BinW: " << sframe->getFitRangeBinW() << '\n'
 	    << '\n';
   TH1 * pdfHist = totalPdf->createHistogram("pdfHist", *mass,
 					    Binning(plotBins),
 					    Scaling(false));

   pdfHist->Scale(nexp);
   pdfHist->SetLineColor(kRed);
   pdfHist->SetMarkerColor(kRed);
   pdfHist->Draw();
   dataHist->Draw("same");
   gPad->Update();
   gPad->WaitPrimitive();

   totalPdf->plotOn(sframe,ProjWData(*data), DrawOption("LF"), FillStyle(1001),
 		   FillColor(kOrange), LineColor(kOrange), Name("h_total"),
  		   NormRange("RangeForPlot"),
 		   Normalization(nexp, RooAbsReal::Raw),
 		   VLines(), Range("RangeForPlot"));
   RooCurve * tmpCurve = sframe->getCurve("h_total");
   tmpCurve->SetTitle("WW/WZ");
   components.remove(*(components.find("wwPdf")));
   if (params_.doNewPhysics) {
     totalPdf->plotOn(sframe, ProjWData(*data), DrawOption("LF"), 
 		     FillStyle(1001), Name("h_NP"), VLines(),
		     FillColor(kCyan+2), LineColor(kCyan+2), 
 		     Normalization(nexp, RooAbsReal::Raw),
 		     Components(RooArgSet(components)),
 		     Range("RangeForPlot"));
     components.remove(*(components.find("NPPdf")));
     tmpCurve = sframe->getCurve("h_NP");
     tmpCurve->SetTitle("new physics");
  }
   int linec(kRed);
  TString pdfName("h_background");
   RooAbsCollection * removals;
   while (components.getSize() > 0) {
     totalPdf->plotOn(sframe, ProjWData(*data), FillColor(linec), 
 		     Name(pdfName),
 		     DrawOption("LF"), Range("RangeForPlot"),
 		     NormRange("RangeForPlot"),
 		     Normalization(nexp, RooAbsReal::Raw),
 		     Components(RooArgSet(components)), VLines(),
 		     FillStyle(1001), LineColor(linec));
     tmpCurve = sframe->getCurve();
     tmpCurve->SetName(pdfName);
     removals = 0;
     switch (comp) {
     case 1: 
       removals = components.selectByName("WpJPdf*");
       linec = kBlack; 
       tmpCurve->SetTitle("W+jets");
       break;
     case 2: 
       linec = kGreen;
       removals = components.selectByName("ttPdf*,mixedPdf*");
       tmpCurve->SetTitle("top");
       break;
     case 3: 
       linec = kMagenta; 
       removals = components.selectByName("qcdPdf*");
       tmpCurve->SetTitle("QCD");
       break;
     default:
       linec = kCyan;
       removals = components.selectByName("WpJPdf*");
       tmpCurve->SetTitle("Z+jets");
     }
     if (removals) {
       components.remove(*removals);
       delete removals;
     }
     if (components.getSize() > 0) {
       pdfName = components[0].GetName();
       pdfName = "h_" + pdfName;
     }
     ++comp;
   }
   theData->SetName("theData");
   sframe->addPlotable(theData, "pe");
    data->plotOn(sframe, RooFit::DataError(errorType_), Name("theData"),
 	       RooFit::Binning(plotBins));
   RooHist * tmpHist = sframe->getHist("theData");
   tmpHist->SetTitle("data");
   sframe->addObject(dataHist, "samepe");
   TLegend * legend = RooWjjFitterUtils::legend4Plot(sframe);
   sframe->addObject(legend);
  if (params_.truncRange) {
   TLine * lowerLine = new TLine(minTrunc_, 0., minTrunc_, 
 				sframe->GetMaximum());
   lowerLine->SetLineWidth(3);
   lowerLine->SetLineColor(kBlue+2);
   lowerLine->SetLineStyle(kDashed);
   TLine * upperLine = new TLine(maxTrunc_, 0., maxTrunc_, 
 				sframe->GetMaximum());
   upperLine->SetLineWidth(3);
   upperLine->SetLineColor(kBlue+2);
   upperLine->SetLineStyle(kDashed);
   sframe->addObject(lowerLine);
   sframe->addObject(upperLine);
       }
  if (logy) {
    sframe->SetMinimum(0.1);
    sframe->SetMaximum(1.0e8);
   } else {
     sframe->SetMinimum(0.);
     sframe->SetMaximum(1.5*sframe->GetMaximum());
   }
   sframe->GetYaxis()->SetTitle("Events / GeV");
   return sframe;
 }

 RooPlot * RooWjjMjjFitter::residualPlot(RooPlot * thePlot, TString curveName,
 					TString pdfName, bool normalize) {
   RooPlot * rframe = thePlot->emptyClone("mass_residuals");
   RooAbsData * data = WorkSpace_.data("data");
   RooAddPdf * totalPdf = dynamic_cast<RooAddPdf *>(WorkSpace_.pdf("totalPdf"));
   RooCurve * tmpCurve;
    RooHist * theData = new RooHist(*(thePlot->getHist("theData")));
   if (pdfName.Length() > 0) {
     double nexp(totalPdf->expectedEvents(RooArgSet(*(WorkSpace_.var("Mass2j_PFCor")))));
     totalPdf->plotOn(rframe, ProjWData(*data), Components(pdfName),
 		     Normalization(nexp, RooAbsReal::Raw),
 		     DrawOption("LF"), VLines(), FillStyle(1001),
 		     FillColor(kOrange), Name("h_ww"), 
 		     LineColor(kOrange), Range("RangeForPlot"));
     tmpCurve = rframe->getCurve("h_ww");
     tmpCurve->SetTitle("WW/WZ");
    if (params_.doNewPhysics) {
       totalPdf->plotOn(rframe, ProjWData(*data), Components("NPPdf"),
 		       Normalization(nexp, RooAbsReal::Raw),
		       DrawOption("LF"), VLines(), FillStyle(1001),
 		       FillColor(kCyan+2), Name("h_NP"),
 		       LineColor(kCyan+2), Range("RangeForPlot"));
       tmpCurve = rframe->getCurve("h_NP");
       tmpCurve->SetTitle("new physics");
     }
   }
   RooHist * hresid = thePlot->residHist("theData", curveName, normalize);
   hresid->SetTitle("data");
   RooHist * bands = new RooHist(*hresid);
   bands->SetName("band");
   bands->SetTitle("error bands");
   bands->SetFillStyle(1001);
   rframe->addPlotable(bands, "e3");
   rframe->addPlotable(hresid, "p");
  
   TLegend * legend = RooWjjFitterUtils::legend4Plot(rframe);
   rframe->addObject(legend);

   if (!normalize) {
     rframe->SetMaximum(initWW_*(0.1));
     rframe->SetMinimum(initWW_*(-0.05));
     rframe->GetYaxis()->SetTitle("Events / GeV");
   } else {
     rframe->SetMaximum(5.);
     rframe->SetMinimum(-5.);
     rframe->GetYaxis()->SetTitle("pull ( #sigma )");
   }

     if (params_.truncRange) {
   TLine * lowerLine = new TLine(minTrunc_,rframe->GetMinimum()*0.6,
 				minTrunc_,rframe->GetMaximum()*0.6);
   lowerLine->SetLineWidth(3);
   lowerLine->SetLineColor(kBlue+2);
   lowerLine->SetLineStyle(kDashed);
   TLine * upperLine = new TLine(maxTrunc_,rframe->GetMinimum()*0.6, 
 				maxTrunc_,rframe->GetMaximum()*0.6);
   upperLine->SetLineWidth(3);
   upperLine->SetLineColor(kBlue+2);
   upperLine->SetLineStyle(kDashed);
   rframe->addObject(lowerLine);
   rframe->addObject(upperLine);
       }  

   return rframe;
 }


////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////********   General Utility and Efficiency Correction Functions   ********////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RooWjjMjjFitter::activateBranches(TTree& t, bool isElectron) {
  t.SetBranchStatus("*",    0);
  t.SetBranchStatus("JetPFCor_Pt",    1);
  t.SetBranchStatus("JetPFCor_Px",    1);
  t.SetBranchStatus("JetPFCor_Py",    1);
  t.SetBranchStatus("JetPFCor_Pz",    1);
  t.SetBranchStatus("JetPFCor_Eta",    1);
  t.SetBranchStatus("JetPFCor_Phi",    1);
  t.SetBranchStatus("JetPFCor_bDiscriminator",    1);
  t.SetBranchStatus("JetPFCor_QGLikelihood",    1);

  t.SetBranchStatus("event_met_pfmet",    1);
  t.SetBranchStatus("event_met_pfmetPhi",    1);
  t.SetBranchStatus("event_met_pfmetsignificance",    1);
  t.SetBranchStatus("JetPFCor_dphiMET",    1);


  if (isElectron) {
    t.SetBranchStatus("W_electron_pt", 1);
    t.SetBranchStatus("W_electron_eta", 1);
  } else {
    t.SetBranchStatus("W_muon_pt", 1);
    t.SetBranchStatus("W_muon_eta", 1);
  }

  t.SetBranchStatus("W_mt",    1);
  t.SetBranchStatus("W_pt",    1);
  t.SetBranchStatus("W_pzNu1",    1);
  t.SetBranchStatus("W_pzNu2",    1);

  t.SetBranchStatus("Mass2j_PFCor",    1);
  t.SetBranchStatus("MassV2j_PFCor",    1);

  t.SetBranchStatus("event_runNo",    1);
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////********   Functions to make the chi squared for the fit   ********///////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
RooPlot * RooWjjMjjFitter::computeChi2(int& ndf) {

  RooRealVar * mass = WorkSpace_.var("Mass2j_PFCor");
  RooAbsBinning& plotBins =  mass->getBinning("plotBinning");
  mass->setBinning(plotBins);
  RooAbsData * data = WorkSpace_.data("data");

  TH1D * dataHist = new TH1D("theData","theData",NBins_, binEdgeArray);
  dataHist->Sumw2();

  RooTreeDataStore * dataStore = dynamic_cast<RooTreeDataStore *>(data->store());
  dataStore->tree().Draw(TString::Format("%s>>%s", mass->GetName(),"theData"),"", "goff");

  RooHist h_data(*dataHist, 0., 1, errorType_, 1.0,false);
  RooAbsPdf * totalPdf = WorkSpace_.pdf("totalPdf");


  int chi2bins;
//   chi2 = RooWjjFitterUtils::computeChi2(h_data, *totalPdf, *mass, chi2bins);

// double RooWjjFitterUtils::computeChi2(RooHist& hist, RooAbsPdf& pdf, 
// 				      RooRealVar& obs, int& nbin, 
// 				      bool correct) {
  int np = h_data.GetN();
  chi2bins = 0;
  double chisqrd(0);

  double x,y,eyl,eyh,exl,exh;
  double avg, pdfSig2;
  double Npdf = totalPdf->expectedEvents(RooArgSet(*mass));
  TString className;
  RooAbsReal * binInt;
  RooAbsReal * fullInt = totalPdf->createIntegral(*mass, *mass);
  double sumN = 0., compN = 0., dataN = 0.;
  for (int i=0; i<np; ++i) {
    h_data.GetPoint(i,x,y);
    eyl = h_data.GetEYlow()[i];
    eyh = h_data.GetEYhigh()[i];
    exl = h_data.GetEXlow()[i];
    exh = h_data.GetEXhigh()[i];

    mass->setVal(x);
    mass->setRange("binRange", x-exl, x+exh);
    binInt = totalPdf->createIntegral(*mass, *mass, "binRange");
    avg = Npdf*binInt->getVal()/fullInt->getVal();
    sumN += avg;
    delete binInt;

    pdfSig2 = 0.;
    className = totalPdf->ClassName();

    //    cout << "className=" << className << endl;
    std::cout << TString::Format("bin [%0.2f, %0.2f]", x-exl, x+exh);// << '\n';
    if (className == "RooHistPdf") {
      RooHistPdf& tmpPdf = dynamic_cast<RooHistPdf&>(*totalPdf);
      pdfSig2 = sig2(tmpPdf, *mass, avg);
    } else if (className == "RooAddPdf") {
      RooAddPdf& tmpPdf = dynamic_cast<RooAddPdf&>(*totalPdf);
      pdfSig2 = sig2(tmpPdf, *mass, avg);
    }
    
//     std::cout << " y: " << y << " avg: " << avg 
// 	      << " eyl: " << eyl << " eyh: " << eyh 
// 	      << '\n';
    if (y != 0) {
      ++chi2bins;
      compN += avg;
      dataN += y;
      double pull2 = (y-avg)*(y-avg);
      pull2 = (y>avg) ? pull2/(eyl*eyl + pdfSig2) : pull2/(eyh*eyh + pdfSig2) ;
      chisqrd += pull2;
    }
  }
//   std::cout << "Npdf: " << Npdf << " sumN: " << sumN 
// 	    << " compN: " << compN << " dataN: " << dataN
// 	    << '\n';
  delete fullInt;
//   return chisqrd;
// }


  chi2bins -= ndf;
  std::cout << "\n *** chi^2/dof = " << chisqrd << "/" << chi2bins << " = " 
	    << chisqrd/chi2bins << " ***\n"
	    << " *** chi^2 probability = " << TMath::Prob(chisqrd, chi2bins)
	    << " ***\n\n";

  delete dataHist;
  ndf = chi2bins;
  return 0;
}


double RooWjjMjjFitter::sig2(RooAddPdf& pdf, RooRealVar& obs, double Nbin) {

  double retVal = 0.;
  TString className;
  double iN = 0, sumf = 0.;
  bool allCoefs(false);
  bool fCoefs(true);
  if (pdf.coefList().getSize() == pdf.pdfList().getSize())
    allCoefs = true;
  if (pdf.coefList().getSize()+1 == pdf.pdfList().getSize())
    fCoefs = true;
  RooAbsReal * coef = 0;
  RooAbsPdf * ipdf = 0;
//   std::cout << "pdf: " << pdf.GetName() 
// 	    << " N for pdf: " << Nbin
// 	    << '\n';
  for (int i = 0; i < pdf.pdfList().getSize(); ++i) {
    ipdf = dynamic_cast<RooAbsPdf *>(pdf.pdfList().at(i));
    coef = 0;
    iN = 0;
    if (pdf.coefList().getSize() > i)
      coef = dynamic_cast< RooAbsReal * >(pdf.coefList().at(i));
    RooAbsReal * fullInt = ipdf->createIntegral(obs, RooFit::NormSet(obs));
    if ((allCoefs) && (coef)) {
      iN = coef->getVal();
      RooAbsReal * binInt = ipdf->createIntegral(obs, RooFit::NormSet(obs),
						 RooFit::Range("binRange"));
      iN *= binInt->getVal()/fullInt->getVal();
      delete binInt;

    } else if ((fCoefs) && (coef)) {
      iN = coef->getVal()*Nbin;
      sumf += coef->getVal();
    } else if (fCoefs) {
      iN = (1.-sumf)*Nbin;
    } else {
      iN = ipdf->expectedEvents(RooArgSet(obs));
      RooAbsReal * binInt = ipdf->createIntegral(obs, RooFit::NormSet(obs),
						 RooFit::Range("binRange"));
      iN *= binInt->getVal()/fullInt->getVal();
      delete binInt;
    }
    delete fullInt;
    className = ipdf->ClassName();
    if (className == "RooHistPdf") {
      RooHistPdf * tmpPdf = dynamic_cast<RooHistPdf *>(ipdf);
      retVal += sig2(*tmpPdf, obs, iN);
    } else if (className == "RooAddPdf") {
      RooAddPdf * tmpPdf = dynamic_cast<RooAddPdf *>(ipdf);
      retVal += sig2(*tmpPdf, obs, iN);
    }
  }
  return retVal;

}

double RooWjjMjjFitter::sig2(RooHistPdf& pdf, RooRealVar& obs, double Nbin) {

  obs.setVal(obs.getMin("binRange"));
  double binVol, weight, weightErr;
  double sumw = 0., sumw2 = 0.;
//   std::cout << "pdf: " << pdf.GetName()
// 	    << " Nbin: " << Nbin;
  while (obs.getVal() < obs.getMax("binRange")) {
    pdf.dataHist().get(obs);
    binVol = pdf.dataHist().binVolume();
    weight = pdf.dataHist().weight();
    weightErr = pdf.dataHist().weightError(RooAbsData::SumW2);
//     std::cout << " binVol: " << binVol
// 	      << " weight: " << weight
// 	      << " weightErr: " << weightErr
// 	      << ' ';
    sumw += weight;
    sumw2 += weightErr*weightErr;
    obs.setVal(obs.getVal() + binVol);
  }
//   std::cout << " sumw: " << sumw
// 	    << " sumw2: " << sumw2;
  if (sumw == 0) return 0.;
  double retVal = Nbin*sqrt(sumw2)/sumw;
//   std::cout << " sig: " << retVal << '\n';
  return retVal*retVal;

}
