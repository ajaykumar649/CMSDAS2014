# PYTHON configuration file for class: JetSubstructurePlotsExample
# Description:  Example of simple EDAnalyzer for substructure jets.
# Author: S. Rappoccio
# Date:  8 Dec 2011


import FWCore.ParameterSet.Config as cms

##  ____        _                       __  __  ____ 
## |  _ \  __ _| |_ __ _    ___  _ __  |  \/  |/ ___|
## | | | |/ _` | __/ _` |  / _ \| '__| | |\/| | |    
## | |_| | (_| | || (_| | | (_) | |    | |  | | |___ 
## |____/ \__,_|\__\__,_|  \___/|_|    |_|  |_|\____|
            
#isQCDMC = True
isQCDMC = False
isZprimeMC = True
#isZprimeMC = False
#isData = True
isData = False

##   ____             __ _                       _     _           
##  / ___|___  _ __  / _(_) __ _ _   _ _ __ __ _| |__ | | ___  ___ 
## | |   / _ \| '_ \| |_| |/ _` | | | | '__/ _` | '_ \| |/ _ \/ __|
## | |__| (_) | | | |  _| | (_| | |_| | | | (_| | |_) | |  __/\__ \
##  \____\___/|_| |_|_| |_|\__, |\__,_|_|  \__,_|_.__/|_|\___||___/
##                         |___/

PFJetCollection   = "goodPatJetsCA8PrunedPF"

if isData:
  PlotSuffix = "_Data"
  inputFile = 'file:ttbsm_53x_data_1_1_8sY.root'

if isQCDMC:
  PlotSuffix = "_QCDMC"  
  inputFile ='file:ttbsm_52x_mc_128_1_lmR.root'
  
if isZprimeMC:
  PlotSuffix = "_ZprimeMC"  
  inputFile ='file:ttbsm_52x_mc_zprime1TeV.root'


##  _            _           _           
## (_)_ __   ___| |_   _  __| | ___  ___ 
## | | '_ \ / __| | | | |/ _` |/ _ \/ __|
## | | | | | (__| | |_| | (_| |  __/\__ \
## |_|_| |_|\___|_|\__,_|\__,_|\___||___/
    
process = cms.Process("Ana")
#############   Format MessageLogger #################
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100


process.TFileService = cms.Service("TFileService",
  fileName = cms.string('jetSubstructurePlotsExample' + PlotSuffix + '.root')
)


##  ____             _ ____                           
## |  _ \ ___   ___ | / ___|  ___  _   _ _ __ ___ ___ 
## | |_) / _ \ / _ \| \___ \ / _ \| | | | '__/ __/ _ \
## |  __/ (_) | (_) | |___) | (_) | |_| | | | (_|  __/
## |_|   \___/ \___/|_|____/ \___/ \__,_|_|  \___\___|
                                                   

#############   Set the number of events #############
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10000)
)
#############   Define the source file ###############
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(inputFile)
)
process.source.inputCommands = cms.untracked.vstring("keep *","drop *_MEtoEDMConverter_*_*")


##  ____  _       _       
## |  _ \| | ___ | |_ ___ 
## | |_) | |/ _ \| __/ __|
## |  __/| | (_) | |_\__ \
## |_|   |_|\___/ \__|___/

#############   PF Jets    ###########################
process.pf = cms.EDAnalyzer("JetSubstructurePlotsExample",
    jetSrc = cms.InputTag('goodPatJetsCA8PrunedPF'),
    leadJetPtMin = cms.double(400.0),
    jetPtMin = cms.double(50.0)
)



#############   Path       ###########################
##  ____       _   _     
## |  _ \ __ _| |_| |__  
## | |_) / _` | __| '_ \ 
## |  __/ (_| | |_| | | |
## |_|   \__,_|\__|_| |_|

process.myseq = cms.Sequence(process.pf )
process.p = cms.Path( process.myseq)

