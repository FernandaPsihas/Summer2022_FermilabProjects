//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Jul  7 13:46:48 2022 by ROOT version 6.22/08
// from TTree EventTree/tree
// found on file: low_energy_electrons_undoped.root
//////////////////////////////////////////////////////////

#ifndef EventTree_h
#define EventTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class EventTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           Run;
   Int_t           Subrun;
   Int_t           Event;
   Double_t        true_x;
   Double_t        true_y;
   Double_t        true_z;
   Double_t        true_E;
   Double_t        true_Edep;
   Int_t           true_Q;
   Int_t           true_L;
   Float_t         wiremap[100][300];

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
   TBranch        *b_wiremap;   //!

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
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("low_energy_electrons_undoped.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("low_energy_electrons_undoped.root");
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
   fChain->SetBranchAddress("wiremap", wiremap, &b_wiremap);
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
