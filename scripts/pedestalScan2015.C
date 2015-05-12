#define pedestalScan2015_cxx

#include <TH1.h>
#include <TF1.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>


#include "./pedestalScan2015.h"

using namespace std;

void pedestalScan2015::Loop()
{
  if (fChain == 0) return;
  
  Long64_t nentries = fChain->GetEntriesFast();  
  
  // histos declaration
  TH1F *H1_Ped_B64[64];

  char histoName[300];
  for (int ii=0; ii<64; ii++) {
    sprintf(histoName, "H1_Ped_B64[%d]", ii);
    H1_Ped_B64[ii] = new TH1F(histoName,histoName,40,480,520);
  }

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    for (UInt_t j=0; j<nAdcChannels; j++) {
      float theAdcValue = adcData[j];
      int theAdcChannel = adcChannel[j];
      if (adcBoard[j]==201392129) {
	if (theAdcChannel>65) cout << "problem?" << endl;
	if (theAdcChannel>63) continue;   // spare channels
	H1_Ped_B64[theAdcChannel]->Fill(theAdcValue);
      }
    }
  }

  // Gaussian fits + dump the output into a txt file
  ofstream *ResFile = new ofstream("../test/pedestalScan.txt",ios::out);
  *ResFile << "## Board" << "\t\t" << "channel" << "\t\t" << "mean" << "\t\t" << "sigma" << "\t\t" << "chi2" << "\t\t" << "threshold (def: mean+5sigma)" << endl;

  for (int ii=0; ii<64; ii++) {   
    int peakBin   = H1_Ped_B64[ii]->GetMaximumBin();
    float h_peak  = H1_Ped_B64[ii]->GetBinCenter(peakBin);
    float h_rms   = H1_Ped_B64[ii]->GetRMS();
    H1_Ped_B64[ii]->Fit("gaus");
    TF1 *fitF = (TF1*)H1_Ped_B64[ii]->GetFunction("gaus");
    double gausMean  = fitF->GetParameter(1);
    double gausSigma = fabs(fitF->GetParameter(2));
    double gausChi2  = 100.;
    double threshold = gausMean + 5.*gausSigma; 
    if (fitF->GetNDF()>0) 
      gausChi2 = fitF->GetChisquare()/fitF->GetNDF() ;
    if (gausChi2>10){ gausMean = h_peak; gausSigma = h_rms; }
    *ResFile << "201392129" << "\t" << ii << "\t" << gausMean << "\t" << gausSigma << "\t" << gausChi2 << "\t" << threshold << endl;
  }

  // Save the histos in plots for the DQM
  char plotFileName[300];
  TCanvas c1("c1","",1);
  c1.Divide(2,2);
  for (int ii=0; ii<64; ii++) {   
    int frame = ii%4+1;
    c1.cd(frame);
    H1_Ped_B64[ii]->Draw();
    if (ii%4==3) { 
      int ratio = ii/4;
      sprintf(plotFileName, "../test/Board201392129_plots%d.png",ratio);
      c1.SaveAs(plotFileName);
    }
  } 
}
