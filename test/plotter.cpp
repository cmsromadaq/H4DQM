#include <TStyle.h>
#include <TROOT.h>
#include <TColor.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraph.h>
#include <TRandom.h>

void
set_plot_blue ()
{
    Double_t Red[3]    = { 0.00, 0.00, 0.00};
    Double_t Green[3]  = { 0.00, 0.00, 0.00};
    Double_t Blue[3]   = { 0.00, 0.00, 1.00};
    Double_t Length[3] = { 0.00, 0.00, 1.00};
    Int_t nb=50;
    TColor::CreateGradientColorTable(3,Length,Red,Green,Blue,nb);
    gStyle->SetNumberContours(nb);
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void setPlotsFormat ()
{
    // general root settings
	gROOT->SetStyle ("Plain") ;
    gStyle->SetOptStat ("emruo") ;
	gStyle->SetOptFit (1111) ;
	gStyle->SetCanvasDefH (300) ; //Height of canvas
	gStyle->SetCanvasDefW (300) ; //Width of canvas
	
	// fonts of titles and labels
	gStyle->SetTitleBorderSize (0) ;
	gStyle->SetTitleX (0.08) ;
	gStyle->SetTitleY (0.97) ;
	gStyle->SetPalette (1,0) ;
	gStyle->SetLabelFont (42,"xyz") ;
	gStyle->SetTextFont (42) ;
	gStyle->SetStatFont (42) ;
	gStyle->SetTitleFont (42,"xyz") ;
	//gStyle->SetPadGridX (1) ;
	//gStyle->SetPadGridY (1) ;

    // positioning
	gStyle->SetTitleXOffset (1.25) ;
	gStyle->SetTitleYOffset (1.4) ;

	gStyle->SetTitleX (0.5) ; // put title box in the middle
	gStyle->SetTitleFont (42) ;
	gStyle->SetTitleAlign (23) ; // center title text in title box
	gStyle->SetTitleSize (0.04, "xyz") ;
	gStyle->SetLabelSize (0.05, "xyz") ;

	gStyle->SetPadRightMargin (0.05)  ;
	gStyle->SetPadLeftMargin (0.15) ;
	gStyle->SetPadBottomMargin (0.15) ;
	gStyle->SetPadTopMargin (0.1) ;

    // ticks and divisions on the axes
	gStyle->SetPadTickX (1) ;
	gStyle->SetPadTickY (1) ;
	gStyle->SetNdivisions(7, "xyz");

    // frame drawing options
	gStyle->SetLineWidth (2) ;
	gStyle->SetFillStyle (0) ;
	gStyle->SetStatStyle (0) ;

    // histogram default drawing options
    gStyle->SetHistFillColor (kOrange) ;
    gStyle->SetHistLineColor (kBlack) ;
    gStyle->SetHistLineStyle (1) ;
    gStyle->SetHistLineWidth (2) ;

    // stats box position
    gStyle->SetStatX (0.95) ;
    gStyle->SetStatY (0.9) ;
    gStyle->SetStatW (0.2) ;
    gStyle->SetStatH (0.15) ;

	set_plot_blue () ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotMe (TH1F * histo)
{
  TString hname = histo->GetName () ;
  TString canvasName = hname + "_small.png" ;
  TCanvas * c1 = new TCanvas ("c1", "c1", 300, 300) ;
  histo->Draw () ;
  c1->Print (canvasName, "png") ;
  canvasName = hname + "_large.png" ;
  delete c1 ;
  c1 = new TCanvas ("c1", "c1", 800, 800) ;
  histo->Draw () ;
  c1->Print (canvasName, "png") ;
  delete c1 ;
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotMe (TH2F * histo)
{
  TString hname = histo->GetName () ;
  TString canvasName = hname + "_small.png" ;
  TCanvas * c1 = new TCanvas ("c1", "c1", 300, 300) ;
  histo->Draw ("colz") ;
  c1->Print (canvasName, "png") ;
  canvasName = hname + "_large.png" ;
  delete c1 ;
  c1 = new TCanvas ("c1", "c1", 800, 800) ;
  histo->Draw ("colz") ;
  c1->Print (canvasName, "png") ;
  delete c1 ;
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotMe (TGraph * graph, const TString & name)
{
  TString canvasName = name + "_small.png" ;
  TCanvas * c1 = new TCanvas ("c1", "c1", 300, 300) ;
  graph->SetMarkerStyle (8) ;
  graph->SetMarkerSize (1.5) ;
  graph->SetMarkerColor (kBlue) ;  
  graph->Draw ("ALP") ;
  c1->Print (canvasName, "png") ;
  canvasName = name + "_large.png" ;
  delete c1 ;
  c1 = new TCanvas ("c1", "c1", 800, 800) ;
  graph->Draw ("ALP") ;
  c1->Print (canvasName, "png") ;
  delete c1 ;
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void setAxisTitles (TH1 * histo, const TString & xTitle, const TString & yTitle) 
{
  histo->GetXaxis ()->SetTitle (xTitle) ;
  histo->GetYaxis ()->SetTitle (yTitle) ;
  return ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int main (int argc, char ** argv) 
{
  setPlotsFormat () ;

  TH1F * testH1 = new TH1F ("testH1", "test histogram", 10, 0., 10.) ;
  setAxisTitles (testH1, "x axis", "y axis") ;
  testH1->Fill (2) ;
  testH1->Fill (3) ;
  plotMe (testH1) ;
  
  TH2F * testH2  = new TH2F ("testH2", "py vs px", 40, -4, 4, 40, -4, 4) ;
  setAxisTitles (testH2, "x axis", "y axis") ;
  Float_t px, py ;
  for (Int_t i = 0; i < 25000; ++i) 
    {
      gRandom->Rannor (px,py) ;
      testH2->Fill (px,py) ;
    }  
  plotMe (testH2) ;  

  Double_t x[100], y[100] ;
  Int_t n = 20 ;
  for (Int_t i = 0 ; i < n ; ++i) 
    {
      x[i] = i * 0.1 ;
      y[i] = 10 * sin (x[i] + 0.2) ;
    }
  TGraph * testG = new TGraph (n,x,y) ;
  setAxisTitles (testG->GetHistogram (), "x axis", "y axis") ;
  plotMe (testG, "testG") ;  
  
  return 0 ;


}