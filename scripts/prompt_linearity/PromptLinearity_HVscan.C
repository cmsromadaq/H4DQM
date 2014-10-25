#define PromptLinearity_cxx

using namespace std;
#include "PromptLinearity_HVscan.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TH1F.h"
#include "TGraphErrors.h"
#include "TFile.h"
#include "TF1.h"



void PromptLinearity::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L PromptLinearity.C
//      Root > PromptLinearity t
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


   TH1F *h[30][4];
   for (int b=0; b<30; b++)
     for (int i=0; i<4; i++)
       h[b][i] = new TH1F(Form("h%d%d",b,i),Form("h%d%d",b,i),10000,0,2000);
   TGraphErrors* gr[4];
   for (int i=0; i<4; i++) gr[i] = new TGraphErrors(30);
   for (int i=0; i<4; i++)
     for (int b=0; b<30; b++){
       gr[i]->SetPoint(b,0,0);
       gr[i]->SetPointError(b,0,0);
     }

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      if (Cut(ientry) < 0) continue;

      int x = (runNumber-432);
      if (x==0) x=1;
      else if (x==1) x=0;

      float volt = 350+x*50;

      for (int i=0; i<4; i++) {
        if (TMath::Abs(TDCreco->at(0))>5 || TMath::Abs(TDCreco->at(1))>5) continue;
        h[x][i]->Fill(digi_max_amplitude->at(i));
      }

   }
 
   for (int b=0; b<30; b++){
     for (int i=0; i<4; i++){
       if (h[b][i]->GetEntries()==0) continue;
       float max = h[b][i]->GetBinCenter(h[b][i]->GetMaximumBin());
       float rms = h[b][i]->GetRMS();
       h[b][i]->Fit("gaus","R","",max-2.5*rms,max+2.5*rms);
       TF1 *myfunc = h[b][i]->GetFunction("gaus");
       Double_t par0 = myfunc->GetParameter(0);
       Double_t par1 = myfunc->GetParameter(1);
       Double_t par1err = myfunc->GetParError(1);
       Double_t par2 = myfunc->GetParameter(2);
       h[b][i]->Fit("gaus","R","",par1-2*par2,par1+2*par2);
       myfunc = h[b][i]->GetFunction("gaus");
       par0 = myfunc->GetParameter(0);
       par1 = myfunc->GetParameter(1);
       par1err = myfunc->GetParError(1);
       par2 = myfunc->GetParameter(2);
       float volt = 350+b*50;
       gr[i]->SetPoint(b,volt,par1);
       gr[i]->SetPointError(b,0,par1err);
     }
   }

   TFile *f = new TFile("linearityHV.root","recreate");

   /*
   for (int b=0; b<30; b++)
     for (int i=0; i<4; i++)
       h[b][i]->Write();
   */

   for (int i=0; i<4; i++){
     gr[i]->SetTitle(Form("CeF_{3} fiber %d",i));
     gr[i]->GetXaxis()->SetTitle("HV (V)");
     gr[i]->GetYaxis()->SetTitle("Digitizer counts");
     gr[i]->GetXaxis()->SetRangeUser(300,1350);
     gr[i]->Draw("* E1");
     gr[i]->SetMarkerStyle(20);
     gr[i]->SetMarkerColor(kBlack);
//     gr[i]->Fit("pol2");
//     gr[i]->GetFunction("pol2")->SetLineColor(kGreen);
//     gr[i]->Fit("pol1","R+","same",0,60);
//     gr[i]->GetFunction("pol1")->SetLineColor(kRed);
//     gr[i]->GetFunction("pol1")->SetRange(0,200);
   }

   f->cd();
   for (int i=0; i<4; i++) gr[i]->Write(Form("fiber%d",i));
   f->Close();

   }
