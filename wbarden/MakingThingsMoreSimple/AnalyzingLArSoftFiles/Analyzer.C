
/// Psuedo code:
/*

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

//

#include "lardataobj/RecoBase/Hit.h"
#include "canvas/Persistency/Provenance/EventAuxiliary.h"
#include "lardataobj/Simulation/SimEnergyDeposit.h"
#include "nusimdata/SimulationBase/MCParticle.h"
#include "nusimdata/SimulationBase/MCTruth.h"
#include "lardataobj/RecoBase/Wire.h"



//This way you can be lazy
using namespace art;
using namespace std;

void Analyzer(string input){
  
  // create a vector of files we want to process
  std::vector<string> filenames;
  
  // read in a file list that we get from a sam-def but remember it 
  // is very long so if we want to run over it all it'll take a while
  // and we'll probably want to break it up on the grid

  //  ifstream myfile("../../FileLists/list_of_monoenergetic_2.5MeV_electrons_undoped.list");
  ifstream myfile(Form("../../FileLists/list_of_%s.list",input.c_str()));
  copy(istream_iterator<string>(myfile),
       istream_iterator<string>(),
       back_inserter(filenames));
 
  //We'll just check the first 25 files for now
  //filenames.erase(filenames.begin()+250,filenames.end());
  
  std::cout << "File Count: " << filenames.size() << std::endl; 
    
  // Here we will create all of our histograms 
  // I did this crazy inefficiently but I don't really care
  // This is currently only set up for single dimensional 
  // projections but extenting it to 2D will be straight forward
  TFile* out = new TFile(Form("%s.root",input.c_str()),"RECREATE");  
  TTree* fTree = new TTree("EventTree","tree");
  int Run;
  fTree->Branch("Run",&Run);
  int Subrun;
  fTree->Branch("Subrun",&Subrun);
  int Event;
  fTree->Branch("Event",&Event);
  double  true_x;
  fTree->Branch("true_x",&true_x);
  double  true_y;
  fTree->Branch("true_y",&true_y);
  double  true_z;
  fTree->Branch("true_z",&true_z);
  double  true_E;
  fTree->Branch("true_E",&true_E);
  double  true_Edep;
  fTree->Branch("true_Edep",&true_Edep);
  int  true_Q;
  fTree->Branch("true_Q",&true_Q);
  int  true_L;
  fTree->Branch("true_L",&true_L);

  double reco_tot_hit_Q;
  fTree->Branch("reco_tot_hit_Q",&reco_tot_hit_Q);

  double reco_N_hit;
  fTree->Branch("reco_N_hit",&reco_N_hit);
  
  double reco_mean_hit_Q;
  fTree->Branch("reco_mean_hit_Q",&reco_mean_hit_Q);
  
  double reco_min_hit_Q;
  fTree->Branch("reco_min_hit_Q",&reco_min_hit_Q);

  double reco_max_hit_Q;
  fTree->Branch("reco_max_hit_Q",&reco_max_hit_Q);

  double reco_tot_matched_E;
  fTree->Branch("reco_tot_matched_E",&reco_tot_matched_E);
  
  double reco_tot_wire_Q;
  fTree->Branch("reco_tot_wire_Q",&reco_tot_wire_Q);

  double reco_mean_wire_Q;
  fTree->Branch("reco_mean_wire_Q",&reco_mean_wire_Q);
  
  double reco_min_wire_Q;
  fTree->Branch("reco_min_wire_Q",&reco_min_wire_Q);

  double reco_max_wire_Q;
  fTree->Branch("reco_max_wire_Q",&reco_max_wire_Q);

  double reco_N_wire;
  fTree->Branch("reco_N_wire",&reco_N_wire);

  // gallery makes it easy to just hand a vector of files

  for (gallery::Event ev(filenames) ; !ev.atEnd(); ev.next()) {
    
    /// Prep our Branches
    Run = 0;
    Subrun = 0;
    Event = 0;
    true_x = 0.0;
    true_y = 0.0;
    true_z = 0.0;
    true_E = 0.0;

    true_Edep = 0.0;
    true_Q = 0.0;
    true_L = 0.0;

    reco_tot_hit_Q = 0.0;
    reco_N_hit = 0.0;
    reco_mean_hit_Q = 0.0;
    reco_min_hit_Q = 0.0;
    reco_max_hit_Q = 0.0;
    reco_tot_matched_E = 0.0;
    reco_tot_wire_Q = 0.0;
    reco_mean_wire_Q = 0.0;
    reco_min_wire_Q = 0.0;
    reco_max_wire_Q = 0.0;
    reco_N_wire = 0.0;

    Run = ev.eventAuxiliary().run();
    Subrun = ev.eventAuxiliary().subRun();
    Event = ev.eventAuxiliary().event();
    
    // This is the truth particles
    auto const &mctruth_handle =
      ev.getValidHandle< std::vector<simb::MCTruth> >("generator");

    auto const &simedep_handle = 
      ev.getValidHandle< std::vector<sim::SimEnergyDeposit> >("ionandscint:priorSCE");
      //ev.getValidHandle< std::vector<sim::SimEnergyDeposit> >("psdope:psdoped");

    // This is a reconstruction of charge on a single channel 
    auto const &hit_handle =
      ev.getValidHandle< std::vector<recob::Hit> >("gaushit");
        
    auto const &wire_handle = 
      ev.getValidHandle< std::vector<recob::Wire> >("caldata");

    art::FindMany<simb::MCParticle, anab::BackTrackerHitMatchingData> parts_per_hit(hit_handle,ev,"gaushitTruthMatch");

    art::FindManyP<recob::Wire> wires_per_hit(hit_handle,ev,"gaushit");
    
    //    auto all_wires(*wire_handle);
    auto simedeps(*simedep_handle);
    auto all_mcts(*mctruth_handle);
    auto gaus_hits(*hit_handle);

    if(all_mcts.size() > 1){
      std::cout << "why are there more than 1 truth particle?!" << std::endl;
    }

    true_x = all_mcts[0].GetParticle(0).Vx();
    true_y = all_mcts[0].GetParticle(0).Vy();
    true_z = all_mcts[0].GetParticle(0).Vz();
    true_E = all_mcts[0].GetParticle(0).E(0)*1000; //Convert from GeV to MeV 

    for(auto edep : simedeps){
      
      true_Edep += edep.Energy();
      true_Q += edep.NumElectrons();
      true_L += edep.NumPhotons();      
      
    }


double avgtick = 0;
double avgwire = 0;

    
    reco_min_hit_Q = 100000000000.0;
    reco_max_hit_Q = 0;

    reco_min_wire_Q = 100000000000.0;
    reco_max_wire_Q = 0;
    
    for(int h = 0; h < gaus_hits.size(); h++){
      
      auto hit = gaus_hits.at(h);
      
      if(hit.View() != 2){ continue;}
      
      std::vector<simb::MCParticle const*> parts_in_my_hit;
      std::vector<anab::BackTrackerHitMatchingData const*> partInfo_in_my_hit;
      parts_per_hit.get(h, parts_in_my_hit, partInfo_in_my_hit);
      
      for(int mcp = 0; mcp < parts_in_my_hit.size(); mcp++){

	auto mcpart = parts_in_my_hit.at(mcp);

	reco_N_hit++;
	reco_tot_hit_Q += hit.Integral();	
	reco_mean_hit_Q += hit.Integral();
	reco_tot_matched_E += partInfo_in_my_hit.at(mcp)->energy;
      	
	if(hit.Integral() < reco_min_hit_Q) reco_min_hit_Q = hit.Integral(); 
	if(hit.Integral() > reco_max_hit_Q) reco_max_hit_Q = hit.Integral(); 

	auto wires = wires_per_hit.at(h);


	reco_N_wire += wires.size();
	int ticks = 0;
	for(int wr = 0; wr < wires.size(); wr++){
		 	  
	  auto signals = wires[wr]->Signal();
	
	  int tick = 0;
	  for(int s = hit.StartTick(); s <= hit.EndTick(); s++){
	    if(s >= hit.StartTick() && s <= hit.EndTick()){
	      reco_tot_wire_Q += signals[s];
	      reco_mean_wire_Q += signals[s];
	      ticks++;
	      if(signals[s] < reco_min_wire_Q) reco_min_wire_Q = signals[s];
	      if(signals[s] > reco_max_wire_Q) reco_max_wire_Q = signals[s];

	    }//overlap with hit
	  }//iterate over all matched ticks
	  	  
	}// Iterate over recob::Wires Matched to Hits
	reco_mean_wire_Q /= ticks;
      }//Iterate over MCP matched to hits
    }//Iterate over hits
    reco_mean_hit_Q /= reco_N_hit; 

    fTree->Fill();

  }//Iterate over Events
  out->cd();  
  fTree->Write();
  
  
  
}//Close Program


