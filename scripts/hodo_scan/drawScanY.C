#include <iostream>
#include <vector>
#include <utility>
#include <stdlib.h>
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TF1.h"
#include "hodoScan.h"
#include "cef3Scan.h"

void run() {


  TGraphErrors* gr0 = new TGraphErrors(0);
  TGraphErrors* gr1 = new TGraphErrors(0);
  TGraphErrors* gr2 = new TGraphErrors(0);
  TGraphErrors* gr3 = new TGraphErrors(0);

  TGraphErrors* gr23 = new TGraphErrors(0);

  TGraphErrors* gr_cef3 = new TGraphErrors(0);
  gr_cef3->SetName("cef3");

  std::vector< std::pair<int, float> > run_pos;
  run_pos.push_back( std::pair< int, float>(225, 250.0) );
  run_pos.push_back( std::pair< int, float>(226, 251.0) );
  run_pos.push_back( std::pair< int, float>(227, 252.0) );
  run_pos.push_back( std::pair< int, float>(228, 253.0) );
  run_pos.push_back( std::pair< int, float>(229, 254.0) );
  run_pos.push_back( std::pair< int, float>(230, 248.0) );
  run_pos.push_back( std::pair< int, float>(231, 247.0) );
  run_pos.push_back( std::pair< int, float>(232, 246.0) );
  run_pos.push_back( std::pair< int, float>(233, 245.0) );
  run_pos.push_back( std::pair< int, float>(234, 244.0) );
  run_pos.push_back( std::pair< int, float>(235, 243.0) );
  run_pos.push_back( std::pair< int, float>(236, 242.0) );
  run_pos.push_back( std::pair< int, float>(237, 249.0) );
  run_pos.push_back( std::pair< int, float>(238, 255.0) );
  run_pos.push_back( std::pair< int, float>(239, 256.0) );
  run_pos.push_back( std::pair< int, float>(240, 257.0) );
  run_pos.push_back( std::pair< int, float>(241, 258.0) );
  run_pos.push_back( std::pair< int, float>(242, 259.0) );
  run_pos.push_back( std::pair< int, float>(243, 260.0) );
  run_pos.push_back( std::pair< int, float>(246, 261.0) );
  run_pos.push_back( std::pair< int, float>(247, 262.0) );
  run_pos.push_back( std::pair< int, float>(248, 263.0) );
  run_pos.push_back( std::pair< int, float>(249, 264.0) );
  run_pos.push_back( std::pair< int, float>(250, 265.0) );
  run_pos.push_back( std::pair< int, float>(251, 266.0) );
  run_pos.push_back( std::pair< int, float>(252, 267.0) );
  run_pos.push_back( std::pair< int, float>(253, 268.0) );
  run_pos.push_back( std::pair< int, float>(254, 270.0) );
  //run_pos.push_back( std::pair< int, float>(255, 272.5) );
  run_pos.push_back( std::pair< int, float>(244, 275.0) );

  for( unsigned i=0; i<run_pos.size(); ++i ) {

    hodoScan p(run_pos[i].first);
    std::vector<float> eff = p.Loop();
    gr0->SetPoint( i, run_pos[i].second, eff[0] );
    gr1->SetPoint( i, run_pos[i].second, eff[1] );
    gr2->SetPoint( i, run_pos[i].second, eff[2] );
    gr3->SetPoint( i, run_pos[i].second, eff[3] );
    
    gr23->SetPoint( i, run_pos[i].second, 0.5*(eff[3]+eff[2]) );
    
    cef3Scan c(run_pos[i].first);
    TH1F* h1_cef3 = c.Loop();
    
    //TF1* f1 = new TF1(Form("f1_%d", run_pos[i].first), "gaus", 3000., 6000.);
    //h1_cef3->Fit( f1, "RQL" );
    //for( unsigned j=0; j<4; ++j ) {
    //  float mu = f1->GetParameter(1);
    //  float sigma = f1->GetParameter(2);
    //  f1->SetRange( mu-sigma, mu+sigma );
    //  h1_cef3->Fit( f1, "RQL" );
    //}

    //TCanvas* c2 = new TCanvas("c2", "", 600, 600);
    //c2->cd();
    //h1_cef3->Draw();
    //c2->SaveAs(Form("provaY/%d.png", run_pos[i].first));
    //delete c2;
    //gr_cef3->SetPoint( i, run_pos[i].second, f1->GetParameter(1) );
    //gr_cef3->SetPointError( i, 0., f1->GetParError(1) );
  

    gr_cef3->SetPoint( i, run_pos[i].second, h1_cef3->GetBinCenter(h1_cef3->GetMaximumBin()));
    
  }

  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();

  TH2D* axes = new TH2D("axes", "", 10, 200., 300., 10, 0., 0.15 );
  axes->SetXTitle("Y Position [mm]");
  axes->SetYTitle("Efficiency");
  axes->Draw();

  gr0->SetMarkerStyle(20);
  gr0->SetMarkerColor(46);
  gr0->SetMarkerSize(2.);

  gr1->SetMarkerStyle(21);
  gr1->SetMarkerColor(29);
  gr1->SetMarkerSize(2.);

  gr2->SetMarkerStyle(22);
  gr2->SetMarkerColor(38);
  gr2->SetMarkerSize(2.);

  gr3->SetMarkerStyle(23);
  gr3->SetMarkerColor(kGreen+2);
  gr3->SetMarkerSize(2.);

  gr23->SetMarkerStyle(20);
  gr23->SetMarkerColor(kBlack);
  gr23->SetMarkerSize(1.);

  gr2->Draw("Psame");
  gr3->Draw("Psame");

  gr23->Draw("Psame");

  TLegend* legend = new TLegend(0.2, 0.6, 0.55, 0.9);
  legend->SetFillColor(0);
  legend->SetTextSize(0.035);
  legend->AddEntry( gr2, "2", "P" );
  legend->AddEntry( gr3, "3", "P" );
  legend->AddEntry( gr23, "2+3", "P" );
  legend->Draw("same");

  c1->SaveAs("posScanY.eps");
  c1->SaveAs("posScanY.png");

  TFile* file = TFile::Open("provaY.root", "recreate");

  file->cd();
  gr_cef3->Write();
  file->Close();

}
