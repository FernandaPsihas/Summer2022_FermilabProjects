//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Apr 28 13:38:44 2022 by ROOT version 6.22/08
// from TTree EventTree/tree
// found on file: ../Files/Alpha_Standard_merged.root
//////////////////////////////////////////////////////////

#ifndef EventTree_h
#define EventTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "vector"
#include "vector"
#include "vector"

class EventTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           Run;
   Int_t           Subrun;
   Int_t           Event;
   vector<double>  *true_x;
   vector<double>  *true_y;
   vector<double>  *true_z;
   vector<double>  *true_E;
   vector<double>  *true_Edep;
   vector<int>     *true_Q;
   vector<int>     *true_L;
   vector<int>     *true_PDGCode;
   vector<int>     *true_trackID;
   vector<int>     *edep_trackID;
   vector<vector<double> > *true_dx;
   vector<vector<int> > *true_dQ;
   vector<vector<int> > *true_dL;
   vector<vector<double> > *true_dE;
   vector<double>  *reco_matched_trkID;
   vector<double>  *reco_matched_PDG;
   vector<double>  *reco_tot_hit_Q;
   vector<vector<double> > *reco_hit_Q;
   vector<vector<double> > *reco_matched_E;
   vector<double>  *reco_tot_wire_Q;
   vector<vector<double> > *reco_wire_Q;

   // List of branches
   TBranch        *b_Run;   //!
   TBranch        *b_Subrun;   //!
   TBranch        *b_Event;   //!
   TBranch        *b_true_x;   //!
   TBranch        *b_true_y;   //!
   TBranch        *b_true_z;   //!
   TBranch        *b_true_E;   //!
   TBranch        *b_true_Edep;   //!
   TBranch        *b_true_Q;   //!
   TBranch        *b_true_L;   //!
   TBranch        *b_true_PDGCode;   //!
   TBranch        *b_true_trackID;   //!
   TBranch        *b_edep_trackID;   //!
   TBranch        *b_true_dx;   //!
   TBranch        *b_true_dQ;   //!
   TBranch        *b_true_dL;   //!
   TBranch        *b_true_dE;   //!
   TBranch        *b_reco_matched_trkID;   //!
   TBranch        *b_reco_matched_PDG;   //!
   TBranch        *b_reco_tot_hit_Q;   //!
   TBranch        *b_reco_hit_Q;   //!
   TBranch        *b_reco_matched_E;   //!
   TBranch        *b_reco_tot_wire_Q;   //!
   TBranch        *b_reco_wire_Q;   //!

   EventTree(TTree *tree=0);
   virtual ~EventTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef EventTree_cxx
EventTree::EventTree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../AnalyzingLArSoftFiles/output.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("../AnalyzingLArSoftFiles/output.root");
      }
      f->GetObject("EventTree",tree);

   }
   Init(tree);
}

EventTree::~EventTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t EventTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t EventTree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void EventTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   true_x = 0;
   true_y = 0;
   true_z = 0;
   true_E = 0;
   true_Edep = 0;
   true_Q = 0;
   true_L = 0;
   true_PDGCode = 0;
   true_trackID = 0;
   edep_trackID = 0;
   true_dx = 0;
   true_dQ = 0;
   true_dL = 0;
   true_dE = 0;
   reco_matched_trkID = 0;
   reco_matched_PDG = 0;
   reco_tot_hit_Q = 0;
   reco_hit_Q = 0;
   reco_matched_E = 0;
   reco_tot_wire_Q = 0;
   reco_wire_Q = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Run", &Run, &b_Run);
   fChain->SetBranchAddress("Subrun", &Subrun, &b_Subrun);
   fChain->SetBranchAddress("Event", &Event, &b_Event);
   fChain->SetBranchAddress("true_x", &true_x, &b_true_x);
   fChain->SetBranchAddress("true_y", &true_y, &b_true_y);
   fChain->SetBranchAddress("true_z", &true_z, &b_true_z);
   fChain->SetBranchAddress("true_E", &true_E, &b_true_E);
   fChain->SetBranchAddress("true_Edep", &true_Edep, &b_true_Edep);
   fChain->SetBranchAddress("true_Q", &true_Q, &b_true_Q);
   fChain->SetBranchAddress("true_L", &true_L, &b_true_L);
   fChain->SetBranchAddress("true_PDGCode", &true_PDGCode, &b_true_PDGCode);
   fChain->SetBranchAddress("true_trackID", &true_trackID, &b_true_trackID);
   fChain->SetBranchAddress("edep_trackID", &edep_trackID, &b_edep_trackID);
   fChain->SetBranchAddress("true_dx", &true_dx, &b_true_dx);
   fChain->SetBranchAddress("true_dQ", &true_dQ, &b_true_dQ);
   fChain->SetBranchAddress("true_dL", &true_dL, &b_true_dL);
   fChain->SetBranchAddress("true_dE", &true_dE, &b_true_dE);
   fChain->SetBranchAddress("reco_matched_trkID", &reco_matched_trkID, &b_reco_matched_trkID);
   fChain->SetBranchAddress("reco_matched_PDG", &reco_matched_PDG, &b_reco_matched_PDG);
   fChain->SetBranchAddress("reco_tot_hit_Q", &reco_tot_hit_Q, &b_reco_tot_hit_Q);
   fChain->SetBranchAddress("reco_hit_Q", &reco_hit_Q, &b_reco_hit_Q);
   fChain->SetBranchAddress("reco_matched_E", &reco_matched_E, &b_reco_matched_E);
   fChain->SetBranchAddress("reco_tot_wire_Q", &reco_tot_wire_Q, &b_reco_tot_wire_Q);
   fChain->SetBranchAddress("reco_wire_Q", &reco_wire_Q, &b_reco_wire_Q);
   Notify();
}

Bool_t EventTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void EventTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t EventTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef EventTree_cxx
