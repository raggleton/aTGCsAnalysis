#!/bin/bash
# This is a script to add extended statistics and put files in working directory.

sourceDir=../Samples_80X_20170512/

echo "Processing Muon Channel"
echo "ttbar";scp $sourceDir"ttbar_mu.root" .
hadd WJets_Ht100To200_mu.root $sourceDir"WJets_Ht100To200_mu.root" $sourceDir"WJets_Ht100To200-ext1_mu.root" $sourceDir"WJets_Ht100To200-ext2_mu.root"
hadd WJets_Ht200To400_mu.root $sourceDir"WJets_Ht200To400_mu.root" $sourceDir"WJets_Ht200To400-ext1_mu.root" $sourceDir"WJets_Ht200To400-ext2_mu.root"
hadd WJets_Ht400To600_mu.root $sourceDir"WJets_Ht400To600_mu.root" $sourceDir"WJets_Ht400To600-ext_mu.root"
hadd WJets_Ht600To800_mu.root $sourceDir"WJets_Ht600To800_mu.root" $sourceDir"WJets_Ht600To800-ext_mu.root"
hadd WJets_Ht800To1200_mu.root $sourceDir"WJets_Ht800To1200_mu.root" $sourceDir"WJets_Ht800To1200-ext_mu.root"
hadd WJets_Ht1200To2500_mu.root $sourceDir"WJets_Ht1200To2500_mu.root" $sourceDir"WJets_Ht1200To2500-ext_mu.root"
hadd WJets_Ht2500ToInf_mu.root $sourceDir"WJets_Ht2500ToInf_mu.root" $sourceDir"WJets_Ht2500ToInf-ext_mu.root"
hadd WW_mu.root $sourceDir"WW_mu.root" $sourceDir"WW-ext_mu.root"
echo "WZ";scp $sourceDir"WZ_mu.root" .
hadd t-ch_mu.root $sourceDir"t-ch_mu.root" $sourceDir"t-ch-ext_mu.root"
echo "s-channel";scp $sourceDir"s-ch_mu.root" .
echo "tw-top";scp $sourceDir"tW-ch-top_mu.root" .
echo "tw-antitop";scp $sourceDir"tW-ch-antitop_mu.root" .
hadd WW-aTGC_mu.root $sourceDir"WW-aTGC_MWW-600To800_mu.root" $sourceDir"WW-aTGC_MWW-800ToInf_mu.root"
hadd WZ-aTGC_mu.root $sourceDir"WZ-aTGC_MWZ-600To800_mu.root" $sourceDir"WZ-aTGC_MWZ-800ToInf_mu.root"
hadd data_mu.root $sourceDir"data-RunB_ver2_mu.root" $sourceDir"data-RunC_mu.root" $sourceDir"data-RunD_mu.root" $sourceDir"data-RunE_mu.root" $sourceDir"data-RunF_mu.root" $sourceDir"data-RunG_mu.root" $sourceDir"data-RunH_ver2_mu.root" $sourceDir"data-RunH_ver3_mu.root"

echo "Processing Electron Channel"
echo "ttbar";scp $sourceDir"ttbar_ele.root" .
hadd WJets_Ht100To200_ele.root $sourceDir"WJets_Ht100To200_ele.root" $sourceDir"WJets_Ht100To200-ext1_ele.root" $sourceDir"WJets_Ht100To200-ext2_ele.root"
hadd WJets_Ht200To400_ele.root $sourceDir"WJets_Ht200To400_ele.root" $sourceDir"WJets_Ht200To400-ext1_ele.root" $sourceDir"WJets_Ht200To400-ext2_ele.root"
hadd WJets_Ht400To600_ele.root $sourceDir"WJets_Ht400To600_ele.root" $sourceDir"WJets_Ht400To600-ext_ele.root"
hadd WJets_Ht600To800_ele.root $sourceDir"WJets_Ht600To800_ele.root" $sourceDir"WJets_Ht600To800-ext_ele.root"
hadd WJets_Ht800To1200_ele.root $sourceDir"WJets_Ht800To1200_ele.root" $sourceDir"WJets_Ht800To1200-ext_ele.root"
hadd WJets_Ht1200To2500_ele.root $sourceDir"WJets_Ht1200To2500_ele.root" $sourceDir"WJets_Ht1200To2500-ext_ele.root"
hadd WJets_Ht2500ToInf_ele.root $sourceDir"WJets_Ht2500ToInf_ele.root" $sourceDir"WJets_Ht2500ToInf-ext_ele.root"
hadd WW_ele.root $sourceDir"WW_ele.root" $sourceDir"WW-ext_ele.root"
echo "WZ";scp $sourceDir"WZ_ele.root" .
hadd t-ch_ele.root $sourceDir"t-ch_ele.root" $sourceDir"t-ch-ext_ele.root"
echo "s-channel";scp $sourceDir"s-ch_ele.root" .
echo "tw-top";scp $sourceDir"tW-ch-top_ele.root" .
echo "tw-antitop";scp $sourceDir"tW-ch-antitop_ele.root" .
hadd WW-aTGC_ele.root $sourceDir"WW-aTGC_MWW-600To800_ele.root" $sourceDir"WW-aTGC_MWW-800ToInf_ele.root"
hadd WZ-aTGC_ele.root $sourceDir"WZ-aTGC_MWZ-600To800_ele.root" $sourceDir"WZ-aTGC_MWZ-800ToInf_ele.root"
hadd data_ele.root $sourceDir"data-RunB_ver2_ele.root" $sourceDir"data-RunC_ele.root" $sourceDir"data-RunD_ele.root" $sourceDir"data-RunE_ele.root" $sourceDir"data-RunF_ele.root" $sourceDir"data-RunG_ele.root" $sourceDir"data-RunH_ver2_ele.root" $sourceDir"data-RunH_ver3_ele.root"
