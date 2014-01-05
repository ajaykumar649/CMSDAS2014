# PYTHON configuration file for class: JetPlotsExample
# Description:  Example of simple EDAnalyzer for jets.
# Author: K. Kousouris
# Date:  25 - August - 2008
# Modified: Kalanand Mishra
# Date:  11 - January - 2011 (for CMS Data Analysis School jet exercise)


import FWCore.ParameterSet.Config as cms

##  ____        _                       __  __  ____ 
## |  _ \  __ _| |_ __ _    ___  _ __  |  \/  |/ ___|
## | | | |/ _` | __/ _` |  / _ \| '__| | |\/| | |    
## | |_| | (_| | || (_| | | (_) | |    | |  | | |___ 
## |____/ \__,_|\__\__,_|  \___/|_|    |_|  |_|\____|
            
isMC = False
##isMC = True

##   ____             __ _                       _     _           
##  / ___|___  _ __  / _(_) __ _ _   _ _ __ __ _| |__ | | ___  ___ 
## | |   / _ \| '_ \| |_| |/ _` | | | | '__/ _` | '_ \| |/ _ \/ __|
## | |__| (_) | | | |  _| | (_| | |_| | | | (_| | |_) | |  __/\__ \
##  \____\___/|_| |_|_| |_|\__, |\__,_|_|  \__,_|_.__/|_|\___||___/
##                         |___/                                   

NJetsToKeep = 2
CaloJetCollection = 'ak5CaloJets'
#PFJetCollection   = 'ak5PFJets'
JPTJetCollection  = 'JetPlusTrackZSPCorJetAntiKt5'
#GenJetCollection  = 'ak5GenJets'
PFJetCollection   = "goodPatJetsPFlow"
PFJetCollectionTight   = "tightPatJetsPFlow"
CAJetCollection   = "goodPatJetsCA8PF"
CAPrunedJetCollection   = "goodPatJetsCA8PrunedPF"
GenJetCollection  = "ak5GenJetsNoNu"

PlotSuffix = "_Data"
inputFile = 'file:ttbsm_53x_data_1_1_8sY.root'

if isMC:
  PlotSuffix = "_MC"  
  inputFile ='file:ttbsm_52x_mc_128_1_lmR.root'
  

##   _            _           _           
## (_)_ __   ___| |_   _  __| | ___  ___ 
## | | '_ \ / __| | | | |/ _` |/ _ \/ __|
## | | | | | (__| | |_| | (_| |  __/\__ \
## |_|_| |_|\___|_|\__,_|\__,_|\___||___/
    
process = cms.Process("Ana")
#############   Format MessageLogger #################
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10



##  ____             _ ____                           
## |  _ \ ___   ___ | / ___|  ___  _   _ _ __ ___ ___ 
## | |_) / _ \ / _ \| \___ \ / _ \| | | | '__/ __/ _ \
## |  __/ (_) | (_) | |___) | (_) | |_| | | | (_|  __/
## |_|   \___/ \___/|_|____/ \___/ \__,_|_|  \___\___|
                                                   

#############   Set the number of events #############
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(2000)
)
#############   Define the source file ###############
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(inputFile)
)
process.source.inputCommands = cms.untracked.vstring("keep *","drop *_MEtoEDMConverter_*_*")

##   ______     _      _      _
##  |__  __|__ | |_   (_) __| |
##  _  | |/ _ \| __/  | |/ _` |
## | \_| |  __/| |_   | | (_| |
##  \____|\___| \__|  |_|\__,_|
##
from PhysicsTools.SelectorUtils.pfJetIDSelector_cfi import pfJetIDSelector
process.tightPatJetsPFlow = cms.EDFilter("PFJetIDSelectionFunctorFilter",
                                         filterParams = pfJetIDSelector.clone(quality=cms.string("TIGHT")),
                                         src = cms.InputTag("goodPatJetsPFlow")
                                         )
##  ____  _       _       
## |  _ \| | ___ | |_ ___ 
## | |_) | |/ _ \| __/ __|
## |  __/| | (_) | |_\__ \
## |_|   |_|\___/ \__|___/

#############   Calo Jets  ###########################
process.calo = cms.EDAnalyzer("CaloJetPlotsExample",
    JetAlgorithm  = cms.string(CaloJetCollection),
    HistoFileName = cms.string('CaloJetPlotsExample'+PlotSuffix+'.root'),
    NJets         = cms.int32(NJetsToKeep)
)
#############   PF Jets    ###########################
process.pf = cms.EDAnalyzer("PFJetPlotsExample",
    JetAlgorithm  = cms.string(PFJetCollection),
    HistoFileName = cms.string('PFJetPlotsExample'+PlotSuffix+'.root'),
    NJets         = cms.int32(NJetsToKeep)
)
#############   PF Jets, Tight Jet ID  ################
process.pfTight = cms.EDAnalyzer("PFJetPlotsExample",
    JetAlgorithm  = cms.string(PFJetCollectionTight),
    HistoFileName = cms.string('PFJetTightPlotsExample'+PlotSuffix+'.root'),
    NJets         = cms.int32(NJetsToKeep)
)

#############   PF Jets, No Corrections    ###########
process.pfUncorr = cms.EDAnalyzer("PFJetPlotsExample",
    JetAlgorithm  = cms.string(PFJetCollection),
    HistoFileName = cms.string('PFJetUncorrPlotsExample'+PlotSuffix+'.root'),
    NJets         = cms.int32(NJetsToKeep),
    jecLevels     = cms.string("Uncorrected")
)


#############   JPT Jets    ###########################
process.jpt = cms.EDAnalyzer("JPTJetPlotsExample",
    JetAlgorithm  = cms.string(JPTJetCollection),
    HistoFileName = cms.string('JPTJetPlotsExample'+PlotSuffix+'.root'),
    NJets         = cms.int32(NJetsToKeep)
)
#############   Gen Jets   ###########################
process.gen = cms.EDAnalyzer("GenJetPlotsExample",
     JetAlgorithm  = cms.string(GenJetCollection),
     HistoFileName = cms.string('GenJetPlotsExample'+PlotSuffix+'.root'),
     NJets         = cms.int32(NJetsToKeep)
)

#############   Cambridge-Aachen Jets R=0.8 ###########################
process.ca = cms.EDAnalyzer("PFJetPlotsExample",
    JetAlgorithm  = cms.string(CAJetCollection),
    HistoFileName = cms.string('CAJetPlotsExample'+PlotSuffix+'.root'),
    NJets         = cms.int32(NJetsToKeep)
)


#############   Cambridge-Aachen Jets R=0.8, With Pruning ############
process.caPruned = cms.EDAnalyzer("PFJetPlotsExample",
    JetAlgorithm  = cms.string(CAPrunedJetCollection),
    HistoFileName = cms.string('CAPrunedJetPlotsExample'+PlotSuffix+'.root'),
    NJets         = cms.int32(NJetsToKeep)
)


#############   Path       ###########################
##  ____       _   _     
## |  _ \ __ _| |_| |__  
## | |_) / _` | __| '_ \ 
## |  __/ (_| | |_| | | |
## |_|   \__,_|\__|_| |_|

##process.myseq = cms.Sequence(process.calo*process.pf*process.jpt*process.gen)
process.myseq = cms.Sequence(process.pf * process.gen * process.pfUncorr *
                             process.tightPatJetsPFlow * process.pfTight *
                             process.ca * process.caPruned )
  
if not isMC: 
  process.myseq.remove ( process.gen )

process.p = cms.Path( process.myseq)

