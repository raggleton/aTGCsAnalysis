///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
// Small script for checking sum of genweights in processed
// trees.
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////

#include <iostream>
#include <TFile.h>
#include <TDirectoryFile.h>
#include <TTree.h>
#include <TTreeFormula.h>
#include <TBranch.h>

using namespace std;

void nEvents(string filename)
{

  double gentWeight;
  int sumSigns=0;
  int sumGenWeights=0;
  TFile file(filename.c_str(), "READ");
  TTree * tree = (TTree*) file.Get("GenWeights/Tree");
  
  cout<<filename<<endl;
  cout<<"Number of entries: "<<tree->GetEntries()<<endl;

  tree->SetBranchAddress("genWeight", &gentWeight);
  for (unsigned int iEntry=0; iEntry<tree->GetEntries(); iEntry++)
  {
    tree -> GetEntry(iEntry); 
    sumSigns+=gentWeight/abs(gentWeight);
    sumGenWeights+=gentWeight;
  }
  
  cout<<"Sum of genWeight signs: "<<sumSigns<<endl;
  cout<<"Sum of genWeights: "<<sumGenWeights<<endl<<endl;

  return;
}

void checkSumEvents()
{

  std::string prefix = "/afs/cern.ch/work/m/maiqbal/private/aTGC/Samples_80X_Working/";

  // Electron channel
  /*nEvents(prefix + "WW_ele.root");
  nEvents(prefix + "WZ_ele.root");
  nEvents(prefix + "s-ch_ele.root");
  nEvents(prefix + "t-ch_ele.root");
  nEvents(prefix + "tW-ch-top_ele.root");
  nEvents(prefix + "tW-ch-antitop_ele.root");
  nEvents(prefix + "WJets_Ht100To200_ele.root");
  nEvents(prefix + "WJets_Ht200To400_ele.root"); 
  nEvents(prefix + "WJets_Ht400To600_ele.root"); 
  nEvents(prefix + "WJets_Ht600To800_ele.root"); 
  nEvents(prefix + "WJets_Ht800To1200_ele.root");
  nEvents(prefix + "WJets_Ht1200To2500_ele.root"); 
  nEvents(prefix + "WJets_Ht2500ToInf_ele.root"); 
  nEvents(prefix + "ttbar_ele.root");

  nEvents(prefix + "WW-aTGC_MWW-600To800_ele.root");
  nEvents(prefix + "WW-aTGC_MWW-800ToInf_ele.root");
  nEvents(prefix + "WZ-aTGC_MWZ-600To800_ele.root");
  nEvents(prefix + "WZ-aTGC_MWZ-800ToInf_ele.root");*/
  
  // Muon channel
  nEvents(prefix + "WW_mu.root");
  nEvents(prefix + "WZ_mu.root");
  nEvents(prefix + "s-ch_mu.root");
  nEvents(prefix + "t-ch_mu.root"); 
  nEvents(prefix + "tW-ch-top_mu.root"); 
  nEvents(prefix + "tW-ch-antitop_mu.root"); 
  nEvents(prefix + "WJets_Ht100To200_mu.root"); 
  nEvents(prefix + "WJets_Ht200To400_mu.root");
  nEvents(prefix + "WJets_Ht400To600_mu.root");
  nEvents(prefix + "WJets_Ht600To800_mu.root");
  nEvents(prefix + "WJets_Ht800To1200_mu.root");
  nEvents(prefix + "WJets_Ht1200To2500_mu.root"); 
  nEvents(prefix + "WJets_Ht2500ToInf_mu.root");
  nEvents(prefix + "ttbar_mu.root");

  nEvents(prefix + "WW-aTGC_MWW-600To800_mu.root");
  nEvents(prefix + "WW-aTGC_MWW-800ToInf_mu.root");
  nEvents(prefix + "WZ-aTGC_MWZ-600To800_mu.root");  
  nEvents(prefix + "WZ-aTGC_MWZ-800ToInf_mu.root");

}
