#include <TStyle.h>
#include <TROOT.h>
#include <TColor.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraph.h>
#include <TRandom.h>
#include <TChain.h>
#include <TString.h>

#include <Event.hpp>


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

void readInputTree (TTree* tree,treeStructData& treeData)
{
  //Instantiate the tree branches
  tree->SetBranchAddress("evtNumber",&treeData.evtNumber);
  tree->SetBranchAddress("evtTimeDist",&treeData.evtTimeDist);
  tree->SetBranchAddress("evtTimeStart",&treeData.evtTimeStart);
  tree->SetBranchAddress("evtTime",&treeData.evtTime);

  tree->SetBranchAddress("boardTriggerBit",&treeData.boardTriggerBit);

  tree->SetBranchAddress("triggerWord",&treeData.triggerWord);

  tree->SetBranchAddress("nAdcChannels",&treeData.nAdcChannels);
  tree->SetBranchAddress("adcBoard",treeData.adcBoard);
  tree->SetBranchAddress("adcChannel",treeData.adcChannel);
  tree->SetBranchAddress("adcData",treeData.adcData);

  tree->SetBranchAddress("nTdcChannels",&treeData.nTdcChannels);
  tree->SetBranchAddress("tdcBoard",treeData.tdcBoard);
  tree->SetBranchAddress("tdcChannel",treeData.tdcChannel);
  tree->SetBranchAddress("tdcData",treeData.tdcData);

  tree->SetBranchAddress("nDigiSamples",&treeData.nDigiSamples);
  tree->SetBranchAddress("digiGroup",treeData.digiGroup);
  tree->SetBranchAddress("digiChannel",treeData.digiChannel);
  tree->SetBranchAddress("digiSampleIndex",treeData.digiSampleIndex);
  tree->SetBranchAddress("digiSampleValue",treeData.digiSampleValue);

  tree->SetBranchAddress("nScalerWords",&treeData.nScalerWords);
  tree->SetBranchAddress("scalerWord",treeData.scalerWord);

  return ;
} 


void Loop(TChain* inputTree,TFile *outputFile){

  treeStructData treeStruct;
  readInputTree(inputTree,treeStruct);

  int nentries = inputTree->GetEntries();
  std::map<TString,TObject*> outObjects;

  //modules
  std::vector<TString> modules;
  modules.push_back("beam");

  //types
  std::vector<TString> types;
  types.push_back("history");

  //history plots
  int historyStep=20; //set the step of events for history plots
  int nBinsHistory=nentries/historyStep;

  TGraph* graph_triggerEff = new TGraph (nBinsHistory);
  graph_triggerEff->SetTitle("triggerEff");
  graph_triggerEff->SetName(modules[0]+TString("_")+types[0]+TString("_")+TString(graph_triggerEff->GetTitle()));
  outObjects[modules[0]+TString("_")+types[0]+TString("_")+TString(graph_triggerEff->GetTitle())]=(TObject*)graph_triggerEff;

  //print booked histograms
  std::cout << "==================== Booked histograms =======================" << std::endl;
  for (std::map<TString,TObject*>::const_iterator out=outObjects.begin();out!=outObjects.end();++out)
    std::cout << out->second->GetName() << std::endl;
  std::cout << "==================== Loop over events =======================" << std::endl;


 for( unsigned iEntry=0; iEntry<nentries; ++iEntry ) {
   inputTree->GetEntry(iEntry);
   if(iEntry%historyStep==0){
     if( (int)iEntry/historyStep-1 < nBinsHistory){//all history plots should go here
       graph_triggerEff->SetPoint((int)iEntry/historyStep-1, iEntry,(float)treeStruct.scalerWord[2]/treeStruct.scalerWord[1]);
     }
   }
 }
 
 
 //plot histories
 plotMe (graph_triggerEff,graph_triggerEff->GetTitle());

 std::cout << "==================== Saving histograms =======================" << std::endl;
 std::cout << "outputFile " << outputFile->GetName() << " opened" << std::endl;
 outputFile->cd();
 for (std::map<TString,TObject*>::const_iterator out=outObjects.begin();out!=outObjects.end();++out)
   out->second->Write(out->first);
 outputFile->Close();
 std::cout << "outputFile " << outputFile << " closed" << std::endl;
 std::cout << "==================== DQM analysis is done =======================" << std::endl;

}



int main (int argc, char ** argv) 
{
  char *filename = NULL;
  char *outfname = NULL;
  int c;

  while ((c = getopt (argc, argv, "f:o:")) != -1)
    switch (c)
      {
      case 'f':
	filename = optarg;
	break;
     case 'o':
	outfname = optarg;
	break;


      case '?':
	if (optopt == 'f'|| optopt == 'o')
	  fprintf (stderr, "Option -%c requires an argument.\n", optopt);
	else if (isprint (optopt))
	  fprintf (stderr, "Unknown option `-%c'.\n", optopt);
	else
	  fprintf (stderr,
		   "Unknown option character `\\x%x'.\n",
		   optopt);
	return 1;
      default:
	exit (-1);
      }

  setPlotsFormat () ;
  

  TFile* inputFile = TFile::Open(filename);
  if( inputFile==0 ) {
    std::cout << "ERROR! Din't find file " << filename << std::endl;
    std::cout << "Exiting." << std::endl;
    exit(11);
  }

  TChain* inputTree = (TChain*) inputFile->Get("H4tree");





  TFile* outputFile = TFile::Open(outfname,"RECREATE");
  Loop(inputTree,outputFile);



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
