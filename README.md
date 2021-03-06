aTGC Analysis
========

```
This is the analysis code for anomalous triple gauge couplings at 13 TeV using CMSSW framework.

Setup Instructions
------------------

# Setup CMSSW
cmsrel CMSSW_8_0_25
cd CMSSW_8_0_25/src
cmsenv

# Checkout aTGC analysis code
git cms-merge-topic -u cms-met:CMSSW_8_0_X-METFilterUpdate
git clone -b 80X git@github.com:muhammadansariqbal/aTGCsAnalysis.git

# Compile
scram b -j 10

# PDF variation map may need to be updated
vi aTGCsAnalysis/TreeMaker/plugins/PDFVariationMap.h

# Jobs can be submit and retrieved using python scripts
cd aTGCsAnalysis/Common/test/
python submit_jobs.py
python retrieve_jobs.py

# Compile the plotting code
cd aTGCsAnalysis/Common/test/Plotting/
make

# Add weight samples after putting in right luminosity
vi CMSLumi.cpp
cd ..
root -l addWeightSamples.cpp+

# Draw
cd Plotting
./draw <arguments>

# Allowed options:
  --help                produce help message
  --channel arg         channel to be used: ele or mu
  --CR arg              control region to make plots with
  --output arg          tag for the output directory
  --input arg           name of input directory
  --withMC              use Monte Carlo or not
  --withData            use data or not
  --withSignal          draw signal or not
  --withSystematics     calculate systematics or not. If not statistical uncertainties are calculated and drawn.
  --wantToWriteHists    to write histograms to the local file

An example below makes plots in the ttbar control region in the electron channel with data, Monte-Carlo, signal and no systematics :
./draw --CR ttbar --channel ele --output ttbar_CR --input /afs/cern.ch/work/m/maiqbal/private/aTGC/Samples_80X_Working/ --withSignal --withMC --withData
