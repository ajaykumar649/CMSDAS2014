#include <memory>
#include <iostream>
#include <algorithm>
#include <vector>
#include <TFile.h>
#include <TMath.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TRandom.h>
#include <TROOT.h>
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/PatCandidates/interface/Jet.h" // based on DataFormats/Candidate/interface/Particle.h
#include <FWCore/Framework/interface/ESHandle.h>
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"


class CMSDAS12ThreejetTestAnalyzer : public edm::EDAnalyzer {
  public:
    explicit CMSDAS12ThreejetTestAnalyzer(const edm::ParameterSet&);
            ~CMSDAS12ThreejetTestAnalyzer() {};

  private:
    virtual void beginJob();
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob();
    virtual bool findMCDaughters(std::vector<pat::Jet>);

    //Internal variables
    int nEvents;
    double _sumPtMin, _etacut, _jetptcut;
    std::string _outputFilename, _outputFilename2, corStep, corFlvr;   
    std::string _patJetType, _inputMCPupname, _inputDatPupname;
    TFile* outputFile; 
    edm::Handle< std::vector<pat::Jet> > PatJets;
    edm::Handle< std::vector<pat::Jet> > myselectedPatJets;
    std::vector<reco::GenParticle*> partons;
    std::vector<reco::GenJet*>      genjets;
    std::vector<pat::Jet>           matchedjets;

    //Histograms.
    TH1F* hNJets;
    TH1F* hJetPt;
    TH1F* hJetEta;
    TH1F* hJetPhi;

    TH1F* hSumHT;
    TH1F* hSelHT;
    TH1F* hNGoodJets;
    TH1F* hSelJetEta;
    TH1F* hSelJetPhi;
    TH1F* hSelJetPt;

    TH1F* hGenjetMass;
    TH2F* hGenjetMassVsSumPt;
    TH1F* hPartonMass;
    TH2F* hPartonMassVsSumPt;
    TH1F* hMatchedMass;
    TH2F* hMatchedMassVsSumPt;
    TH1F* hTripletMass;
    TH2F* hTripletMassVsSumPt;
    TH1F* hAllTripletMass;
    TH2F* hAllTripletMassVsSumPt;

};
