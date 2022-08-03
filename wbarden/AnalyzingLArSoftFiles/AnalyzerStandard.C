// /pnfs/uboone/data/uboone/reconstructed/prod_v08_00_00_18/data_extbnb_mcc9.1_v08_00_00_18/run1_reco2_C1/00/00/69/58/PhysicsRun-2016_7_24_11_54_40-0006958-00040_20160724T214243_ext_bnb_20160724T234647_merged_20181107T121731_optfilter_20181224T075125_reco1_postwcct_postdl_20181224T082906_20190723T182808_reco2.root


/// Psuedo code:
/*

  Find all the T0 from acpttrigtagger
  
  then grab the tracks that have an association through acpttrigtagger
  
  using these tracks get the associated caloritmetry data product

  then got through all the calo's trajectory points, mark it's x, then find the associated hit and store it's attributes 
  

 */


// Standard things to include
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

#include <fstream>
#include <iterator>
#include <algorithm>
#include <math.h> 
// These are the includes to use "Root" things 
#include "TInterpreter.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TTree.h"
#include "TFile.h"
#include "TLorentzVector.h"

// These are the larsoft includes that let you
// have access to data-products and the event 
// details
#include "canvas/Utilities/InputTag.h"
#include "gallery/Event.h"
//#include "gallery/Event.h"
#include "gallery/ValidHandle.h"
#include "gallery/Handle.h"
#include "canvas/Persistency/Common/FindMany.h"
#include "canvas/Persistency/Common/FindOne.h"
#include "canvas/Persistency/Common/FindManyP.h"
#include "canvas/Persistency/Common/fwd.h"
#include "canvas/Persistency/Common/Ptr.h"
#include "canvas/Persistency/Provenance/Timestamp.h"

//I'll need, calo, tracks, hits, anab::T0

#include "lardataobj/RecoBase/Hit.h"
#include "canvas/Persistency/Provenance/EventAuxiliary.h"
#include "lardataobj/Simulation/SimEnergyDeposit.h"
#include "nusimdata/SimulationBase/MCParticle.h"
#include "lardataobj/RecoBase/Wire.h"



//This way you can be lazy
using namespace art;
using namespace std;

void AnalyzerStandard(){
  
  // create a vector of files we want to process
  std::vector<string> filenames;
  
  // read in a file list that we get from a sam-def but remember it 
  // is very long so if we want to run over it all it'll take a while
  // and we'll probably want to break it up on the grid

  ifstream myfile("list_of_standard_electrons.list");
  copy(istream_iterator<string>(myfile),
       istream_iterator<string>(),
       back_inserter(filenames));
 
  //We'll just check the first 25 files for now
  filenames.erase(filenames.begin()+25,filenames.end());
  
  std::cout << "File Count: " << filenames.size() << std::endl; 
  
  
  // Here we will create all of our histograms 
  // I did this crazy inefficiently but I don't really care
  // This is currently only set up for single dimensional 
  // projections but extenting it to 2D will be straight forward
  TFile* out = new TFile("output.root","RECREATE");  
  TTree* fTree = new TTree("EventTree","tree");
  int Run;
  fTree->Branch("Run",&Run);
  int Subrun;
  fTree->Branch("Subrun",&Subrun);
  int Event;
  fTree->Branch("Event",&Event);
  std::vector<double>  true_x;
  fTree->Branch("true_x",&true_x);
  std::vector<double>  true_y;
  fTree->Branch("true_y",&true_y);
  std::vector<double>  true_z;
  fTree->Branch("true_z",&true_z);
  std::vector<double>  true_E;
  fTree->Branch("true_E",&true_E);
  std::vector<double>  true_Edep;
  fTree->Branch("true_Edep",&true_Edep);
  std::vector<int>  true_Q;
  fTree->Branch("true_Q",&true_Q);
  std::vector<int>  true_L;
  fTree->Branch("true_L",&true_L);

  std::vector<int>  true_PDGCode;
  fTree->Branch("true_PDGCode",&true_PDGCode);
  std::vector<int>  true_trackID;
  fTree->Branch("true_trackID",&true_trackID);
  std::vector<int>  edep_trackID;
  fTree->Branch("edep_trackID",&edep_trackID);
 
  std::vector< std::vector<double> > true_dx;
  std::vector< std::vector<int> > true_dQ;
  std::vector< std::vector<int> > true_dL;
  std::vector< std::vector<double> > true_dE;
  fTree->Branch("true_dx",&true_dx);
  fTree->Branch("true_dQ",&true_dQ);
  fTree->Branch("true_dL",&true_dL);
  fTree->Branch("true_dE",&true_dE); 
  

  std::vector<double> reco_matched_trkID;
  fTree->Branch("reco_matched_trkID",&reco_matched_trkID);

  std::vector<double> reco_matched_PDG;
  fTree->Branch("reco_matched_PDG",&reco_matched_PDG);

  std::vector<double> reco_tot_hit_Q;
  fTree->Branch("reco_tot_hit_Q",&reco_tot_hit_Q);

  std::vector< std::vector<double> > reco_hit_Q;
  fTree->Branch("reco_hit_Q",&reco_hit_Q);

  std::vector< std::vector<double> > reco_matched_E;
  fTree->Branch("reco_matched_E",&reco_matched_E);
  
  std::vector< double > reco_tot_wire_Q;
  fTree->Branch("reco_tot_wire_Q",&reco_tot_wire_Q);

  std::vector< std::vector< double > > reco_wire_Q;
  fTree->Branch("reco_wire_Q",&reco_wire_Q);
  
  // gallery makes it easy to just hand a vector of files

  for (gallery::Event ev(filenames) ; !ev.atEnd(); ev.next()) {
    
    /// Prep our Branches
    Run = 0;
    Subrun = 0;
    Event = 0;
    true_x.clear();
    true_y.clear();
    true_z.clear();
    true_E.clear();
    true_PDGCode.clear();
    true_trackID.clear();
    edep_trackID.clear();

    true_Edep.clear();
    true_Q.clear();
    true_L.clear();
    
    true_dx.clear();
    true_dQ.clear();
    true_dL.clear();
    true_dE.clear();

    reco_matched_trkID.clear();
    reco_matched_PDG.clear();
    reco_tot_hit_Q.clear();
    reco_hit_Q.clear();
    reco_matched_E.clear();
    reco_tot_wire_Q.clear();
    reco_wire_Q.clear();

    Run = ev.eventAuxiliary().run();
    Subrun = ev.eventAuxiliary().subRun();
    Event = ev.eventAuxiliary().event();
    


    // This is the truth particles
    auto const &mcpart_handle =
      ev.getValidHandle< std::vector<simb::MCParticle> >("largeant");

    auto const &simedep_handle = 
      ev.getValidHandle< std::vector<sim::SimEnergyDeposit> >("ionandscint:priorSCE");


     //This
     //auto const & psedepListHandle
    //  = ev.getValidHandle< std::vector<sim::SimEnergyDeposit> >("psdope:psdoped");
    
    //     auto psedeps(*psedepListHandle);
     
     // OR this
     std::vector<sim::SimEnergyDeposit> psedeps;

    // This is a reconstruction of charge on a single channel 
    auto const &hit_handle =
      ev.getValidHandle< std::vector<recob::Hit> >("gaushit");
        
    auto const &wire_handle = 
      ev.getValidHandle< std::vector<recob::Wire> >("caldata");

    art::FindMany<simb::MCParticle, anab::BackTrackerHitMatchingData> parts_per_hit(hit_handle,ev,"gaushitTruthMatch");

    art::FindManyP<recob::Wire> wires_per_hit(hit_handle,ev,"gaushit");
    
    //    auto all_wires(*wire_handle);
    auto simedeps(*simedep_handle);
    auto all_mcparts(*mcpart_handle);
    auto gaus_hits(*hit_handle);

    for(auto mcp : all_mcparts){
      
      auto it = std::find(true_trackID.begin(), 
			  true_trackID.end(), 
			  abs(mcp.TrackId()));
      
      if (it != true_trackID.end()){
      
	int k = std::distance(true_trackID.begin(), it);
	
	true_x[k] = mcp.Position(0).X();
	true_y[k] = mcp.Position(0).Y();
	true_z[k] = mcp.Position(0).Z();
	true_E[k] = mcp.E(0);
	true_PDGCode[k] = mcp.PdgCode();
	true_trackID[k] = abs(mcp.TrackId());		
		
      }
      else{

	true_x.push_back(mcp.Position(0).X());
	true_y.push_back(mcp.Position(0).Y());
	true_z.push_back(mcp.Position(0).Z());
	true_E.push_back(mcp.E(0));
	true_PDGCode.push_back(mcp.PdgCode());
	true_trackID.push_back(abs(mcp.TrackId()));

      }

    }
       
    if(psedeps.size() != 0){//With PSDopants
      for(auto edep : simedeps){
	
	auto it = std::find(edep_trackID.begin(), 
			    edep_trackID.end(), 
			    abs(edep.TrackID()));
	
	int k = 0;

        if (it == edep_trackID.end()){
          edep_trackID.push_back(abs(edep.TrackID()));
          true_Edep.push_back(0);
	  true_Q.push_back(0);
	  true_L.push_back(0);
	  true_dE.resize(edep_trackID.size());
	  
          k = edep_trackID.size() - 1;
        }
        else{
          k = std::distance(edep_trackID.begin(), it);
        }
	
	true_Edep[k] += edep.Energy();       
	true_dE[k].push_back(edep.Energy());
      
      }

      true_dx.resize(edep_trackID.size());
      true_dQ.resize(edep_trackID.size());
      true_dL.resize(edep_trackID.size());

      for(auto psdep : psedeps){
	
	auto it = std::find(edep_trackID.begin(), 
			    edep_trackID.end(), 
			    abs(psdep.TrackID()));

	if(it == edep_trackID.end()){
	  std::cout << "psdopants found a track ID not in edeps!" << std::endl;
	}
	else{
	  int k = std::distance(edep_trackID.begin(), it);
		
	  true_Q[k] += psdep.NumElectrons();
	  true_L[k] += psdep.NumPhotons();
	  true_dx[k].push_back(psdep.StepLength());
	  true_dQ[k].push_back(psdep.NumElectrons());
	  true_dL[k].push_back(psdep.NumPhotons());	  
	}

      }//it over psdeps

    }//With PSDopants
    else{//NO PSDopants
      for(auto edep : simedeps){
	
	auto it = std::find(edep_trackID.begin(), 
			    edep_trackID.end(), 
			    abs(edep.TrackID()));
	int k = 0;
	
	if (it == edep_trackID.end()){
          edep_trackID.push_back(abs(edep.TrackID()));
	  true_Edep.push_back(0);	  	  
	  true_Q.push_back(0);
	  true_L.push_back(0);
	  
	  true_dx.resize(edep_trackID.size());
	  true_dQ.resize(edep_trackID.size());
	  true_dL.resize(edep_trackID.size());
	  true_dE.resize(edep_trackID.size());
	  
	  k = edep_trackID.size() - 1;
	}
	else{
	  k = std::distance(edep_trackID.begin(), it);
	}

	true_Edep[k] += edep.Energy();	
	true_Q[k] += edep.NumElectrons();
        true_L[k] += edep.NumPhotons();
	  
	true_dx[k].push_back(edep.StepLength());
	true_dQ[k].push_back(edep.NumElectrons());
	true_dL[k].push_back(edep.NumPhotons());
	true_dE[k].push_back(edep.Energy());	      
      }
    }//No PSDopants
        
    reco_matched_trkID.resize(true_trackID.size());
    reco_matched_PDG.resize(true_trackID.size());
    reco_tot_hit_Q.resize(true_trackID.size());
    reco_hit_Q.resize(true_trackID.size());
    reco_matched_E.resize(true_trackID.size());


    reco_tot_wire_Q.resize(true_trackID.size());
    reco_wire_Q.resize(true_trackID.size());
    
 
    std::fill(reco_tot_hit_Q.begin(), reco_tot_hit_Q.end(), 0);
    std::fill(reco_tot_wire_Q.begin(), reco_tot_wire_Q.end(), 0);    
       
    for(int h = 0; h < gaus_hits.size(); h++){
      
      auto hit = gaus_hits.at(h);
      
      if(hit.View() != 2){ continue;}
      
      std::vector<simb::MCParticle const*> parts_in_my_hit;
      std::vector<anab::BackTrackerHitMatchingData const*> partInfo_in_my_hit;
      parts_per_hit.get(h, parts_in_my_hit, partInfo_in_my_hit);
      
      for(int mcp = 0; mcp < parts_in_my_hit.size(); mcp++){

	auto mcpart = parts_in_my_hit.at(mcp);
	
	auto it = std::find(true_trackID.begin(),
                            true_trackID.end(),
                            abs(mcpart->TrackId()));
	
	int k = std::distance(true_trackID.begin(), it);
	
	reco_matched_trkID[k] = mcpart->TrackId();
	reco_matched_PDG[k] = mcpart->PdgCode();

	reco_tot_hit_Q[k] += hit.Integral();
	reco_hit_Q[k].push_back(hit.Integral());
	reco_matched_E[k].push_back(partInfo_in_my_hit.at(mcp)->energy);
      	
	auto wires = wires_per_hit.at(h);
	/*
	for(auto wire_const : wires){
		 
	  recob::Wire wire = const_cast<recob::Wire>(wire_const);
	*/

	for(int wr = 0; wr < wires.size(); wr++){
		 
	  auto signals = wires[wr]->Signal();
	
	  int tick = 0;
	  for(int s = hit.StartTick(); s <= hit.EndTick(); s++){
	    if(s >= hit.StartTick() && s <= hit.EndTick()){
	      reco_tot_wire_Q[k] += signals[s];
	      reco_wire_Q[k].push_back(signals[s]);
	    }//overlap with hit
	  }//iterate over all matched ticks
	  	  
	}// Iterate over recob::Wires Matched to Hits
      }//Iterate over MCP matched to hits
    }//Iterate over hits
     
    fTree->Fill();

  }//Iterate over Events
  out->cd();  
  fTree->Write();
  
  
  
}//Close Program


