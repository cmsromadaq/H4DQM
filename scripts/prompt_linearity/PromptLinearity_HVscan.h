//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Oct 25 04:48:29 2014 by ROOT version 5.34/19
// from TTree outputTree/outputTree
// found on file: all.root
//////////////////////////////////////////////////////////

#ifndef PromptLinearity_h
#define PromptLinearity_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>
#include <vector>

// Fixed size dimensions of array or collections stored in the TTree if any.

class PromptLinearity {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   UInt_t          runNumber;
   UInt_t          spillNumber;
   UInt_t          evtNumber;
   vector<float>   *ADCvalues;
   vector<float>   *BGOvalues;
   vector<float>   *SCINTvalues;
   vector<float>   *TDCreco;
   vector<float>   *digi_charge_integrated;
   vector<float>   *digi_max_amplitude;
   vector<float>   *digi_pedestal;
   vector<float>   *digi_pedestal_rms;
   vector<float>   *digi_time_at_frac30;
   vector<float>   *digi_time_at_frac50;
   vector<float>   *digi_time_at_max;
   vector<bool>    *HODOX1;
   vector<bool>    *HODOX2;
   vector<bool>    *HODOY1;
   vector<bool>    *HODOY2;

   // List of branches
   TBranch        *b_runNumber;   //!
   TBranch        *b_spillNumber;   //!
   TBranch        *b_evtNumber;   //!
   TBranch        *b_ADCvalues;   //!
   TBranch        *b_BGOvalues;   //!
   TBranch        *b_SCINTvalues;   //!
   TBranch        *b_TDCreco;   //!
   TBranch        *b_digi_charge_integrated;   //!
   TBranch        *b_digi_max_amplitude;   //!
   TBranch        *b_digi_pedestal;   //!
   TBranch        *b_digi_pedestal_rms;   //!
   TBranch        *b_digi_time_at_frac30;   //!
   TBranch        *b_digi_time_at_frac50;   //!
   TBranch        *b_digi_time_at_max;   //!
   TBranch        *b_HODOX1;   //!
   TBranch        *b_HODOX2;   //!
   TBranch        *b_HODOY1;   //!
   TBranch        *b_HODOY2;   //!

   PromptLinearity(TTree *tree=0);
   virtual ~PromptLinearity();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef PromptLinearity_cxx
PromptLinearity::PromptLinearity(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("all.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("all.root");
      }
      f->GetObject("outputTree",tree);

   }
   Init(tree);
}

PromptLinearity::~PromptLinearity()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t PromptLinearity::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t PromptLinearity::LoadTree(Long64_t entry)
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

void PromptLinearity::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   ADCvalues = 0;
   BGOvalues = 0;
   SCINTvalues = 0;
   TDCreco = 0;
   digi_charge_integrated = 0;
   digi_max_amplitude = 0;
   digi_pedestal = 0;
   digi_pedestal_rms = 0;
   digi_time_at_frac30 = 0;
   digi_time_at_frac50 = 0;
   digi_time_at_max = 0;
   HODOX1 = 0;
   HODOX2 = 0;
   HODOY1 = 0;
   HODOY2 = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("runNumber", &runNumber, &b_runNumber);
   fChain->SetBranchAddress("spillNumber", &spillNumber, &b_spillNumber);
   fChain->SetBranchAddress("evtNumber", &evtNumber, &b_evtNumber);
   fChain->SetBranchAddress("ADCvalues", &ADCvalues, &b_ADCvalues);
   fChain->SetBranchAddress("BGOvalues", &BGOvalues, &b_BGOvalues);
   fChain->SetBranchAddress("SCINTvalues", &SCINTvalues, &b_SCINTvalues);
   fChain->SetBranchAddress("TDCreco", &TDCreco, &b_TDCreco);
   fChain->SetBranchAddress("digi_charge_integrated", &digi_charge_integrated, &b_digi_charge_integrated);
   fChain->SetBranchAddress("digi_max_amplitude", &digi_max_amplitude, &b_digi_max_amplitude);
   fChain->SetBranchAddress("digi_pedestal", &digi_pedestal, &b_digi_pedestal);
   fChain->SetBranchAddress("digi_pedestal_rms", &digi_pedestal_rms, &b_digi_pedestal_rms);
   fChain->SetBranchAddress("digi_time_at_frac30", &digi_time_at_frac30, &b_digi_time_at_frac30);
   fChain->SetBranchAddress("digi_time_at_frac50", &digi_time_at_frac50, &b_digi_time_at_frac50);
   fChain->SetBranchAddress("digi_time_at_max", &digi_time_at_max, &b_digi_time_at_max);
   fChain->SetBranchAddress("HODOX1", &HODOX1, &b_HODOX1);
   fChain->SetBranchAddress("HODOX2", &HODOX2, &b_HODOX2);
   fChain->SetBranchAddress("HODOY1", &HODOY1, &b_HODOY1);
   fChain->SetBranchAddress("HODOY2", &HODOY2, &b_HODOY2);
   Notify();
}

Bool_t PromptLinearity::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void PromptLinearity::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t PromptLinearity::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
  return (entry>=0);
}
#endif // #ifdef PromptLinearity_cxx
