#include <iostream>
#include <fstream>
#include <strstream>
#include <vector>
#include <TFile.h>
#include <TTree.h>
#include <TNtuple.h>
#include <TString.h>
#include <TH1.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TStyle.h>


#include "TMath.h"
#include "TF1.h"
#include "TMinuit.h"
#include "TLatex.h"

using namespace std;

//// Global Parameters
const TString StorageDirPrefix = "dcap://cmsdca.fnal.gov:24125/pnfs/fnal.gov/usr/cms/WAX/11/store/user/cmsdas/2013/JetsLongExerciseDijetMassSpectrumInAssociationWithW/";//At FNAL

void CombinationPlotter () {

  ///Define the cuts string and histogram range
  TString Cuts="(W_muon_pt>25)&&(abs(W_muon_eta)<2.1)&&(event_met_pfmet>25)&&(JetPFCor_Pt[1]>30)&&(JetPFCor_Pt[2]<30)&&(abs(JetPFCor_Eta[0])<2.4)&&(abs(JetPFCor_Eta[1])<2.4)&&(abs(JetPFCor_Eta[0]-JetPFCor_Eta[1])<1.2)&&(JetPFCor_Pt[0]>40)&&(JetPFCor_Pt[1]/Mass2j_PFCor>0.3)&&(JetPFCor_Pt[1]/Mass2j_PFCor<0.7)&&(sqrt(JetPFCor_Pt[0]**2+JetPFCor_Pt[1]**2+2*JetPFCor_Pt[0]*JetPFCor_Pt[1]*cos(JetPFCor_Phi[0]-JetPFCor_Phi[1]))>45.)&&(abs(JetPFCor_dphiMET[0])>0.4)&&(W_mt>50.)&&(Mass2j_PFCor<300)&&(Mass2j_PFCor>60)";
  double Min=60.0;
  double Max=300.0;
  int NBins= 25;


  ///Fill the histograms for each individual background.
  TFile* f_TTbar = TFile::Open(StorageDirPrefix + "ReducedTrees/RD_mu_TTbar_CMSSW532.root");
  TTree* InTree_TTbar = (TTree*)f_TTbar->Get("WJet");
  TH1F* h_TTbar= new TH1F("h_TTbar","h_TTbar",NBins,Min,Max);
  InTree_TTbar->Draw("Mass2j_PFCor>>h_TTbar",Cuts);

  TFile* f_STopT_T = TFile::Open(StorageDirPrefix + "ReducedTrees/RD_mu_STopT_T_CMSSW532.root");
  TTree* InTree_STopT_T = (TTree*)f_STopT_T->Get("WJet");
  TH1F* h_STopT_T= new TH1F("h_STopT_T","h_STopT_T",NBins,Min,Max);
  InTree_STopT_T->Draw("Mass2j_PFCor>>h_STopT_T",Cuts);

  TFile* f_STopT_Tbar = TFile::Open(StorageDirPrefix + "ReducedTrees/RD_mu_STopT_Tbar_CMSSW532.root");
  TTree* InTree_STopT_Tbar = (TTree*)f_STopT_Tbar->Get("WJet");
  TH1F* h_STopT_Tbar= new TH1F("h_STopT_Tbar","h_STopT_Tbar",NBins,Min,Max);
  InTree_STopT_Tbar->Draw("Mass2j_PFCor>>h_STopT_Tbar",Cuts);

  TFile* f_STopS_T = TFile::Open(StorageDirPrefix + "ReducedTrees/RD_mu_STopS_T_CMSSW532.root");
  TTree* InTree_STopS_T = (TTree*)f_STopS_T->Get("WJet");
  TH1F* h_STopS_T= new TH1F("h_STopS_T","h_STopS_T",NBins,Min,Max);
  InTree_STopS_T->Draw("Mass2j_PFCor>>h_STopS_T",Cuts);

  TFile* f_STopS_Tbar = TFile::Open(StorageDirPrefix + "ReducedTrees/RD_mu_STopS_Tbar_CMSSW532.root");
  TTree* InTree_STopS_Tbar = (TTree*)f_STopS_Tbar->Get("WJet");
  TH1F* h_STopS_Tbar= new TH1F("h_STopS_Tbar","h_STopS_Tbar",NBins,Min,Max);
  InTree_STopS_Tbar->Draw("Mass2j_PFCor>>h_STopS_Tbar",Cuts);

  TFile* f_STopTW_T = TFile::Open(StorageDirPrefix + "ReducedTrees/RD_mu_STopTW_T_CMSSW532.root");
  TTree* InTree_STopTW_T = (TTree*)f_STopTW_T->Get("WJet");
  TH1F* h_STopTW_T= new TH1F("h_STopTW_T","h_STopTW_T",NBins,Min,Max);
  InTree_STopTW_T->Draw("Mass2j_PFCor>>h_STopTW_T",Cuts);

  TFile* f_STopTW_Tbar = TFile::Open(StorageDirPrefix + "ReducedTrees/RD_mu_STopTW_Tbar_CMSSW532.root");
  TTree* InTree_STopTW_Tbar = (TTree*)f_STopTW_Tbar->Get("WJet");
  TH1F* h_STopTW_Tbar= new TH1F("h_STopTW_Tbar","h_STopTW_Tbar",NBins,Min,Max);
  InTree_STopTW_Tbar->Draw("Mass2j_PFCor>>h_STopTW_Tbar",Cuts);


 TFile* f_ZpJ = TFile::Open(StorageDirPrefix + "ReducedTrees/RD_mu_ZpJ_CMSSW532.root");
  TTree* InTree_ZpJ = (TTree*)f_ZpJ->Get("WJet");
  TH1F* h_ZpJ= new TH1F("h_ZpJ","h_ZpJ",NBins,Min,Max);
  InTree_ZpJ->Draw("Mass2j_PFCor>>h_ZpJ",Cuts);


  TFile* f_WpJ = TFile::Open(StorageDirPrefix + "ReducedTrees/RD_mu_WpJ_CMSSW532.root");
  TTree* InTree_WpJ = (TTree*)f_WpJ->Get("WJet");
  TH1F* h_WpJ= new TH1F("h_WpJ","h_WpJ",NBins,Min,Max);
  InTree_WpJ->Draw("Mass2j_PFCor>>h_WpJ",Cuts);

 TFile* f_WW = TFile::Open(StorageDirPrefix + "ReducedTrees/RD_mu_WW_CMSSW532.root");
  TTree* InTree_WW = (TTree*)f_WW->Get("WJet");
  TH1F* h_WW= new TH1F("h_WW","h_WW",NBins,Min,Max);
  InTree_WW->Draw("Mass2j_PFCor>>h_WW",Cuts);

  ////BEGIN STACKED DRAW
  ///Fill the histogram for the Data
  TFile* f_Data = TFile::Open(StorageDirPrefix + "ReducedTrees/RD_WmunuJets_DataAll_GoldenJSON_19p3invfb.root");
  TTree* InTree_Data = (TTree*)f_Data->Get("WJet");
  TH1F* h_Data= new TH1F("h_Data","h_Data",NBins,Min,Max);
  InTree_Data->Draw("Mass2j_PFCor>>h_Data",Cuts);


  /// Scale the histograms to the expected event count in the data = Luminosity*CrossSection*(NPassingAllCuts/NGenerated)
  /// Typical place to find the cross sections: https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSectionsat8TeV
  double Lumi=19300;//Luminosity in pb.

  h_TTbar->Scale(Lumi*225.197/6893735);
  cout << "Number Of Expected TTbar Events in " << Lumi << " pb of Data = " << h_TTbar->Integral() << endl;

  h_STopT_T->Scale(Lumi*55.531/3758221);
  h_STopT_Tbar->Scale(Lumi*30.0042/1935066);
  h_STopS_T->Scale(Lumi*3.89394/259960);
  h_STopS_Tbar->Scale(Lumi*1.75776/139974);
  h_STopTW_T->Scale(Lumi*11.1773/497657);
  h_STopTW_Tbar->Scale(Lumi*11.1773/493458);
  h_ZpJ->Scale(Lumi*3503.71/30209426);
  h_WpJ->Scale(Lumi*36257.2/69122011);
  h_WW->Scale(Lumi*57.1097/9450414);


 cout << "Rescaling to fit results" << endl;
  double dataFitScale=266404.0/261094.0;
  h_WW->Scale(dataFitScale*6166.9/4629.8);
  h_WpJ->Scale(dataFitScale*211360./239990.);
  //hMix->Scale(dataFitScale*42822/45383.);

  cout << "Number Of Expected Z+Jets Events in " << Lumi << " pb of Data = " << h_ZpJ->Integral() << endl;
  cout << "Number Of Expected WpJ Events in " << Lumi << " pb of Data = " << h_WpJ->Integral() << endl;
  cout << "Number Of Expected WW Events in " << Lumi << " pb of Data = " << h_WW->Integral() << endl;
  cout << "Number of Events in Data = " << h_Data->Integral() << endl;


 TH1F* hSingleTop= new TH1F("hSingleTop","Single Top Processes",NBins,Min,Max);
  hSingleTop->Add(h_STopT_T);
  hSingleTop->Add(h_STopT_Tbar);
  hSingleTop->Add(h_STopS_T);
  hSingleTop->Add(h_STopS_Tbar);
  hSingleTop->Add(h_STopTW_T);
  hSingleTop->Add(h_STopTW_Tbar);

 cout << "Number Of Expected Single Top Events in " << Lumi << " pb of Data = " << hSingleTop->Integral() << endl;

TH1F* hMix= new TH1F("hMix","Weighted Sum of The Mixture Processes",NBins,Min,Max);
  hMix->Add(h_TTbar);
  hMix->Add(hSingleTop);
  hMix->Add(h_ZpJ);

  hMix->Scale(dataFitScale*42822/45383.);

  cout << "Number Of Expected Mixture Events in " << Lumi << " pb of Data = " << hMix->Integral() << endl;
  hMix->Draw();

  // Add all of the histograms
  TH1F* hTotal= new TH1F("hTotal","Weighted Sum of All Processes",NBins,Min,Max);
  hTotal->Add(h_WW);
  hTotal->Add(h_WpJ);
  hTotal->Add(hMix);
  cout << "Total Number Of Expected Events in " << Lumi << " pb of Data = " << hTotal->Integral() << endl;


 // Some graphics options
  gStyle->SetOptStat(0);//Remove the Stat Box
  hTotal->SetXTitle("M_{jj}");//Label the X Axis
  hTotal->SetYTitle("Evts/14GeV");//Label the Y Axis
  hTotal->Draw();

  ////BEGIN STACKED DRAW
  ///Draw hTotal, h_Mix+h_WpJ on top of it and h_Mix on top of it; effectively drawing a stacked histogram of h_Mix, h_WpJ and h_WW.
  ///Fill the Mix + WJets histogram
  TH1F* hMixAndWpJ= new TH1F("hMixAndWpJ","Sum of h_Mix and h_WpJ",NBins,Min,Max);
  hMixAndWpJ->Add(hMix);
  hMixAndWpJ->Add(h_WpJ);

////END STACKED DRAW
  ///Draw the histogram for the Data
  h_Data->SetLineWidth(2);
  h_Data->SetMarkerStyle(8);
  h_Data->SetMarkerSize(1);

  cout << "Chi2/ndf=" << h_Data->Chi2Test(hTotal,"CHI2/NDF") << endl;

  ///Set the graphics options
  hTotal->SetFillColor(kYellow);//The only part that will be visible is the WW
  hMixAndWpJ->SetFillColor(kRed);//The WpJ will be visible
  hMix->SetFillColor(kBlack);
  TCanvas *cnv = new TCanvas("cnv","cnv",10,10,900,600);
  TLegend *lgnd = new TLegend(0.7,0.65,0.90,0.9);
  lgnd->AddEntry(hMix,"Top and Z+Jets mix","f");
  lgnd->AddEntry(hMixAndWpJ,"W+Jets","f");
  lgnd->AddEntry(hTotal,"WW","f");
  lgnd->AddEntry(h_Data,"Data","l");

  ///Draw
  hTotal->Draw();
  hMixAndWpJ->Draw("same");
  hMix->Draw("same");
  h_Data->Draw("esame");
  lgnd->Draw();
  cnv->SaveAs("WJets_long_exe.png");
  ////END STACKED DRAW

}



