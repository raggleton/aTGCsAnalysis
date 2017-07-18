from WMCore.Configuration import Configuration
config = Configuration()
config.section_("General")
config.General.requestName = '{REQUEST_NAME}'
config.General.workArea = 'crab_projects'
config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '{PSET_NAME}'
config.section_("Data")
config.Data.inputDataset = '{DATASET}'
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = {UNITS_PER_JOB}
config.Data.publication = False
config.section_("Site")
config.Site.storageSite = 'T2_DE_DESY'
