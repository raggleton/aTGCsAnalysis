// -*- C++ -*-
//
// Package:    BTaggingEffAnalyzer
// Class:      BTaggingEffAnalyzer
// 
/**\class BTaggingEffAnalyzer BTaggingEffAnalyzer.cc Analysis/EDSHyFT/plugins/BTaggingEffAnalyzer.cc
 Description: [one line class summary]
 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Dinko Ferencek
//         Created:  Thu Oct  4 20:25:54 CDT 2012
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TEfficiency.h"
#include "TLorentzVector.h"

#include "../../TreeMaker/plugins/ScaleFactorHelper.h"

namespace reco {
  typedef edm::Ptr<reco::Muon> MuonPtr;
  typedef edm::Ptr<reco::GsfElectron> ElectronPtr;
}

typedef std::vector<pat::Jet> PatJetCollection;

//
// class declaration
//

class BTaggingEffAnalyzer : public edm::EDAnalyzer {
   public:
      explicit BTaggingEffAnalyzer(const edm::ParameterSet&);
      ~BTaggingEffAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

      // ----------member data ---------------------------
      edm::EDGetTokenT<PatJetCollection> JetsToken;
      const edm::InputTag jetsTag;
      const std::string   discriminatorTag;
      const double  discriminatorValue;

      const std::vector<double>  ptBinning;
      const std::vector<double>  etaBinning;

      edm::Service<TFileService>  fs;
      TEfficiency  *BTaggingEff_b;
      TEfficiency  *BTaggingEff_c;
      TEfficiency  *BTaggingEff_udsg;

      std::string channel;
      const double metCut, leptonPtCut, wPtCut, mwwCut;
      edm::EDGetTokenT<edm::View<pat::Jet>> fatJetsToken_;
      edm::EDGetTokenT<edm::View<pat::MET> > metToken_;
      edm::EDGetTokenT<edm::View<reco::Candidate>> leptonicVToken_;
      edm::EDGetTokenT<edm::View<reco::Vertex> > vertexToken_;
      edm::EDGetTokenT<edm::View<reco::Candidate>> leptonsToken_;

      MuonScaleFactor MuonScaleFactor_;
      ElectronScaleFactor ElectronScaleFactor_;
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
BTaggingEffAnalyzer::BTaggingEffAnalyzer(const edm::ParameterSet& iConfig) :
  JetsToken(consumes<std::vector<pat::Jet>>(iConfig.getParameter<edm::InputTag>("JetsTag"))),
  discriminatorTag(iConfig.getParameter<std::string>("DiscriminatorTag")),
  discriminatorValue(iConfig.getParameter<double>("DiscriminatorValue")),
  ptBinning(iConfig.getParameter<std::vector<double>>("ptBinning")),
  etaBinning(iConfig.getParameter<std::vector<double>>("etaBinning")),
  channel(iConfig.getParameter<std::string>("channel")),
  metCut(iConfig.getParameter<double>("METCut")),
  leptonPtCut(iConfig.getParameter<double>("leptonPtCut")),
  wPtCut(iConfig.getParameter<double>("WPtCut")),
  mwwCut(iConfig.getParameter<double>("MWWCut")),
  fatJetsToken_(consumes<edm::View<pat::Jet>>(iConfig.getParameter<edm::InputTag>("fatJetSrc"))),
  metToken_(consumes<edm::View<pat::MET>>(iConfig.getParameter<edm::InputTag>("metSrc"))),
  leptonicVToken_(consumes<edm::View<reco::Candidate>>(iConfig.getParameter<edm::InputTag>("leptonicVSrc"))),
  vertexToken_(consumes<edm::View<reco::Vertex>>(iConfig.getParameter<edm::InputTag>("vertexSrc"))),
  leptonsToken_(consumes<edm::View<reco::Candidate>>(iConfig.getParameter<edm::InputTag>("leptonSrc"))),
  MuonScaleFactor_(
    "aTGCsAnalysis/TreeMaker/data/Muon_TrackingSF_RunBtoH.root",
    "aTGCsAnalysis/TreeMaker/data/Muon_IDSF_average_RunBtoH.root",
    "aTGCsAnalysis/TreeMaker/data/Muon_IsoSF_average_RunBtoH.root",
    "aTGCsAnalysis/TreeMaker/data/Muon_SingleLeptonTriggerSF_average_RunBtoH.root",
    "HighPtID",
    "tkLooseISO"
  ),
  ElectronScaleFactor_(
    "aTGCsAnalysis/TreeMaker/data/HEEP_SF.root",
    "aTGCsAnalysis/TreeMaker/data/egammaEffi.txt_EGM2D.root"
  )
{
  if ((channel != "mu") && (channel != "el")) {
    throw cms::Exception("InvalidValue") << "Invalid value for channel parameter, should be mu or el." << std::endl;
  }

   //now do what ever initialization is needed
   BTaggingEff_b    = fs->make<TEfficiency>("BTaggingEff_b", ";p_{T} (GeV);#eta", ptBinning.size()-1, &ptBinning[0], etaBinning.size()-1, &etaBinning[0]);
   BTaggingEff_c    = fs->make<TEfficiency>("BTaggingEff_c", ";p_{T} (GeV);#eta", ptBinning.size()-1, &ptBinning[0], etaBinning.size()-1, &etaBinning[0]);
   BTaggingEff_udsg = fs->make<TEfficiency>("BTaggingEff_udsg", ";p_{T} (GeV);#eta", ptBinning.size()-1, &ptBinning[0], etaBinning.size()-1, &etaBinning[0]);
   BTaggingEff_b    -> SetStatisticOption(TEfficiency::kFCP);
   BTaggingEff_c    -> SetStatisticOption(TEfficiency::kFCP);
   BTaggingEff_udsg -> SetStatisticOption(TEfficiency::kFCP);

   
}


BTaggingEffAnalyzer::~BTaggingEffAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
BTaggingEffAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::Handle<PatJetCollection> jets;
  iEvent.getByToken(JetsToken,jets);

  edm::Handle<edm::View<pat::Jet> > fatjets;
  iEvent.getByToken(fatJetsToken_, fatjets);

  edm::Handle<edm::View<reco::Candidate> > leptons;
  iEvent.getByToken(leptonsToken_, leptons);

  edm::Handle<edm::View<reco::Vertex> > vertices;
  iEvent.getByToken(vertexToken_, vertices);

  edm::Handle<edm::View<reco::Candidate> > leptonicVs;
  iEvent.getByToken(leptonicVToken_, leptonicVs);

  edm::Handle<edm::View<pat::MET> > metHandle;
  iEvent.getByToken(metToken_, metHandle);

  // Get selection vars
  double Wboson_lep_pt(-99.), Wboson_lep_eta(-99.), Wboson_lep_phi(-99.), Wboson_lep_mass(-99.);
  if (leptonicVs->size() > 0) {
    const reco::Candidate  & leptonicV = leptonicVs->at(0);
    Wboson_lep_pt = leptonicV.pt();
    Wboson_lep_eta = leptonicV.eta();
    Wboson_lep_phi = leptonicV.phi();
    Wboson_lep_mass = leptonicV.mass();
  } else {
    return;
  }

  double METCand_pt(-99.), METCand_phi(-99.);
  if (metHandle->size() > 0) {
    const pat::MET& metCand = metHandle->at(0);
    METCand_pt = metCand.pt();
    METCand_phi = metCand.phi();
  } else {
    return;
  }

  // Get lepton SF
  double lepton_pt(-99.), lepton_eta(-99.), lepton_phi(-99.);
  double LeptonSF = 1.;
  auto leptonPtr = leptons->ptrAt(0);
  if (channel == "mu"){
    // Muon scale factors apply cuts on their nVtx
    int goodNPV = 0;
    for (const auto & itr : *vertices) {
      if (fabs(itr.z()) <= 25 && itr.ndof() > 4 && fabs(itr.position().rho()) <= 2 && !itr.isFake()) {
        goodNPV++;
      }
    }
    reco::MuonPtr asmuonPtr(leptonPtr);
    lepton_pt = asmuonPtr->tunePMuonBestTrack()->pt();
    lepton_eta = asmuonPtr->tunePMuonBestTrack()->eta();
    lepton_phi = asmuonPtr->tunePMuonBestTrack()->phi();
    LeptonSF = MuonScaleFactor_.getScaleFactor(lepton_pt, std::abs(lepton_eta), lepton_phi, goodNPV);
  } else if (channel == "el") {
    reco::ElectronPtr aselectronPtr(leptonPtr);
    lepton_pt = (leptons -> at(0)).pt();
    lepton_eta = (leptons -> at(0)).eta();
    lepton_phi = (leptons -> at(0)).phi();
    reco::SuperClusterRef  superCluster = aselectronPtr->superCluster();
    double sc_eta = superCluster->eta();
    LeptonSF = ElectronScaleFactor_.getScaleFactor(lepton_pt, lepton_eta, sc_eta, lepton_phi, vertices->size());
  }

  double deltaR_LeptonWJet(-99.), deltaPhi_WJetMet(-99.), deltaPhi_WJetWlep(-99.);

  pat::Jet VJet = (fatjets->at(0));
  deltaR_LeptonWJet = deltaR(lepton_eta, lepton_phi, VJet.eta(), VJet.phi());
  deltaPhi_WJetMet = deltaPhi(VJet.phi(), METCand_phi);
  deltaPhi_WJetWlep = deltaPhi(VJet.phi(), Wboson_lep_phi);

  ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > hadronicVp4, leptonicVp4, lvj_p4;
  //hadronic W
  hadronicVp4.SetPt(VJet.pt());
  hadronicVp4.SetEta(VJet.eta());
  hadronicVp4.SetPhi(VJet.phi());
  hadronicVp4.SetM(VJet.mass());
  //leptonic W
  leptonicVp4.SetPt(Wboson_lep_pt);
  leptonicVp4.SetEta(Wboson_lep_eta);
  leptonicVp4.SetPhi(Wboson_lep_phi);
  leptonicVp4.SetM(Wboson_lep_mass);
  lvj_p4 = hadronicVp4 + leptonicVp4;
  double MWW = lvj_p4.M();

  // std::cout << "MET: " << METCand_pt << std::endl;
  // std::cout << "leptonpt: " << lepton_pt << std::endl;
  // std::cout << "Wpt: " << Wboson_lep_pt << std::endl;
  // std::cout << "deltaR_LeptonWJet: " << fabs(deltaR_LeptonWJet) << std::endl;
  // std::cout << "deltaPhi_WJetMet: " << fabs(deltaPhi_WJetMet) << std::endl;
  // std::cout << "deltaPhi_WJetWlep: " << fabs(deltaPhi_WJetWlep) << std::endl;
  // std::cout << "MWW: " << MWW << std::endl;

  // Do selection cuts here
  if (!(METCand_pt > metCut
        && lepton_pt > leptonPtCut
        && Wboson_lep_pt > wPtCut
        && fabs(deltaR_LeptonWJet) > M_PI/2.
        && fabs(deltaPhi_WJetMet) > 2.
        && fabs(deltaPhi_WJetWlep) > 2.
        && MWW > mwwCut)) {
    return;
  }

  // loop over jets
  for(PatJetCollection::const_iterator it = jets->begin(); it != jets->end(); ++it)
  {
    uint flav = abs(it->hadronFlavour());
    bool passed = (it->bDiscriminator(discriminatorTag.c_str()) >= discriminatorValue);

    if(flav==5)
    {
      // BTaggingEff_b->Fill(passed,it->pt(), it->eta());
      BTaggingEff_b->FillWeighted(passed, LeptonSF, it->pt(), it->eta());
    }
    else if(flav==4)
    {
      // BTaggingEff_c->Fill(passed,it->pt(), it->eta());
      BTaggingEff_c->FillWeighted(passed, LeptonSF, it->pt(), it->eta());
    }
    else
    {
      // BTaggingEff_udsg->Fill(passed,it->pt(), it->eta());
      BTaggingEff_udsg->FillWeighted(passed, LeptonSF, it->pt(), it->eta());
    }
  }
}


// ------------ method called once each job just before starting event loop  ------------
void 
BTaggingEffAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
BTaggingEffAnalyzer::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
void 
BTaggingEffAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
BTaggingEffAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
BTaggingEffAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
BTaggingEffAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
BTaggingEffAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(BTaggingEffAnalyzer);