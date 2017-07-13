// -*- C++ -*-
//
// Package:    aTGCsAnalysis/GenBraFracAnalyzer
// Class:      GenBraFracAnalyzer
// 
/**\class GenBraFracAnalyzer GenBraFracAnalyzer.cc aTGCsAnalysis/GenBraFracAnalyzer/plugins/GenBraFracAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Muhammad Ansar Iqbal
//         Created:  Wed, 12 Jul 2017 11:28:59 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "TLorentzVector.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"
#include "TFile.h"

using namespace edm;
using namespace std;
using namespace reco;

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class GenBraFracAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit GenBraFracAnalyzer(const edm::ParameterSet&);
      ~GenBraFracAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      virtual bool decaysHadronic(const GenParticle*);

      TTree* tree;
      ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double>> hadronicVp4, leptonicVp4, dibosonp4;
      double ptV, mWV;
      bool inSelRegA, inSelRegB;
      int totalEvents, selectedEventsRegA, selectedEventsRegB;
      EDGetTokenT<View<GenParticle>> src;

      // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
GenBraFracAnalyzer::GenBraFracAnalyzer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
   usesResource("TFileService");
   Service<TFileService> fs;
   tree=fs->make<TTree>("EventInfo","EventInfo");
   tree->Branch("ptV", &ptV, "ptV/D");
   tree->Branch("mWV", &mWV, "mWV/D");
   tree->Branch("inSelRegA", &inSelRegA, "inSelRegA/B");
   tree->Branch("inSelRegB", &inSelRegB, "inSelRegB/B");

   ptV=0;
   mWV=0;
   inSelRegA=false;
   inSelRegB=false;
   totalEvents=0;
   selectedEventsRegA=0;
   selectedEventsRegB=0;
   src = consumes<View<GenParticle>>(iConfig.getParameter<InputTag>("src"));
}


GenBraFracAnalyzer::~GenBraFracAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
GenBraFracAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   ptV=0;
   mWV=0;
   inSelRegA=false;
   inSelRegB=false;
 
   Handle<View<GenParticle>> genParticles;
   iEvent.getByToken(src, genParticles);

   // Scan the list of gen particles, isolate the starting W or Z, identify the hadronically decaying one and store its pt.
   for(size_t i=0; i<genParticles->size(); i++)
   {
	const GenParticle & p = (*genParticles)[i];
	if((fabs(p.pdgId())==23 || fabs(p.pdgId())==24) && p.status()==22)
	{
		if(decaysHadronic(&p))
		{
			ptV=p.pt();
			
			hadronicVp4.SetPt(p.pt());
			hadronicVp4.SetEta(p.eta());
			hadronicVp4.SetPhi(p.phi());
			hadronicVp4.SetM(p.mass());
			//cout<<"*Hadronically decaying W* "<<hadronicVp4.px()<<" "<<hadronicVp4.py()<<" "<<hadronicVp4.pz()<<" "<<hadronicVp4.e()<<" "<<hadronicVp4.M()<<endl;
		}
		else
		{
			leptonicVp4.SetPt(p.pt());
			leptonicVp4.SetEta(p.eta());
			leptonicVp4.SetPhi(p.phi());
			leptonicVp4.SetM(p.mass());
			//cout<<"*Leptonically decaying W* "<<leptonicVp4.px()<<" "<<leptonicVp4.py()<<" "<<leptonicVp4.pz()<<" "<<leptonicVp4.e()<<" "<<leptonicVp4.M()<<endl;
		}
	}
   }

   dibosonp4=hadronicVp4+leptonicVp4;
   mWV=dibosonp4.M();
   //cout<<"Diboson mass: "<<mWV<<endl<<endl;

   totalEvents++;
   if(ptV>180 && mWV>600 && mWV<800)
   {
	selectedEventsRegA++;
	inSelRegA=true;
   }
   else if(ptV>180 && mWV>800)
   {
        selectedEventsRegB++;
        inSelRegB=true;
   }

   tree->Fill();

#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}

bool GenBraFracAnalyzer::decaysHadronic(const GenParticle* p)
{
   if(p!=NULL)
   {	
	//cout<<p->pdgId()<<" ";
        if(abs(p->pdgId())<7 || abs(p->pdgId())==21) return true;
        else
        {
		for(size_t i=0; i<p->numberOfDaughters(); ++i)
		{
                	const GenParticle* d= (GenParticle*)p->daughter(i);
                	if(decaysHadronic(d)) return true;
		}
        }
   }
   return false;	
}

// ------------ method called once each job just before starting event loop  ------------
void 
GenBraFracAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
GenBraFracAnalyzer::endJob() 
{
   cout<<endl<<"Total events in the file: "<<totalEvents<<endl;
   cout<<"Events with diboson mass in [600, 800] GeV: "<<selectedEventsRegA<<endl;
   cout<<"Required branching fraction for diboson mass in [600, 800]: "<<(float)selectedEventsRegA/(float)totalEvents<<endl;
   cout<<"Events with diboson mass > 800 GeV: "<<selectedEventsRegB<<endl;
   cout<<"Required branching fraction for diboson mass > 800 GeV: "<<(float)selectedEventsRegB/(float)totalEvents<<endl;
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
GenBraFracAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(GenBraFracAnalyzer);
