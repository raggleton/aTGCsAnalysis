import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:/afs/cern.ch/work/m/maiqbal/private/aTGC/TestSamples/WZ_mu.root'
    )
)

process.demo = cms.EDAnalyzer('GenBraFracAnalyzer',
	src=cms.InputTag("prunedGenParticles")
)


process.p = cms.Path(process.demo)

process.TFileService = cms.Service("TFileService",
	fileName = cms.string("tree.root")
)
