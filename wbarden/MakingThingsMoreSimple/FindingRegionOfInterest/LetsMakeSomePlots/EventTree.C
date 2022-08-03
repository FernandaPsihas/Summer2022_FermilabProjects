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
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;

   std::vector<TH2D*> map;
   int it_map = 0;


//   TH2D* map = new TH2D("map","title;x;y", 100,0,100,300,0,300);
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
	if(jentry>10) continue;
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
	it_map++;

	map.resize(jentry+1);
	map[jentry] = new TH2D(Form("m_%d",jentry),"",50,0,50,250,0,250);
	for(int w = 0; w <50; w++){
	  for(int t = 0; t<250; t++){
		map[jentry]->SetBinContent(w,t,wiremap[w][t]);
		//map->SetBinContent(w,t,wiremap[w][t]);
	    }//loop over ticks
	}//loop over wires
      // if (Cut(ientry) < 0) continue;
   }//end of event loop

std::vector<TCanvas*> can;
can.resize(nentries);
for(int h = 0; h<map.size(); h++){
	can[h] = new TCanvas();
	can[h]->cd();
	map[h]->Draw("colz");
}
//map->Draw("colz");
}
