// Example of jet substructure example: JetSubstructurePlotsExample
// Description:  Example of simple EDAnalyzer to check into jet substructure
// Author: S. Rappoccio
// Date: 07 Dec 2011
#ifndef JetSubstructurePlotsExample_h
#define JetSubstructurePlotsExample_h
#include <TH1.h>
#include <TFile.h>
#include "TNamed.h"
#include <vector>
#include <map>
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

class JetSubstructurePlotsExample : public edm::EDAnalyzer 
   {
     public:
       JetSubstructurePlotsExample(edm::ParameterSet const& cfg);
     private:
       void beginJob();
       void analyze(edm::Event const& e, edm::EventSetup const& iSetup);
       void endJob();

       edm::InputTag            jetSrc_;      /// Jet source to plot
       double                   leadJetPtMin_;/// Minimum leading jet pt, in place to be above trigger threshold
       double                   jetPtMin_;    /// Minimum jet pt to plot
       TFileDirectory *         theDir_;      /// Histogram directory for this module
   };
#endif
