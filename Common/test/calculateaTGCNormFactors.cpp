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

#define lumi 35922.0

using namespace std;

/*string to_string(double x)
{
  ostringstream ss;
  ss << x;
  return ss.str();
}*/

int nEffEvents(string filename)
{
	TFile file(filename.c_str(), "READ");
	TTree* tree = (TTree*) file.Get("GenWeights/Tree");
	double gentWeight;
	tree->SetBranchAddress("genWeight", &gentWeight);
	int sum = 0;
	for (unsigned int iEntry=0; iEntry<tree->GetEntries(); iEntry++)
	{
		tree->GetEntry(iEntry); 
		//sum += gentWeight/std::abs(gentWeight);
		sum += gentWeight;
	}
	return sum;
}

void calculateFactor(string fileNameSM, double xSecSM,  string fileName600To800, double xSec600To800, string fileName800ToInf, double xSec800ToInf)
{
	TFile * fileSM = TFile::Open(fileNameSM.c_str());
	TFile * file600To800 = TFile::Open(fileName600To800.c_str());
	TFile * file800ToInf = TFile::Open(fileName800ToInf.c_str());

	TTree * treeSM = (TTree*) fileSM->Get("treeDumper/BasicTree");
	TTree * tree600To800 = (TTree*) file600To800->Get("treeDumper/BasicTree");
	TTree * tree800ToInf = (TTree*) file800ToInf->Get("treeDumper/BasicTree");

	double lumiWeightSM, lumiWeight600To800, lumiWeight800ToInf;
	double MWW, jetPt, otherTotWeight;
	vector<double>* aTGCReweights=0;
	double yieldSM=0;
	double yieldSM600To800=0;
	double yieldSM800ToInf=0;
	double yield600To800=0;
	double yield800ToInf=0;

	// The SM tree
	lumiWeightSM=(xSecSM*lumi)/nEffEvents(fileNameSM);
	treeSM->SetBranchAddress("MWW", &MWW);
	treeSM->SetBranchAddress("jet_pt", &jetPt);
	treeSM->SetBranchAddress("totWeight", &otherTotWeight);
	for (unsigned int iEntry = 0; iEntry < treeSM->GetEntries(); iEntry ++)
	{
		treeSM->GetEntry(iEntry);
		yieldSM += (lumiWeightSM*otherTotWeight);
		if(MWW>600 && MWW<800 && jetPt>180) yieldSM600To800 += (lumiWeightSM*otherTotWeight);
		else if(MWW>800 && jetPt>180) yieldSM800ToInf += (lumiWeightSM*otherTotWeight);
	}

	// aTGC tree having MWW from 600 to 800 Gev
	lumiWeight600To800=(xSec600To800*lumi)/nEffEvents(fileName600To800);
        tree600To800->SetBranchAddress("MWW", &MWW);
        tree600To800->SetBranchAddress("jet_pt", &jetPt);
        tree600To800->SetBranchAddress("totWeight", &otherTotWeight);
	tree600To800->SetBranchAddress("aTGCWeights", &aTGCReweights);
        for (unsigned int iEntry = 0; iEntry < tree600To800->GetEntries(); iEntry ++)
        {
                tree600To800->GetEntry(iEntry);
                yield600To800 += (lumiWeight600To800*otherTotWeight*aTGCReweights->at(61));
        }

	// aTGC tree having MWW from 800 to Inf Gev
	lumiWeight800ToInf=(xSec800ToInf*lumi)/nEffEvents(fileName800ToInf);
        tree800ToInf->SetBranchAddress("MWW", &MWW);
        tree800ToInf->SetBranchAddress("jet_pt", &jetPt);
        tree800ToInf->SetBranchAddress("totWeight", &otherTotWeight);
        tree800ToInf->SetBranchAddress("aTGCWeights", &aTGCReweights);
        for (unsigned int iEntry = 0; iEntry < tree800ToInf->GetEntries(); iEntry ++)
        {
                tree800ToInf->GetEntry(iEntry);
                yield800ToInf += (lumiWeight800ToInf*otherTotWeight*aTGCReweights->at(61));
        }

	cout<<endl<<"*****************************************************************************************"<<endl;
	cout<<"* Results for "<<fileNameSM<<endl;
	cout<<"*****************************************************************************************"<<endl<<endl;
	cout<<"SM yield in full range: "<<yieldSM<<endl;
	cout<<"SM yield in 600<MWW<800 jetPt>180: "<<yieldSM600To800<<endl;
	cout<<"SM yield in MWW>800 jetPt>180: "<<yieldSM800ToInf<<endl;
	cout<<"aTGC-SM yield in 600<MWW<800 jetPt>180: "<<yield600To800<<endl;
	cout<<"aTGC-SM yield in MWW>800 jetPt>180: "<<yield800ToInf<<endl<<endl;
	cout<<"Scaling factor for 600<MWW<800: "<<yieldSM600To800/yield600To800<<endl;
	cout<<"Scaling factor for MWW>800: "<<yieldSM800ToInf/yield800ToInf<<endl<<endl;
	cout<<"*****************************************************************************************"<<endl;

	// Plot MWW for checking visually
	string parsingStr;
	TCanvas* can = new TCanvas("can", "Checking aTGC Samples Normalization");

	parsingStr=to_string(lumiWeightSM)+"*totWeight";
	treeSM->SetLineColor(1);
	treeSM->Draw("MWW>>hSM",parsingStr.c_str(),"hist");
	
	TH1F *hSM = (TH1F*)gDirectory->Get("hSM");
	hSM->SetTitle("Weighted Diboson Mass");
	hSM->GetXaxis()->SetTitle("MVV (GeV)");
	hSM->GetYaxis()->SetTitle("Number of Events");

	parsingStr=to_string(lumiWeight600To800*yieldSM600To800/yield600To800)+"*totWeight*aTGCWeights[61]";
	tree600To800->SetLineColor(2);
	tree600To800->Draw("MWW>>h600To800",parsingStr.c_str(),"same hist");

	parsingStr=to_string(lumiWeight800ToInf*yieldSM800ToInf/yield800ToInf)+"*totWeight*aTGCWeights[61]";
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
	
	calculateFactor(prefix+"WW_mu.root", 49.997, prefix+"WW-aTGC_MWW-600To800_mu.root", 0.1833, prefix+"WW-aTGC_MWW-800ToInf_mu.root", 0.2366);
	calculateFactor(prefix+"WZ_mu.root", 11.46, prefix+"WZ-aTGC_MWZ-600To800_mu.root", 0.06493, prefix+"WZ-aTGC_MWZ-800ToInf_mu.root", 0.1012);
	calculateFactor(prefix+"WW_ele.root", 49.997, prefix+"WW-aTGC_MWW-600To800_ele.root", 0.1833, prefix+"WW-aTGC_MWW-800ToInf_ele.root", 0.2366);
	calculateFactor(prefix+"WZ_ele.root", 11.46, prefix+"WZ-aTGC_MWZ-600To800_ele.root", 0.06493, prefix+"WZ-aTGC_MWZ-800ToInf_ele.root", 0.1012);
}
