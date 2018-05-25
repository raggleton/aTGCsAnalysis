// This small script calculates the multiplying factors for aTGC samples,
// so that the SM distributions from the aTGC samples and SM samples agree.
// Run with root -l calculateaTGCNormFactors.cpp+

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

#define lumi 35922.0

using namespace std;

/*string to_string(double x)
{
  ostringstream ss;
  ss << x;
  return ss.str();
}*/

double nEffEvents(string filename)
{
	TFile file(filename.c_str(), "READ");
	TTree* tree = (TTree*) file.Get("GenWeights/Tree");
	double gentWeight;
	tree->SetBranchAddress("genWeight", &gentWeight);
	double sum = 0;
	for (unsigned int iEntry=0; iEntry<tree->GetEntries(); iEntry++)
	{
		tree->GetEntry(iEntry); 
		//sum += gentWeight/std::abs(gentWeight);
		sum += gentWeight;
	}
	return sum;
}

void calculateFactor(string fileNameSM, double xSecSM,  string fileName600To800, double xSec600To800, string fileName800ToInf, double xSec800ToInf, string ch)
{
	TFile * fileSM = TFile::Open(fileNameSM.c_str());
	TFile * file600To800 = TFile::Open(fileName600To800.c_str());
	TFile * file800ToInf = TFile::Open(fileName800ToInf.c_str());

	TTree * treeSM = (TTree*) fileSM->Get("treeDumper/BasicTree");
	TTree * tree600To800 = (TTree*) file600To800->Get("treeDumper/BasicTree");
	TTree * tree800ToInf = (TTree*) file800ToInf->Get("treeDumper/BasicTree");

	double lumiWeightSM, lumiWeight600To800, lumiWeight800ToInf;
	double MWW, jetPt, jet_tau21_PUPPI, jet_mass_softdrop_PUPPI, W_pt, deltaR_LeptonWJet, deltaPhi_WJetMet, deltaPhi_WJetWlep, l_pt, pfMET, otherTotWeight;
	int nbtag;
	double pfMETval;
	vector<double>* aTGCReweights=0;
	double yieldSM=0;
	double yieldSM600To800=0;
	double yieldSM800ToInf=0;
	double yieldaTGCLow600To800=0;
	double yieldaTGCLow800ToInf=0;
	double yieldaTGCHigh600To800=0;
	double yieldaTGCHigh800ToInf=0;

	// The SM tree
	lumiWeightSM=(xSecSM*lumi)/nEffEvents(fileNameSM);
	treeSM->SetBranchAddress("MWW_SD", &MWW);
	treeSM->SetBranchAddress("jet_pt", &jetPt);
	treeSM->SetBranchAddress("jet_tau21_PUPPI", &jet_tau21_PUPPI);
        treeSM->SetBranchAddress("jet_mass_softdrop_PUPPI", &jet_mass_softdrop_PUPPI);
        treeSM->SetBranchAddress("W_pt", &W_pt);
        treeSM->SetBranchAddress("deltaR_LeptonWJet", &deltaR_LeptonWJet);
        treeSM->SetBranchAddress("deltaPhi_WJetMet", &deltaPhi_WJetMet);
        treeSM->SetBranchAddress("deltaPhi_WJetWlep", &deltaPhi_WJetWlep);
        treeSM->SetBranchAddress("l_pt", &l_pt);
        treeSM->SetBranchAddress("pfMET", &pfMET);
        treeSM->SetBranchAddress("nbtag", &nbtag);
	treeSM->SetBranchAddress("totWeight", &otherTotWeight);
	if (ch=="mu") pfMETval=40.;
	else if (ch=="el") pfMET=80.;
	for (unsigned int iEntry = 0; iEntry < treeSM->GetEntries(); iEntry ++)
	{
		treeSM->GetEntry(iEntry);
		yieldSM += (lumiWeightSM*otherTotWeight);
		if(MWW>600 && MWW<800 && jetPt>200 && jet_tau21_PUPPI<0.55 && jet_mass_softdrop_PUPPI<105. && jet_mass_softdrop_PUPPI>65. && W_pt>200. && abs(deltaR_LeptonWJet)>TMath::Pi()/2. && abs(deltaPhi_WJetMet)>2. && abs(deltaPhi_WJetWlep)>2. && nbtag==0 && pfMET>pfMETval) yieldSM600To800 += (lumiWeightSM*otherTotWeight);
		else if(MWW>=800 && jetPt>200 && jet_tau21_PUPPI<0.55 && jet_mass_softdrop_PUPPI<105. && jet_mass_softdrop_PUPPI>65. && W_pt>200. && abs(deltaR_LeptonWJet)>TMath::Pi()/2. && abs(deltaPhi_WJetMet)>2. && abs(deltaPhi_WJetWlep)>2. && nbtag==0 && pfMET>pfMETval) yieldSM800ToInf += (lumiWeightSM*otherTotWeight);
	}

	// aTGC tree having MWW from 600 to 800 GeV
	lumiWeight600To800=(xSec600To800*lumi)/nEffEvents(fileName600To800);
        tree600To800->SetBranchAddress("MWW_SD", &MWW);
        tree600To800->SetBranchAddress("jet_pt", &jetPt);
	tree600To800->SetBranchAddress("jet_tau21_PUPPI", &jet_tau21_PUPPI);
        tree600To800->SetBranchAddress("jet_mass_softdrop_PUPPI", &jet_mass_softdrop_PUPPI);
        tree600To800->SetBranchAddress("W_pt", &W_pt);
        tree600To800->SetBranchAddress("deltaR_LeptonWJet", &deltaR_LeptonWJet);
        tree600To800->SetBranchAddress("deltaPhi_WJetMet", &deltaPhi_WJetMet);
        tree600To800->SetBranchAddress("deltaPhi_WJetWlep", &deltaPhi_WJetWlep);
        tree600To800->SetBranchAddress("l_pt", &l_pt);
        tree600To800->SetBranchAddress("pfMET", &pfMET);
        tree600To800->SetBranchAddress("nbtag", &nbtag);
        tree600To800->SetBranchAddress("totWeight", &otherTotWeight);
	tree600To800->SetBranchAddress("aTGCWeights", &aTGCReweights);
        for (unsigned int iEntry = 0; iEntry < tree600To800->GetEntries(); iEntry ++)
        {
                tree600To800->GetEntry(iEntry);
		if(MWW<800 && jetPt>200 && jet_tau21_PUPPI<0.55 && jet_mass_softdrop_PUPPI<105. && jet_mass_softdrop_PUPPI>65. && W_pt>200. && abs(deltaR_LeptonWJet)>TMath::Pi()/2. && abs(deltaPhi_WJetMet)>2. && abs(deltaPhi_WJetWlep)>2. && nbtag==0 && pfMET>pfMETval) yieldaTGCLow600To800 += (lumiWeight600To800*otherTotWeight*aTGCReweights->at(61));
		else if(MWW>=800 && jetPt>200 && jet_tau21_PUPPI<0.55 && jet_mass_softdrop_PUPPI<105. && jet_mass_softdrop_PUPPI>65. && W_pt>200. && abs(deltaR_LeptonWJet)>TMath::Pi()/2. && abs(deltaPhi_WJetMet)>2. && abs(deltaPhi_WJetWlep)>2. && nbtag==0 && pfMET>pfMETval) yieldaTGCLow800ToInf += (lumiWeight600To800*otherTotWeight*aTGCReweights->at(61));
        }

	// aTGC tree having MWW from 800 to Inf GeV
	lumiWeight800ToInf=(xSec800ToInf*lumi)/nEffEvents(fileName800ToInf);
        tree800ToInf->SetBranchAddress("MWW_SD", &MWW);
        tree800ToInf->SetBranchAddress("jet_pt", &jetPt);
	tree800ToInf->SetBranchAddress("jet_tau21_PUPPI", &jet_tau21_PUPPI);
        tree800ToInf->SetBranchAddress("jet_mass_softdrop_PUPPI", &jet_mass_softdrop_PUPPI);
        tree800ToInf->SetBranchAddress("W_pt", &W_pt);
        tree800ToInf->SetBranchAddress("deltaR_LeptonWJet", &deltaR_LeptonWJet);
        tree800ToInf->SetBranchAddress("deltaPhi_WJetMet", &deltaPhi_WJetMet);
        tree800ToInf->SetBranchAddress("deltaPhi_WJetWlep", &deltaPhi_WJetWlep);
        tree800ToInf->SetBranchAddress("l_pt", &l_pt);
        tree800ToInf->SetBranchAddress("pfMET", &pfMET);
        tree800ToInf->SetBranchAddress("nbtag", &nbtag);
        tree800ToInf->SetBranchAddress("totWeight", &otherTotWeight);
        tree800ToInf->SetBranchAddress("aTGCWeights", &aTGCReweights);
        for (unsigned int iEntry = 0; iEntry < tree800ToInf->GetEntries(); iEntry ++)
        {
                tree800ToInf->GetEntry(iEntry);
		if(MWW<800 && jetPt>200 && jet_tau21_PUPPI<0.55 && jet_mass_softdrop_PUPPI<105. && jet_mass_softdrop_PUPPI>65. && W_pt>200. && abs(deltaR_LeptonWJet)>TMath::Pi()/2. && abs(deltaPhi_WJetMet)>2. && abs(deltaPhi_WJetWlep)>2. && nbtag==0 && pfMET>pfMETval) yieldaTGCHigh600To800 += (lumiWeight800ToInf*otherTotWeight*aTGCReweights->at(61));
		else if(MWW>=800 && jetPt>200 && jet_tau21_PUPPI<0.55 && jet_mass_softdrop_PUPPI<105. && jet_mass_softdrop_PUPPI>65. && W_pt>200. && abs(deltaR_LeptonWJet)>TMath::Pi()/2. && abs(deltaPhi_WJetMet)>2. && abs(deltaPhi_WJetWlep)>2. && nbtag==0 && pfMET>pfMETval) yieldaTGCHigh800ToInf += (lumiWeight800ToInf*otherTotWeight*aTGCReweights->at(61));
        }
	
	double a=yieldaTGCLow600To800;
	double b=yieldaTGCHigh600To800;
	double c=yieldaTGCLow800ToInf;
	double d=yieldaTGCHigh800ToInf;
	double f=yieldSM600To800;
	double g=yieldSM800ToInf;

	double x= ((f*d)-(g*b)) / ((a*d)-(b*c));
	double y= ((f*c)-(g*a)) / ((b*c)-(a*d));	

	cout<<endl<<"*****************************************************************************************"<<endl;
	cout<<"* Results for "<<fileNameSM<<endl;
	cout<<"*****************************************************************************************"<<endl<<endl;
	cout<<"SM yield in full range: "<<yieldSM<<endl;
	cout<<"SM yield in 600<MWW<800: "<<f<<endl;
	cout<<"SM yield in MWW>800: "<<g<<endl;
	cout<<"aTGC-SM yield in MWW<800: "<<a+b<<endl;
	cout<<"aTGC-SM yield in MWW>800: "<<c+d<<endl<<endl;
	cout<<"Scaling factor for 600<MWW<800: "<<x<<endl;
	cout<<"Scaling factor for MWW>800: "<<y<<endl<<endl;
	cout<<"*****************************************************************************************"<<endl;

	// Plot MWW for checking visually
	string parsingStr;
	TCanvas* can = new TCanvas("can", "Checking aTGC Samples Normalization");

	parsingStr=to_string(lumiWeightSM)+"*totWeight * (jet_pt>200 && jet_tau21_PUPPI<0.55 && jet_mass_softdrop_PUPPI<105. && jet_mass_softdrop_PUPPI>65. && W_pt>200. && abs(deltaR_LeptonWJet)>pi/2. && abs(deltaPhi_WJetMet)>2. && abs(deltaPhi_WJetWlep)>2. && nbtag==0 && pfMET>"+to_string(pfMETval)+")";
	treeSM->SetLineColor(1);
	treeSM->Draw("MWW>>hSM",parsingStr.c_str(),"hist");
	
	TH1F *hSM = (TH1F*)gDirectory->Get("hSM");
	hSM->SetTitle("Weighted Diboson Mass");
	hSM->GetXaxis()->SetTitle("MVV (GeV)");
	hSM->GetYaxis()->SetTitle("Number of Events");

	parsingStr=to_string(lumiWeight600To800*x)+"*totWeight*aTGCWeights[61] * (jet_pt>200 && jet_tau21_PUPPI<0.55 && jet_mass_softdrop_PUPPI<105. && jet_mass_softdrop_PUPPI>65. && W_pt>200. && abs(deltaR_LeptonWJet)>pi/2. && abs(deltaPhi_WJetMet)>2. && abs(deltaPhi_WJetWlep)>2. && nbtag==0 && pfMET>"+to_string(pfMETval)+")";
	tree600To800->SetLineColor(2);
	tree600To800->Draw("MWW>>h600To800",parsingStr.c_str(),"same hist");

	parsingStr=to_string(lumiWeight800ToInf*y)+"*totWeight*aTGCWeights[61] * (jet_pt>200 && jet_tau21_PUPPI<0.55 && jet_mass_softdrop_PUPPI<105. && jet_mass_softdrop_PUPPI>65. && W_pt>200. && abs(deltaR_LeptonWJet)>pi/2. && abs(deltaPhi_WJetMet)>2. && abs(deltaPhi_WJetWlep)>2. && nbtag==0 && pfMET>"+to_string(pfMETval)+")";
	tree800ToInf->SetLineColor(3);
	tree800ToInf->Draw("MWW>>h800ToInf",parsingStr.c_str(),"same hist");

	tree800ToInf->SetLineColor(4);
	tree800ToInf->Draw("MWW>>hSum",parsingStr.c_str(),"same hist");

	TH1F *h600To800 = (TH1F*)gDirectory->Get("h600To800");
	TH1F *hSum = (TH1F*)gDirectory->Get("hSum");

	hSum->Add(h600To800);
	hSum->Draw("same");

	can->Modified();
	can->Update();
	cout<<"Press a key to continue..."<<endl;
	getchar();

}

void calculateaTGCNormFactors()
{
	string prefix = "/afs/cern.ch/work/m/maiqbal/private/aTGC/Samples_80X_Working/";
	
	calculateFactor(prefix+"WW_mu.root", 49.997, prefix+"WW-aTGC_MWW-600To800_mu.root", 0.1833, prefix+"WW-aTGC_MWW-800ToInf_mu.root", 0.2366, "mu");
	calculateFactor(prefix+"WZ_mu.root", 11.46, prefix+"WZ-aTGC_MWZ-600To800_mu.root", 0.06493, prefix+"WZ-aTGC_MWZ-800ToInf_mu.root", 0.1012, "mu");
	calculateFactor(prefix+"WW_ele.root", 49.997, prefix+"WW-aTGC_MWW-600To800_ele.root", 0.1833, prefix+"WW-aTGC_MWW-800ToInf_ele.root", 0.2366, "ele");
	calculateFactor(prefix+"WZ_ele.root", 11.46, prefix+"WZ-aTGC_MWZ-600To800_ele.root", 0.06493, prefix+"WZ-aTGC_MWZ-800ToInf_ele.root", 0.1012, "ele");
}
