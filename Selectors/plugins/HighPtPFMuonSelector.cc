// -*- C++ -*-
//
// Package:    MuonSelector/HighPtPFMuonSelector
// Class:      HighPtPFMuonSelector
// 
/**\class HighPtPFMuonSelector HighPtPFMuonSelector.cc MuonSelector/HighPtPFMuonSelector/plugins/HighPtPFMuonSelector.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Ivan Shvetsov
//         Created:  Fri, 24 Oct 2014 13:19:57 GMT
//
//


// system include files
#include <memory>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/ShallowCloneCandidate.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/CompositeCandidateFwd.h"
#include "CommonTools/CandUtils/interface/AddFourMomenta.h"
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

//
// class declaration
//

class HighPtPFMuonSelector : public edm::EDProducer {
   public:
      explicit HighPtPFMuonSelector(const edm::ParameterSet&);
      ~HighPtPFMuonSelector();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual void produce(edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      
      edm::EDGetTokenT<edm::View<pat::Muon> > MuonToken_;
      edm::EDGetTokenT<edm::View<reco::Vertex> > VertexToken_;
      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

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
HighPtPFMuonSelector::HighPtPFMuonSelector(const edm::ParameterSet& iConfig):
    MuonToken_ (consumes<edm::View<pat::Muon> > (iConfig.getParameter<edm::InputTag>( "src" ) ) ),
    VertexToken_ (consumes<edm::View<reco::Vertex> > (iConfig.getParameter<edm::InputTag>( "vertex" ) ) )
{
  produces<std::vector<pat::Muon>>();
  //register your products
/* Examples
   produces<ExampleData2>();

   //if do put with a label
   produces<ExampleData2>("label");
 
   //if you want to put into the Run
   produces<ExampleData2,InRun>();
*/
   //now do what ever other initialization is needed
  
}


HighPtPFMuonSelector::~HighPtPFMuonSelector()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
HighPtPFMuonSelector::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   
   Handle <edm::View <pat::Muon> > muons;
   Handle <edm::View <reco::Vertex> > Vertices;
   
   std::auto_ptr<std::vector<pat::Muon> > OutMuonCollection(new std::vector<pat::Muon>);
     
   iEvent.getByToken(MuonToken_, muons);
   iEvent.getByToken(VertexToken_, Vertices);
   
  // std::cout << muons -> size() << std::endl;
   
   for (unsigned int iMuon = 0; iMuon < muons -> size(); iMuon ++)
   {

      if ((muon::isHighPtMuon(muons -> at(iMuon), Vertices -> at(0))) && (muons -> at(iMuon)).isPFMuon()) OutMuonCollection -> push_back( muons -> at(iMuon));
   }
   
//   std::cout << OutMuonCollection -> size() << std::endl;
   iEvent.put(OutMuonCollection);
 //  std::cout << " Vertices : " << Vertices -> at(0) << std::endl;
   //std::cout <<  isTightMuon() << std::endl;
   
/* This is an event example
   //Read 'ExampleData' from the Event
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);

   //Use the ExampleData to create an ExampleData2 which 
   // is put into the Event
   std::auto_ptr<ExampleData2> pOut(new ExampleData2(*pIn));
   iEvent.put(pOut);
*/

/* this is an EventSetup example
   //Read SetupData from the SetupRecord in the EventSetup
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
*/
 
}

// ------------ method called once each job just before starting event loop  ------------
void 
HighPtPFMuonSelector::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HighPtPFMuonSelector::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
HighPtPFMuonSelector::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
HighPtPFMuonSelector::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
HighPtPFMuonSelector::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
HighPtPFMuonSelector::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
HighPtPFMuonSelector::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(HighPtPFMuonSelector);
