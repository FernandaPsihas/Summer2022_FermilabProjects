#define EventTree_cxx
#include "EventTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void EventTree::Loop()
{
//   In a ROOT session, you can do:
//      root> .L EventTree.C
//      root> EventTree t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch

  TH2D* true_vs_hitQ = new TH2D("hitQ","",50,0,20,250,0,2500);
  TH2D* true_vs_wireQ = new TH2D("wireQ","",50,0,20,250,0,2500);

  TH2D* E_vs_Nhits = new TH2D("E_vs_Nhits","",100,0,20,10,-0.5,9.5);

  TFile* outfile = new TFile("outfile.root","RECREATE"); 
  TTree* fTree = new TTree("FastTree","tree");
  double x; 
  double y;
  double z;
  int Nhits;
  double wireQ;
  double hitQ;
  double correction;
  double trueE;
  double Edep;
  double matchedE;
  
  fTree->Branch("x",&x);
  fTree->Branch("y",&y);
  fTree->Branch("z",&z);
  fTree->Branch("Nhits",&Nhits);
  fTree->Branch("wireQ",&wireQ);
  fTree->Branch("hitQ",&hitQ);
  fTree->Branch("correction",&correction);
  fTree->Branch("trueE",&trueE);
  fTree->Branch("Edep",&Edep);
  fTree->Branch("matchedE",&matchedE);

  if (fChain == 0) return;
  
  Long64_t nentries = fChain->GetEntriesFast();
  
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      x = 0; 
      y = 0;
      z = 0;
      Nhits = 0;
      wireQ = 0;
      hitQ = 0;
      correction = 0;
      trueE = 0;
      Edep = 0;
      matchedE = 0;
      
      for(int part = 0; part < true_E->size(); part++){
       
	std::cout << true_PDGCode->at(part) <<std::endl;

	for(int e = 0; e < true_Edep->size(); e++){
	  Edep += true_Edep->at(e);
	}
	      
	if(reco_tot_hit_Q->at(part) == 0) continue;

	x = true_x->at(part);
	y = true_y->at(part);
	z = true_z->at(part);
	
	Nhits = reco_hit_Q->at(part).size();
	
	correction = (1+(1-exp(-1*fabs(200-fabs(true_x->at(part)))/1601)));

	wireQ = reco_tot_wire_Q->at(part);
	hitQ = reco_tot_hit_Q->at(part);

	trueE = (1000.*true_E->at(part)) - 0.51099895000; 	
	
	for(int h = 0; h < reco_matched_E->at(part).size(); h++){
	  
	  matchedE += reco_matched_E->at(part).at(h);
	  
	}
      }

      fTree->Fill();

  }

  outfile->cd();  
  fTree->Write();
  
}
