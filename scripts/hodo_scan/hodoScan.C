#define hodoScan_cxx
#include "hodoScan.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>

void hodoScan::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L hodoScan.C
//      Root > hodoScan t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
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

   float frac[4];
   for (int i=0; i<4; i++) frac[i]=0;

   const int offset = 4;
   const float threshold = 500.;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      for (int i=0; i<4; i++) {
	if (digi_charge_integrated->at(offset+i)>threshold) frac[i]+=1.0/float(nentries);
      }
      // if (Cut(ientry) < 0) continue;
   }

   std::cout << "RUN " << runNumber << " SPILL " << spillNumber << std::endl; 
   for (int i=0; i<4; i++) std::cout << frac[i] << " ";
   std::cout << endl;

}
