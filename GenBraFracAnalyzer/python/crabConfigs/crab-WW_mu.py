from WMCore.Configuration import Configuration
config = Configuration()
config.section_("General")
config.General.requestName = 'WW_mu_BraFrac'
config.General.workArea = 'crabProjects'
config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '/afs/cern.ch/user/m/maiqbal/private/aTGC/CMSSW_8_0_28/src/aTGCsAnalysis/GenBraFracAnalyzer/python/ConfFile_cfg.py'
config.section_("Data")
config.Data.inputDataset = '/WWToLNuQQ_13TeV-powheg/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.publication = False
config.section_("Site")
#config.User.voGroup = 'dcms'
config.Site.storageSite = 'T2_DE_DESY'
