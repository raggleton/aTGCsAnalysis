// This is a script for debugging certain uncertainties.

#include <TFile.h>
#include <TTree.h>
#include <TTreeFormula.h>
#include <TBranch.h>
#include <TH1.h>
#include <TPad.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TDirectoryFile.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <TMath.h>

using namespace std;

void debugUnc()
{
	TFile * fileIn = TFile::Open("/afs/cern.ch/work/m/maiqbal/private/aTGC/Samples_80X_Working/WW_ele.root");
	TTree * treeIn = (TTree*) fileIn->Get("BasicTree");

	double MWW, MWW_SD, jet_mass_softdrop_PUPPI, l_pt;
	double MWW_LeptonResUp, MWW_SD_LeptonResUp, W_pt_LeptonResUp, l_pt_LeptonEnDown, MWW_SD_LeptonEnDown;
	double jet_pt, jet_tau21_PUPPI, deltaR_LeptonWJet, deltaPhi_WJetMet, deltaPhi_WJetWlep, pfMET;
	int nbtag;
	double W_pt, W_eta, W_phi, W_mass;
	double W_pt_LeptonEnDown, W_mass_LeptonEnDown;

	treeIn->SetBranchAddress("MWW", &MWW);
	treeIn->SetBranchAddress("MWW_SD", &MWW_SD);
	treeIn->SetBranchAddress("jet_mass_softdrop_PUPPI", &jet_mass_softdrop_PUPPI);
	treeIn->SetBranchAddress("l_pt", &l_pt);
	treeIn->SetBranchAddress("MWW_LeptonResUp", &MWW_LeptonResUp);
	treeIn->SetBranchAddress("MWW_SD_LeptonResUp", &MWW_SD_LeptonResUp);
	treeIn->SetBranchAddress("W_pt_LeptonResUp", &W_pt_LeptonResUp);
	treeIn->SetBranchAddress("jet_pt", &jet_pt);
	treeIn->SetBranchAddress("jet_tau21_PUPPI", &jet_tau21_PUPPI);
	treeIn->SetBranchAddress("deltaR_LeptonWJet", &deltaR_LeptonWJet);
	treeIn->SetBranchAddress("deltaPhi_WJetMet", &deltaPhi_WJetMet);
	treeIn->SetBranchAddress("deltaPhi_WJetWlep", &deltaPhi_WJetWlep);
	treeIn->SetBranchAddress("nbtag", &nbtag);
	treeIn->SetBranchAddress("pfMET", &pfMET);
	treeIn->SetBranchAddress("W_pt", &W_pt);
	treeIn->SetBranchAddress("W_eta", &W_eta);
        treeIn->SetBranchAddress("W_phi", &W_phi);
        treeIn->SetBranchAddress("W_mass", &W_mass);
        treeIn->SetBranchAddress("W_pt_LeptonEnDown", &W_pt_LeptonEnDown);
        treeIn->SetBranchAddress("W_mass_LeptonEnDown", &W_mass_LeptonEnDown);
	treeIn->SetBranchAddress("MWW_SD_LeptonEnDown",&MWW_SD_LeptonEnDown);
	treeIn->SetBranchAddress("l_pt_LeptonEnDown", &l_pt_LeptonEnDown);

	int interestingEvents=0;
	for (unsigned int iEntry = 0; iEntry < treeIn->GetEntries(); iEntry ++)
	{
		treeIn->GetEntry(iEntry);
		if(jet_pt > 200. && jet_tau21_PUPPI < 0.55  && jet_mass_softdrop_PUPPI < 105. && jet_mass_softdrop_PUPPI > 65. && W_pt > 200.  && abs(deltaR_LeptonWJet) > TMath::Pi()/2. && abs(deltaPhi_WJetMet) > 2. && abs(deltaPhi_WJetWlep) > 2. && MWW_SD < 900. && MWW_SD_LeptonEnDown > 900. && nbtag == 0 && l_pt > 50. && pfMET > 80.)
		{
			interestingEvents++;
			cout<<"========================================================"<<endl;
			cout<<"Event "<<interestingEvents<<":"<<endl<<endl;
			cout<<"l_pt "<<l_pt<<endl;
                        cout<<"l_pt_LeptonEnDown "<<l_pt_LeptonEnDown<<endl;
			cout<<"W_pt "<<W_pt<<endl;
			cout<<"W_pt_LeptonEnDown "<<W_pt_LeptonEnDown<<endl;
			cout<<"W_mass "<<W_mass<<endl;
			cout<<"W_mass_LeptonEnDown "<<W_mass_LeptonEnDown<<endl;
			cout<<"MWW_SD "<<MWW_SD<<endl;
			cout<<"MWW_SD_LeptonEnDown "<<MWW_SD_LeptonEnDown<<endl;
		}
	}

	cout<<"========================================================"<<endl;
	cout<<"Found "<<interestingEvents<<" interesting events."<<endl;
	cout<<"========================================================"<<endl;
}
