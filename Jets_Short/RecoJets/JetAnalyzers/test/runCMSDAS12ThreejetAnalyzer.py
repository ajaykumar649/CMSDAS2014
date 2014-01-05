import FWCore.ParameterSet.Config as cms

process = cms.Process("data")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

########################################################
 ######### Select Number of Events To Run Over ########
########################################################
process.maxEvents = cms.untracked.PSet( 
  input = cms.untracked.int32(-1) 
)

########################################################
 ######### Specify Which Files To Run Over ############
########################################################
# 1: Signal MC: rpv gluino
# 2: 2011 data (1.1fb-1)
whichfiles= 1


 #############   Define the source file ###############
if (whichfiles==1):
    thefileNames = cms.untracked.vstring('file:/gpfs/gpfsddn/cms/user/cmsdas/2012/JetLongExercise/Files/RPVgluinoPAT.root')
if (whichfiles==2):
    thefileNames = cms.untracked.vstring('file:/gpfs/gpfsddn/cms/user/cmsdas/2012/JetLongExercise/Files/HTDatav1v2May10Rereco_6Jet45GeVPAT.root',
                                         'file:/gpfs/gpfsddn/cms/user/cmsdas/2012/JetLongExercise/Files/HTDatav4_6Jet45GeVPAT.root')

process.source = cms.Source("PoolSource", fileNames = thefileNames)

 ###########   Initialize Analyzer and Run ###########
process.data = cms.EDAnalyzer('CMSDAS12ThreejetTestAnalyzer')                               
process.p = cms.Path(process.data)
