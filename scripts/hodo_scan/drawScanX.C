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


  TGraphErrors* gr01 = new TGraphErrors(0);

  TGraphErrors* gr0 = new TGraphErrors(0);
  TGraphErrors* gr1 = new TGraphErrors(0);
  TGraphErrors* gr2 = new TGraphErrors(0);
  TGraphErrors* gr3 = new TGraphErrors(0);

  TGraphErrors* gr_cef3 = new TGraphErrors(0);
  gr_cef3->SetName("cef3");

  std::vector< std::pair<int, float> > run_pos;
  run_pos.push_back( std::pair< int, float>(175, 198.5) );
  run_pos.push_back( std::pair< int, float>(176, 198.0) );
  run_pos.push_back( std::pair< int, float>(177, 197.5) );
  run_pos.push_back( std::pair< int, float>(178, 197.0) );
  run_pos.push_back( std::pair< int, float>(179, 196.5) );
  run_pos.push_back( std::pair< int, float>(180, 196.0) );
  run_pos.push_back( std::pair< int, float>(181, 199.0) );
  run_pos.push_back( std::pair< int, float>(182, 199.5) );
  run_pos.push_back( std::pair< int, float>(183, 200.0) );
  run_pos.push_back( std::pair< int, float>(184, 200.5) );
  run_pos.push_back( std::pair< int, float>(185, 201.0) );
  run_pos.push_back( std::pair< int, float>(186, 201.5) );
  run_pos.push_back( std::pair< int, float>(187, 202.0) );
  run_pos.push_back( std::pair< int, float>(189, 202.5) );
  run_pos.push_back( std::pair< int, float>(190, 195.5) );
  run_pos.push_back( std::pair< int, float>(191, 195.0) );
  run_pos.push_back( std::pair< int, float>(192, 135.0) );
  run_pos.push_back( std::pair< int, float>(193, 194.5) );
  run_pos.push_back( std::pair< int, float>(194, 190.0) );
  run_pos.push_back( std::pair< int, float>(195, 210.0) );
  run_pos.push_back( std::pair< int, float>(196, 185.0) );
  run_pos.push_back( std::pair< int, float>(197, 175.0) );
  run_pos.push_back( std::pair< int, float>(198, 100.0) );
  run_pos.push_back( std::pair< int, float>(199, 50.0) );
  run_pos.push_back( std::pair< int, float>(200, 0.0) );
  run_pos.push_back( std::pair< int, float>(200, 0.0) );
  run_pos.push_back( std::pair< int, float>(208, 187.5) );
  run_pos.push_back( std::pair< int, float>(209, 191) );
  run_pos.push_back( std::pair< int, float>(210, 192) );
  run_pos.push_back( std::pair< int, float>(211, 192.5) );
  run_pos.push_back( std::pair< int, float>(212, 193) );
  run_pos.push_back( std::pair< int, float>(213, 191.5) );
  run_pos.push_back( std::pair< int, float>(214, 190.5) );
  run_pos.push_back( std::pair< int, float>(215, 189.5) );
  run_pos.push_back( std::pair< int, float>(216, 188.5) );
  run_pos.push_back( std::pair< int, float>(217, 187.5) );
  run_pos.push_back( std::pair< int, float>(218, 187.0) );
  run_pos.push_back( std::pair< int, float>(219, 186.5) );
  run_pos.push_back( std::pair< int, float>(220, 186.0) );
  run_pos.push_back( std::pair< int, float>(221, 185.5) );
  run_pos.push_back( std::pair< int, float>(222, 185.0) );
  run_pos.push_back( std::pair< int, float>(223, 184.5) );
  run_pos.push_back( std::pair< int, float>(224, 184.0) );



  for( unsigned i=0; i<run_pos.size(); ++i ) {

    hodoScan p(run_pos[i].first);
    std::vector<float> eff = p.Loop();
    gr0->SetPoint( i, run_pos[i].second, eff[0] );
    gr1->SetPoint( i, run_pos[i].second, eff[1] );
    gr2->SetPoint( i, run_pos[i].second, eff[2] );
    gr3->SetPoint( i, run_pos[i].second, eff[3] );

    gr01->SetPoint( i, run_pos[i].second, 0.5*(eff[0]+eff[1]) );
    
    cef3Scan c(run_pos[i].first);
    TH1F* h1_cef3 = c.Loop();
    //
    //TF1* f1 = new TF1(Form("f1_%d", run_pos[i].first), "gaus", 2000., 6000.);
    //h1_cef3->Fit( f1, "RQ" );
    //for( unsigned j=0; j<4; ++j ) {
    //  float mu = f1->GetParameter(1);
    //  float sigma = f1->GetParameter(2);
    //  f1->SetRange( mu-0.7*sigma, mu+0.7*sigma );
    //  h1_cef3->Fit( f1, "RQ" );
    //}

    //TCanvas* c2 = new TCanvas("c2", "", 600, 600);
    //c2->cd();
    //h1_cef3->Draw();
    //c2->SaveAs(Form("prova/%d.png", run_pos[i].first));
    //delete c2;
    //gr_cef3->SetPoint( i, run_pos[i].second, f1->GetParameter(1) );
    //gr_cef3->SetPointError( i, 0., f1->GetParError(1) );

    gr_cef3->SetPoint( i, run_pos[i].second, h1_cef3->GetBinCenter(h1_cef3->GetMaximumBin()));
    
  }

  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();

  TH2D* axes = new TH2D("axes", "", 10, 120., 250., 10, 0., 0.15 );
  axes->SetXTitle("X Position [mm]");
  axes->SetYTitle("Efficiency");
  axes->Draw();

  gr0->SetMarkerStyle(20);
  gr0->SetMarkerColor(46);
  gr0->SetMarkerSize(1.);

  gr1->SetMarkerStyle(21);
  gr1->SetMarkerColor(29);
  gr1->SetMarkerSize(1.);

  gr2->SetMarkerStyle(22);
  gr2->SetMarkerColor(38);
  gr2->SetMarkerSize(1.);

  gr3->SetMarkerStyle(23);
  gr3->SetMarkerColor(kGreen+2);
  gr3->SetMarkerSize(1.);

  gr01->SetMarkerStyle(20);
  gr01->SetMarkerColor(kBlack);
  gr01->SetMarkerSize(1.);

  gr0->Draw("Psame");
  gr1->Draw("Psame");
  //gr2->Draw("Psame");
  //gr3->Draw("Psame");

  gr01->Draw("Psame");

  TLegend* legend = new TLegend(0.2, 0.6, 0.55, 0.9);
  legend->SetFillColor(0);
  legend->SetTextSize(0.035);
  legend->AddEntry( gr0, "0", "P" );
  legend->AddEntry( gr1, "1", "P" );
  legend->AddEntry( gr01, "0+1", "P" );
  legend->Draw("same");

  c1->SaveAs("posScanX.eps");
  c1->SaveAs("posScanX.png");

  TFile* file = TFile::Open("prova.root", "recreate");

  file->cd();
  gr_cef3->Write();
  file->Close();

}
