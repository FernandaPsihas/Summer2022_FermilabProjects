
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
  filenames.erase(filenames.begin()+10,filenames.end());
  
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

  double wiremap[100][300]; //wires//ticks
  fTree->Branch("wiremap",&wiremap);

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
    
    for(int i = 0; i < 100; i++)
      for(int j = 0; j < 300; j++)
	wiremap[i][j] = 0;

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
    
    auto all_wires(*wire_handle);
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
    int reco_N_hit = 0;

    for(int h = 0; h < gaus_hits.size(); h++){
      
      auto hit = gaus_hits.at(h);
      
      if(hit.View() != 2){ continue;}
      
      std::vector<simb::MCParticle const*> parts_in_my_hit;
      std::vector<anab::BackTrackerHitMatchingData const*> partInfo_in_my_hit;
      parts_per_hit.get(h, parts_in_my_hit, partInfo_in_my_hit);
      
      for(int mcp = 0; mcp < parts_in_my_hit.size(); mcp++){

	auto mcpart = parts_in_my_hit.at(mcp);

	reco_N_hit++;
	
	avgtick += hit.PeakTime();
	avgwire += hit.Channel();
	
      }
    }
    
    if(reco_N_hit == 0) continue;
    
    avgwire /= reco_N_hit;
    avgtick /= reco_N_hit;

    std::cout << "avgwire : " << avgwire << std::endl;
    std::cout << "avgtick : " << avgtick << std::endl;
    
    int it_w = 0;

    for(int w = 0; w < all_wires.size(); w++){

      if(all_wires[w].View() != 2){ continue;}
      if(all_wires[w].Channel() < int(avgwire-49)) continue;
      if(all_wires[w].Channel() > int(avgwire+50)) continue;
      
      auto signals = all_wires[w].Signal();	
      
      int it_t = 0;
      
      for(int s = 0; s < signals.size(); s++){
	if(s < int(avgtick-149)) continue; 
	if(s > int(avgtick+150)) continue; 
	       
	if(it_w > 100-1) std::cout << "too many wires! " << it_w << std::endl;
	if(it_t > 300-1) std::cout << "too many ticks! " << it_t << std::endl;
	
	wiremap[it_w][it_t] = signals[s];	  
	it_t++;
	
      }//iterate over ticks
      it_w++;
    }//iterate over wires
    
    fTree->Fill();
    
  }//Iterate over Events
  out->cd();  
  fTree->Write();
  
  
  
}//Close Program


