#include <plotterTools.hpp>


plotterTools::plotterTools(char* filename, char*outfname){

  setPlotsFormat () ;
  inputFile_ = TFile::Open(filename);

  if( inputFile_==0 ) {
    std::cout << "ERROR! Din't find file " << filename << std::endl;
    std::cout << "Exiting." << std::endl;
    exit(11);
  }

  inputTree_ = (TChain*) inputFile_->Get("H4tree");
  outputFile_ = TFile::Open(outfname,"RECREATE");

};


void plotterTools::set_plot_blue ()
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


void  plotterTools::setPlotsFormat ()
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


void  plotterTools::plotMe (TH1F * histo)
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


void  plotterTools::plotMe (TH2F * histo)
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


void  plotterTools::plotMe (TGraph * graph, const TString & name)
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


void  plotterTools::setAxisTitles (TH1 * histo, const TString & xTitle, const TString & yTitle) 
{
  histo->GetXaxis ()->SetTitle (xTitle) ;
  histo->GetYaxis ()->SetTitle (yTitle) ;
  return ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

void  plotterTools::readInputTree (treeStructData& treeData)
{
  //Instantiate the tree branches
  inputTree_->Print();
  inputTree_->SetBranchAddress("evtNumber",&treeData.evtNumber);
  inputTree_->SetBranchAddress("evtTimeDist",&treeData.evtTimeDist);
  inputTree_->SetBranchAddress("evtTimeStart",&treeData.evtTimeStart);
  inputTree_->SetBranchAddress("evtTime",&treeData.evtTime);


  inputTree_->SetBranchAddress("triggerWord",&treeData.triggerWord);

  inputTree_->SetBranchAddress("nAdcChannels",&treeData.nAdcChannels);
  inputTree_->SetBranchAddress("adcBoard",treeData.adcBoard);
  inputTree_->SetBranchAddress("adcChannel",treeData.adcChannel);
  inputTree_->SetBranchAddress("adcData",treeData.adcData);

  inputTree_->SetBranchAddress("nTdcChannels",&treeData.nTdcChannels);
  inputTree_->SetBranchAddress("tdcBoard",treeData.tdcBoard);
  inputTree_->SetBranchAddress("tdcChannel",treeData.tdcChannel);
  inputTree_->SetBranchAddress("tdcData",treeData.tdcData);

  inputTree_->SetBranchAddress("nDigiSamples",&treeData.nDigiSamples);
  inputTree_->SetBranchAddress("digiGroup",treeData.digiGroup);
  inputTree_->SetBranchAddress("digiChannel",treeData.digiChannel);
  inputTree_->SetBranchAddress("digiSampleIndex",treeData.digiSampleIndex);
  inputTree_->SetBranchAddress("digiSampleValue",treeData.digiSampleValue);

  inputTree_->SetBranchAddress("nScalerWords",&treeData.nScalerWords);
  inputTree_->SetBranchAddress("scalerWord",treeData.scalerWord);

  return ;
} 


void  plotterTools::Loop(){

  treeStructData treeStruct;
  readInputTree(treeStruct);

  int nentries = inputTree_->GetEntries();


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
  outObjects_[modules[0]+TString("_")+types[0]+TString("_")+TString(graph_triggerEff->GetTitle())]=(TObject*)graph_triggerEff;

  //print booked histograms
  std::cout << "==================== Booked histograms =======================" << std::endl;
  for (std::map<TString,TObject*>::const_iterator out=outObjects_.begin();out!=outObjects_.end();++out)
    std::cout << out->second->GetName() << std::endl;
  std::cout << "==================== Loop over events =======================" << std::endl;


 for( unsigned iEntry=0; iEntry<nentries; ++iEntry ) {
   inputTree_->GetEntry(iEntry);
   if(iEntry%historyStep==0){
     if( (int)iEntry/historyStep-1 < nBinsHistory){//all history plots should go here
       graph_triggerEff->SetPoint((int)iEntry/historyStep-1, iEntry,(float)treeStruct.scalerWord[2]/treeStruct.scalerWord[1]);
     }
   }
 }
 
 
 //plot histories
 plotMe (graph_triggerEff,graph_triggerEff->GetTitle());

 saveHistos();

}

void plotterTools::saveHistos(){
  std::cout << "==================== Saving histograms =======================" << std::endl;
  std::cout << "outputFile " << outputFile_->GetName() << " opened" << std::endl;
  outputFile_->cd();
  for (std::map<TString,TObject*>::const_iterator out=outObjects_.begin();out!=outObjects_.end();++out)
    out->second->Write(out->first);
  outputFile_->Close();
  std::cout << "outputFile " << outputFile_ << " closed" << std::endl;
  std::cout << "==================== DQM analysis is done =======================" << std::endl;
}
