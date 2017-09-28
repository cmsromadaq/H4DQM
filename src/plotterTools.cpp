#include <plotterTools.hpp>
#include <assert.h>

#define VERBOSE 0

template <class T, class D> void outTreeBranch<T,D>::addMember(TString name, int pos){
  if (varplots->find(name)==varplots->end()) {std::cout << "WRONG ADDMEMBER " << name.Data() << std::endl; return;}
  members.push_back(make_pair(name,pos));
}

template <class T, class D> void outTreeBranch<T,D>::addDummy(int howmany){
  for (int i=0; i<howmany; i++) members.push_back(make_pair(TString("DUMMY"),int(-1)));
}

template <class T, class D> void outTreeBranch<T,D>::Fill(){
  data.clear();
  for (std::vector<std::pair<TString,int> >::const_iterator it = members.begin(); it!=members.end(); it++){
    if (it->first=="DUMMY") {data.push_back(T(0)); continue;}
    if ((*varplots)[it->first]->type!=kPlot1D) {std::cout << "WRONG TYPE" << std::endl; continue;}
    if ((*varplots)[it->first]->Get()->size()<=it->second) {std::cout << "WRONG SIZE" << std::endl; continue;}
    data.push_back( T( *((*varplots)[it->first]->Get(it->second)) ));
  }
}

template <class T, class D> outTreeBranch<T,D>::outTreeBranch(){dataptr = &data;};
template <class T, class D> outTreeBranch<T,D>::outTreeBranch(TString name_, std::map<TString,varPlot<D>*> *varplots_): name(name_), varplots(varplots_) {dataptr = &data;}; // does not take ownership of varplots
template <class T, class D> outTreeBranch<T,D>::~outTreeBranch(){};


template <class D> std::vector<D>* varPlot<D>::Get(){ return &x; }
template <class D> std::pair<std::vector<D>*, std::vector<D>*> varPlot<D>::Get2D(){ return std::make_pair<std::vector<D>*, std::vector<D>*>(&x,&y); }
template <class D> D* varPlot<D>::Get(uint i){ return &(x.at(i)); }
template <class D> std::pair<D*,D*> varPlot<D>::Get2D(uint i){ return std::make_pair<D*,D*>(&(x.at(i)),&(y.at(i))); }
template <class D> uint varPlot<D>::Size() { return x.size(); }
template <class D> TObject* varPlot<D>::Plot(){ return plot; }
template <class D> void varPlot<D>::SetPlot(TObject* plot_){plot=plot_;}
template <class D> TObject* varPlot<D>::GetPlot(){return plot;}
template <class D> void varPlot<D>::SetName(TString name_){name=name_;}
template <class D> void varPlot<D>::ClearVectors(){x.clear(); y.clear();}

template <class D> void varPlot<D>::SetGM(TString group_, TString module_){
  group = group_;
  module = module_;
}

template <class D> void varPlot<D>::Fill(D valX, D valY, int i) {
  if (type!=kPlot1D && type!=kPlotGraph && type!=kPlot1DProf) {std::cout << "WRONG 1D " << name.Data() << std::endl; return;}
  if (type==kPlot1D)
    {
      if (i<0) x.push_back(valX);
      else {
	if (x.size()<=i) x.resize(i+1);
	x.at(i)=valX;
      }
      (dynamic_cast<TH1F*>(plot))->Fill(valX);
    }
  if (type==kPlot1DProf)
    {
      if (i<0) {x.push_back(valX); y.push_back(valY);}
      else
	{
	  if (x.size()<=i){
	    x.resize(i+1);
	    y.resize(i+1);
	  }
	  x.at(i)=valX;
	  y.at(i)=valY;
	}
      (dynamic_cast<TProfile*>(plot))->Fill(valX,valY);
    }
  if (type==kPlotGraph)
    {
      (dynamic_cast<TGraph*>(plot))->SetPoint(*iHistEntry,*iThisEntry,valX);
    }
}
template <class D> void varPlot<D>::Fill2D(D valX, D valY, D valZ, int i) {
  if (type!=kPlot2D && type!=kPlot2DProf) {std::cout << "WRONG 2D " << name.Data() << std::endl; return;}
  if (type==kPlot2D)
    {
      if (i<0) {x.push_back(valX); y.push_back(valY);}
      else
	{
	  if (x.size()<=i) {
	    x.resize(i+1);
	    y.resize(i+1);
	  }
	  x.at(i)=valX;
	  y.at(i)=valY;
	}
      (dynamic_cast<TH2F*>(plot))->Fill(valX,valY);
    }
  if (type==kPlot2DProf)
    {
      if (i<0) {x.push_back(valX); y.push_back(valY); z.push_back(valZ);}
      else {
	if (x.size()<=i) {
	  x.resize(i+1);
	  y.resize(i+1);
	  z.resize(i+1);
	}
	x.at(i)=valX;
	y.at(i)=valY;
	z.at(i)=valZ;
      }
      (dynamic_cast<TProfile2D*>(plot))->Fill(valX,valY,valZ);
    }
}

template <class D> varPlot<D>::varPlot()
{
  xptr = &x;
  yptr = &y;
  plot = NULL;
  waveform = NULL;
  doPlot = true;
  doProfile = false;
}

template <class D> varPlot<D>::varPlot(int *iThisEntry_, int *iHistEntry_, PlotType type_, bool profile_, uint size_):
  iThisEntry(iThisEntry_),
  iHistEntry(iHistEntry_),
  type(type_),
  doProfile(profile_)
{
  x.resize(size_);
  y.resize(size_);
  xptr = &x;
  yptr = &y;
  plot = NULL;
  waveform = NULL;
  doPlot = true;
  doProfile = false;
}

template <class D> varPlot<D>::~varPlot(){
  if (plot) delete plot;
  if (waveform) delete waveform;
}


float 
plotterTools::getMinimumP (TProfile * p)
{
  float min = p->GetBinContent (1) ;
  for (int i = 2 ; i <= p->GetNbinsX () ; ++i)
    {
      if (p->GetBinError (i) == 0) continue ;
     // cout << min << p->GetBinContent (i) << endl ;
      if (p->GetBinContent (i) < min) min = p->GetBinContent (i) ;
    }
  return min ;
}


float 
plotterTools::getMaximumP (TProfile * p)
{
  float max = p->GetBinContent (1) ;
  for (int i = 2 ; i <= p->GetNbinsX () ; ++i)
    {
      if (p->GetBinError (i) == 0) continue ;
//      cout << max << " " << p->GetBinContent (i) << endl ;
      if (p->GetBinContent (i) > max) max = p->GetBinContent (i) ;
    }
  return max ;
}



plotterTools::plotterTools(TString filename, TString outfname, TString outdname,TString triggerType){

  setPlotsFormat () ;
  inputFile_ = TFile::Open(filename);

  if( inputFile_==0 ) {
    std::cout << "ERROR! Din't find file " << filename << std::endl;
    std::cout << "Exiting." << std::endl;
    exit(11);
  }

  inputTree_ = (TChain*) inputFile_->Get("H4tree");
  outputFile_ = TFile::Open(outfname,"RECREATE");
  outputDir_=outdname;
  triggerType_=triggerType;

  fillFiberOrder();

  wantADCplots = false;
  wantDigiplots = false;

};

void plotterTools::initIntegrated(TString nameFile){

  gStyle->SetOptStat (0) ;
  isIntegratedNew=true;
  integratedFile_=TFile::Open(nameFile,"new");
  if(integratedFile_==NULL){
    isIntegratedNew=false;
    integratedFile_=TFile::Open(nameFile,"Update");
    }
  assert(integratedFile_!=NULL);

  map<TString, varPlot<float>*>::iterator it;

  TH1F* evt_info=NULL;
  TH1F* trg_info=NULL;

  TH1F* hX_info=NULL;
  TH1F* hY_info=NULL;

  TH1F* hSX_info=NULL;
  TH1F* hSY_info=NULL;


  it=varplots.find("beamPositionX");
  if(it!=varplots.end()){
    hX_info=(TH1F*)varplots["beamPositionX"]->GetPlot();
    hY_info=(TH1F*)varplots["beamPositionY"]->GetPlot();

  }

  it=varplots.find("nTotalEvts");
  if(it!=varplots.end()){
    evt_info=(TH1F*)varplots["nTotalEvts"]->GetPlot();
    trg_info=(TH1F*)varplots["fractionTakenTrigHisto"]->GetPlot();
  }

  it=varplots.find("beamPositionSmallX");
  if(it!=varplots.end()){
    hSX_info=(TH1F*)varplots["beamPositionSmallX"]->GetPlot();
    hSY_info=(TH1F*)varplots["beamPositionSmallY"]->GetPlot();

  }


  if(hX_info!=NULL){

      integratedPlots_["hodo_meanX_spill"]=(TH1F*)integratedFile_->Get("hodo_meanX_spill");
      integratedPlots_["hodo_meanY_spill"]=(TH1F*)integratedFile_->Get("hodo_meanY_spill");//add controls
      integratedPlots_["hodo_RMSX_spill"]=(TH1F*)integratedFile_->Get("hodo_RMSX_spill");
      integratedPlots_["hodo_RMSY_spill"]=(TH1F*)integratedFile_->Get("hodo_RMSY_spill");//add controls
     
      if(integratedPlots_["hodo_meanX_spill"]==NULL){
	integratedPlots_["hodo_meanX_spill"]=new TH1F("hodo_meanX_spill","hodo_meanX_spill",200,0,200);
	integratedPlots_["hodo_meanY_spill"]=new TH1F("hodo_meanY_spill","hodo_meanY_spill",200,0,200);
	integratedPlots_["hodo_RMSX_spill"]=new TH1F("hodo_RMSX_spill","hodo_RMSX_spill",200,0,200);
	integratedPlots_["hodo_RMSY_spill"]=new TH1F("hodo_RMSY_spill","hodo_RMSY_spill",200,0,200);


      }

    }
    if(evt_info!=NULL){
      integratedPlots_["DAQStatus_nTotalEvtsPerSpill"]=(TH1F*)integratedFile_->Get("DAQStatus_nTotalEvtsPerSpill");
      integratedPlots_["DAQStatus_nTotalEvtsPerSpillHisto"]=(TH1F*)integratedFile_->Get("DAQStatus_nTotalEvtsPerSpillHisto");
      integratedPlots_["DAQStatus_fractionTakenTrigPerSpill"]=(TH1F*)integratedFile_->Get("DAQStatus_fractionTakenTrigPerSpill");
      integratedPlots_["DAQStatus_triggerRateHisto"]=(TH1F*)integratedFile_->Get("DAQStatus_triggerRateHisto");
      integratedPlots_["DAQStatus_triggerRatePerSpill"]=(TH1F*)integratedFile_->Get("DAQStatus_triggerRatePerSpill");
      integratedPlots_["DAQStatus_growingEventPlot"]=(TH1F*)integratedFile_->Get("DAQStatus_growingEventPlot");
      if(integratedPlots_["DAQStatus_nTotalEvtsPerSpill"]==NULL){
	integratedPlots_["DAQStatus_nTotalEvtsPerSpill"]=new TH1F("DAQStatus_nTotalEvtsPerSpill","DAQStatus_nTotalEvtsPerSpill",200,0,200);
	integratedPlots_["DAQStatus_nTotalEvtsPerSpillHisto"]=new TH1F("DAQStatus_nTotalEvtsPerSpillHisto","DAQStatus_nTotalEvtsPerSpillHisto",2000,100,7000);
	integratedPlots_["DAQStatus_fractionTakenTrigPerSpill"]=new TH1F("DAQStatus_fractionTakenTrigPerSpill","DAQStatus_fractionTakenTrigPerSpill",200,0,200);
	integratedPlots_["DAQStatus_triggerRateHisto"]=new TH1F("DAQStatus_triggerRateHisto","DAQStatus_triggerRateHisto",2000,0,1000);
	integratedPlots_["DAQStatus_triggerRatePerSpill"]=new TH1F("DAQStatus_triggerRatePerSpill","DAQStatus_triggerRatePerSpill",2000,0,1000);
	integratedPlots_["DAQStatus_growingEventPlot"]=new TH1F("DAQStatus_growingEventPlot","DAQStatus_growingEventPlot",2000,0,1000);
      }

    }

    if(hSX_info!=NULL){

      integratedPlots_["hodoSmall_meanX_spill"]=(TH1F*)integratedFile_->Get("hodoSmall_meanX_spill");
      integratedPlots_["hodoSmall_meanY_spill"]=(TH1F*)integratedFile_->Get("hodoSmall_meanY_spill");//add controls
      integratedPlots_["hodoSmall_RMSX_spill"]=(TH1F*)integratedFile_->Get("hodoSmall_RMSX_spill");
      integratedPlots_["hodoSmall_RMSY_spill"]=(TH1F*)integratedFile_->Get("hodoSmall_RMSY_spill");//add controls

      if(integratedPlots_["hodoSmall_meanX_spill"]==NULL){
	integratedPlots_["hodoSmall_meanX_spill"]=new TH1F("hodoSmall_meanX_spill","hodoSmall_meanX_spill",200,0,200);
	integratedPlots_["hodoSmall_meanY_spill"]=new TH1F("hodoSmall_meanY_spill","hodoSmall_meanY_spill",200,0,200);
	integratedPlots_["hodoSmall_RMSX_spill"]=new TH1F("hodoSmall_RMSX_spill","hodoSmall_RMSX_spill",200,0,200);
	integratedPlots_["hodoSmall_RMSY_spill"]=new TH1F("hodoSmall_RMSY_spill","hodoSmall_RMSY_spill",200,0,200);

      }

    }



  if(hX_info!=NULL){

  int iBin=0;
  for(iBin=1;iBin<integratedPlots_["hodo_meanX_spill"]->GetNbinsX() && integratedPlots_["hodo_meanX_spill"]->GetBinContent(iBin)>0; ++iBin){}
  integratedPlots_["hodo_meanX_spill"]->SetBinContent(iBin,hX_info->GetMean());
  integratedPlots_["hodo_meanX_spill"]->SetBinError(iBin,hX_info->GetRMS());

  integratedPlots_["hodo_meanY_spill"]->SetBinContent(iBin,hY_info->GetMean());
  integratedPlots_["hodo_meanY_spill"]->SetBinError(iBin,hY_info->GetRMS());

  integratedPlots_["hodo_RMSX_spill"]->SetBinContent(iBin,hX_info->GetRMS());
  integratedPlots_["hodo_RMSY_spill"]->SetBinContent(iBin,hY_info->GetRMS());

  setAxisTitles(integratedPlots_["hodo_meanX_spill"], "nSpill","mean X" );
  setAxisTitles(integratedPlots_["hodo_meanY_spill"], "nSpill","mean Y" );

  setAxisTitles(integratedPlots_["hodo_RMSX_spill"], "nSpill","RMS X" );
  setAxisTitles(integratedPlots_["hodo_RMSY_spill"], "nSpill","RMS Y" );


  plotMe(integratedPlots_["hodo_meanX_spill"]);
  plotMe(integratedPlots_["hodo_meanY_spill"]);

  plotMe(integratedPlots_["hodo_RMSX_spill"]);
  plotMe(integratedPlots_["hodo_RMSY_spill"]);

  }

  if(hSX_info!=NULL){
    
    int iBin=0;
    for(iBin=1;iBin<integratedPlots_["hodoSmall_meanX_spill"]->GetNbinsX() && integratedPlots_["hodoSmall_meanX_spill"]->GetBinContent(iBin)>0; ++iBin){}
    integratedPlots_["hodoSmall_meanX_spill"]->SetBinContent(iBin,hX_info->GetMean());
    integratedPlots_["hodoSmall_meanX_spill"]->SetBinError(iBin,hX_info->GetRMS());
    
    integratedPlots_["hodoSmall_meanY_spill"]->SetBinContent(iBin,hY_info->GetMean());
    integratedPlots_["hodoSmall_meanY_spill"]->SetBinError(iBin,hY_info->GetRMS());

    integratedPlots_["hodoSmall_RMSX_spill"]->SetBinContent(iBin,hX_info->GetRMS());    
    integratedPlots_["hodoSmall_RMSY_spill"]->SetBinContent(iBin,hY_info->GetRMS());    
    
    setAxisTitles(integratedPlots_["hodoSmall_meanX_spill"], "nSpill","mean X" );
    setAxisTitles(integratedPlots_["hodoSmall_meanY_spill"], "nSpill","mean Y" );
    
    plotMe(integratedPlots_["hodoSmall_meanX_spill"]);
    plotMe(integratedPlots_["hodoSmall_meanY_spill"]);

    setAxisTitles(integratedPlots_["hodoSmall_RMSX_spill"], "nSpill","RMS X" );
    setAxisTitles(integratedPlots_["hodoSmall_RMSY_spill"], "nSpill","RMS Y" );
    
    plotMe(integratedPlots_["hodoSmall_RMSX_spill"]);
    plotMe(integratedPlots_["hodoSmall_RMSY_spill"]);


  }


  if(evt_info != NULL){

  int iBin=0;
  for(iBin=1;iBin<integratedPlots_["DAQStatus_nTotalEvtsPerSpill"]->GetNbinsX() && integratedPlots_["DAQStatus_nTotalEvtsPerSpill"]->GetBinContent(iBin)>0; ++iBin){}
  integratedPlots_["DAQStatus_nTotalEvtsPerSpill"]->SetBinContent(iBin,evt_info->GetEntries());
  setAxisTitles(integratedPlots_["DAQStatus_nTotalEvtsPerSpill"], "nSpill","nEvts" );
  plotMe(integratedPlots_["DAQStatus_nTotalEvtsPerSpill"]);



  integratedPlots_["DAQStatus_fractionTakenTrigPerSpill"]->SetBinContent(iBin,trg_info->GetMean());
  setAxisTitles(integratedPlots_["DAQStatus_fractionTakenTrigPerSpill"], "nSpill","fractionTakenTrig" );
  plotMe(integratedPlots_["DAQStatus_fractionTakenTrigPerSpill"]);


  integratedPlots_["DAQStatus_triggerRatePerSpill"]->SetBinContent(iBin,1000000*evt_info->GetEntries()/(timeEnd_[0]-timeStart_[0]));//it's in Hz
  setAxisTitles(integratedPlots_["DAQStatus_triggerRatePerSpill"],"nSpill" ,"trigger Rate (Hz)" );
  std::cout<<"NAME----"<<integratedPlots_["DAQStatus_triggerRatePerSpill"]->GetName();
  plotMe(integratedPlots_["DAQStatus_triggerRatePerSpill"]);

  int oldEvents=integratedPlots_["DAQStatus_growingEventPlot"]->GetBinContent(iBin-1);
  integratedPlots_["DAQStatus_growingEventPlot"]->SetBinContent(iBin,oldEvents+evt_info->GetEntries());
  setAxisTitles(integratedPlots_["DAQStatus_growingEventPlot"],"nSpill" ,"n Total Events" );
  plotMe(integratedPlots_["DAQStatus_growingEventPlot"]);


  gStyle->SetOptStat ("emr") ;


  integratedPlots_["DAQStatus_nTotalEvtsPerSpillHisto"]->Fill(evt_info->GetEntries());
  setAxisTitles(integratedPlots_["DAQStatus_nTotalEvtsPerSpillHisto"], "NEvts Per Spill","Entries" );
  plotMe(integratedPlots_["DAQStatus_nTotalEvtsPerSpillHisto"]);

  integratedPlots_["DAQStatus_triggerRateHisto"]->Fill(1000000*evt_info->GetEntries()/(timeEnd_[0]-timeStart_[0]));//it's in Hz
  //  integratedPlots_["triggerRateHisto"]->SetBinError(iBin,evt_info->GetRMS());
  setAxisTitles(integratedPlots_["DAQStatus_triggerRateHisto"], "trigger Rate (Hz)","Entries" );
  plotMe(integratedPlots_["DAQStatus_triggerRateHisto"]);


  

  }
  integratedFile_->cd();
  
  for(std::map<TString,TH1F*>::const_iterator out=integratedPlots_.begin();out!=integratedPlots_.end();++out)
    out->second->Write(out->first,TObject::kOverwrite);

  integratedFile_->Close();
}


void plotterTools::set_palette_fancy (){
  const Int_t NRGBs = 5;
  const Int_t NCont = 255;
  Float_t r[5];
  Float_t g[5];
  Float_t b[5];
  TColor* c[5];
  int Colors[5]={kBlack,kBlue+2,kBlue-3,kBlue-4,kWhite};
  for(int i=0;i<5;++i){
    c[i]=gROOT->GetColor(Colors[i]);
    c[i]->GetRGB(r[i],g[i],b[i]);
  }
  // Double_t stops[NRGBs] = { 0.00, 0.40, 0.50, 0.80, 1.00}; original
  Double_t stops[NRGBs] = { 0.00, 0.15, 0.50, 0.80, 1.00};
  Double_t red[NRGBs] = {r[4],r[3],r[2],r[1],r[0] };
  Double_t green[NRGBs] = {g[4],g[3],g[2],g[1],g[0] };
  Double_t blue[NRGBs] = {b[4],b[3],b[2],b[1],b[0] };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);
}

void plotterTools::set_palette_fancy2 (){
  const Int_t NRGBs = 5;
  const Int_t NCont = 255;

  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);
}

void plotterTools::set_palette_twocolor (int col1, int col2){
  const Int_t NRGBs = 2;
  const Int_t NCont = 255;
  Float_t r[NRGBs];
  Float_t g[NRGBs];
  Float_t b[NRGBs];
  TColor* c[NRGBs];
  int Colors[NRGBs]={col1,col2};
  for(int i=0;i<NRGBs;++i){
    c[i]=gROOT->GetColor(Colors[i]);
    c[i]->GetRGB(r[i],g[i],b[i]);
  }
  Double_t stops[NRGBs] = { 0.00, 1.00};
  Double_t red[NRGBs] = {r[0],r[1]};
  Double_t green[NRGBs] = {g[0],g[1]};
  Double_t blue[NRGBs] = {b[0],b[1]};
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);

}

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
    //    gStyle->SetOptStat ("emr") ;
    gStyle->SetOptFit (0001) ;
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

    // Margins:



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

    //    set_plot_blue () ;
    set_palette_fancy () ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void  plotterTools::plotMe (TH1F * histo, TString name)
{
  TString hname = (name==TString("")) ? histo->GetName () : name.Data();
  TString  canvasName =outputDir_+ "/"+ hname + ".png" ;
	
  TCanvas*  c1 = new TCanvas ("c1", "c1", 800, 800) ;
  string myName=histo->GetName();
  int stat=gStyle->GetOptStat();
  if ( myName.find("MULTILINE") !=string::npos )
	  	{
		if (myName[myName.size()-1] != '0') return; 
		gStyle->SetOptStat(0);
		//create and draw 
		TPad *p0= new TPad("p0","p0",0,0.0,1,.2); p0->SetTopMargin(0); p0->SetBottomMargin(.5);
		TPad *p1= new TPad("p1","p1",0,0.2,1,.3); p1->SetTopMargin(0); p1->SetBottomMargin(0);
		TPad *p2= new TPad("p2","p2",0,0.3,1,.4); p2->SetTopMargin(0); p2->SetBottomMargin(0);
		TPad *p3= new TPad("p3","p3",0,0.4,1,.5); p3->SetTopMargin(0); p3->SetBottomMargin(0);
		TPad *p4= new TPad("p4","p4",0,0.5,1,.6); p4->SetTopMargin(0); p4->SetBottomMargin(0);
		TPad *p5= new TPad("p5","p5",0,0.6,1,.7); p5->SetTopMargin(0); p5->SetBottomMargin(0);
		TPad *p6= new TPad("p6","p6",0,0.7,1,.8); p6->SetTopMargin(0); p6->SetBottomMargin(0);
		TPad *p7= new TPad("p7","p7",0,0.8,1,1.); p7->SetTopMargin(.5); p7->SetBottomMargin(0);
		p0->Draw();
		p1->Draw();
		p2->Draw();
		p3->Draw();
		p4->Draw();
		p5->Draw();
		p6->Draw();
		p7->Draw();

		TH1F *h0=histo;
		myName.erase(myName.size()-1); myName+= "1";
		cout<< "Getting Histo"<<myName<<endl;
		TH1F *h1=(TH1F*)(varplots[myName]->GetPlot());
		myName.erase(myName.size()-1); myName+= "2";
		cout<< "Getting Histo"<<myName<<endl;
		TH1F *h2=(TH1F*)(varplots[myName]->GetPlot());

		h0->SetLineColor(kBlue);
		h1->SetLineColor(kRed);
		h2->SetLineColor(kGreen);

		h0->SetFillColor(0);
		h1->SetFillColor(0);
		h2->SetFillColor(0);
		h0->SetFillStyle(0);
		h1->SetFillStyle(0);
		h2->SetFillStyle(0);
		h0->SetTitle("");
		h0->GetYaxis()->SetTitle("");
	
		h0->Scale(3);
		h1->Scale(2);
		h2->Scale(1);

		// draw
		p7->cd();
		h0->GetXaxis()->SetRangeUser(     0,625000-1);
		h0->DrawCopy("HIST");
		h1->Draw("HIST SAME");
		h2->Draw("HIST SAME");
		TLegend *l=new TLegend(0.35,.5,.65,.98);
			l->SetFillColor(0);
			l->SetFillStyle(0);
			l->SetBorderSize(0);
			l->AddEntry(h0,"Time0","F");
			l->AddEntry(h1,"Time1","F");
			l->AddEntry(h2,"Time2","F");
			l->Draw();
		p6->cd();
		h0->GetXaxis()->SetRangeUser(625000,1250000-1);
		h0->DrawCopy("HIST");
		h1->Draw("HIST SAME");
		h2->Draw("HIST SAME");
		p5->cd();
		h0->GetXaxis()->SetRangeUser(1250000,1875000-1);
		h0->DrawCopy("HIST");
		h1->Draw("HIST SAME");
		h2->Draw("HIST SAME");
		p4->cd();
		h0->GetXaxis()->SetRangeUser(1875000,2500000-1);
		h0->DrawCopy("HIST");
		h1->Draw("HIST SAME");
		h2->Draw("HIST SAME");
		p3->cd();
		h0->GetXaxis()->SetRangeUser(2500000,3125000-1);
		h0->DrawCopy("HIST");
		h1->Draw("HIST SAME");
		h2->Draw("HIST SAME");
		p2->cd();
		h0->GetXaxis()->SetRangeUser(3125000,3750000-1);
		h0->DrawCopy("HIST");
		h1->Draw("HIST SAME");
		h2->Draw("HIST SAME");
		p1->cd();
		h0->GetXaxis()->SetRangeUser(3750000,4375000-1);
		h0->DrawCopy("HIST");
		h1->Draw("HIST SAME");
		h2->Draw("HIST SAME");
		p0->cd();
		h0->GetXaxis()->SetRangeUser(4375000,5000000-1);
		h0->DrawCopy("HIST");
		h1->Draw("HIST SAME");
		h2->Draw("HIST SAME");
		}
  else
  	histo->Draw () ;
  c1->Print (canvasName, "png") ;
  gStyle->SetOptStat(stat);


  delete c1 ;
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void  plotterTools::plotMe (TH2F * histo, TString name)
{
  gStyle->SetPadTopMargin(0.10);
  gStyle->SetPadBottomMargin(0.10);
  gStyle->SetPadLeftMargin(0.10);
  gStyle->SetPadRightMargin(0.15);
  
  TString hname = (name==TString("")) ? histo->GetName () : name.Data();
  TString  canvasName = outputDir_+ "/"+hname + ".png" ;
  TCanvas*  c1 = new TCanvas ("c1", "c1", 800, 800) ;

  if (hname.Contains("MatrixView")){
    set_palette_twocolor(kWhite,kCyan);
    histo->SetMarkerColor(kBlack);
    histo->SetStats(kFALSE);
    histo->Draw ("colz text e1") ;
  }
  else if (hname.Contains("map")){
    set_palette_fancy2();
    histo->SetMarkerColor(kBlack);
    histo->SetStats(kFALSE);
    histo->Draw ("colz text") ;
  }
  else if (hname.Contains("_pulse")){
    histo->GetYaxis()->SetRangeUser(0,3800);
    histo->SetStats(kFALSE);
    histo->Draw ("colz") ;
  }
  else histo->Draw ("colz") ;
  
  std::cout << "plotting " << name << std::endl;
  c1->Print (canvasName, "png") ;
  set_palette_fancy();
  
  delete c1 ;
  return ;

}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void  plotterTools::plotMe (TGraph * graph, const TString & name)
{
  TString  canvasName = outputDir_+ "/"+name + ".png" ;
  TCanvas*  c1 = new TCanvas ("c1", "c1", 800, 800) ;
  graph->SetMarkerStyle (8) ;
  graph->SetMarkerSize (1.5) ;
  graph->SetMarkerColor (kBlue) ;  
  graph->Draw ("AP") ;
  c1->Print (canvasName, "png") ;
  delete c1 ;
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void  plotterTools::setAxisTitles (TH1 * histo, const TString  xTitle, const TString  yTitle) 
{
  histo->GetXaxis ()->SetTitle (xTitle) ;
  histo->GetYaxis ()->SetTitle (yTitle) ;
  return ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

void  plotterTools::setAxisTitles (TH2 * histo, const TString  xTitle, const TString  yTitle) 
{
  histo->GetXaxis ()->SetTitle (xTitle) ;
  histo->GetYaxis ()->SetTitle (yTitle) ;
  return ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

void  plotterTools::setAxisTitles (TGraph * histo, const TString  xTitle, const TString  yTitle) 
{
  histo->GetXaxis ()->SetTitle (xTitle) ;
  histo->GetYaxis ()->SetTitle (yTitle) ;
  return ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

void plotterTools::computeVariable(TString name){
 
 if(name=="triggerEff"){
   //   varplots[name]->Fill(((float)treeStruct_.scalerWord[2]/treeStruct_.scalerWord[1]),1.);
 }else if(name=="nEvts"){
   varplots[name]->Fill(((float)treeStruct_.evtNumber),1.);
  }else if(name=="nTrigSPS"){
   //   varplots[name]->Fill(((float)treeStruct_.scalerWord[1]),1.);
  }else if(name=="nTrigSPSVsnTrig"){
   //   varplots[name]->Fill2D(((float)treeStruct_.scalerWord[1]),((float)treeStruct_.scalerWord[2]),1.);
  }else if(name=="nTrigSPSVsnTrig3D"){
   //   varplots[name]->Fill2D(((float)treeStruct_.scalerWord[1]),((float)treeStruct_.scalerWord[2]),1.); // FIX
//    variablesContainer_[variablesIterator_[name]][0]=((float)treeStruct_.scalerWord[1]);
//    variablesContainer_[variablesIterator_[name]][1]=((float)treeStruct_.scalerWord[2]);
//    variablesContainer_[variablesIterator_[name]][2]=((float)treeStruct_.scalerWord[0]);
 }else if(name=="nFibersOnX1"){

   int fibersOn=0;
   for(int i=0;i<64;i++){
     if(fibersOn_[hodoX1][i]==1) fibersOn++;
   }
   varplots[name]->Fill(fibersOn,1.);

 }else if(name=="nFibersOnY1"){

   int fibersOn=0;
   for(int i=0;i<64;i++){
     if(fibersOn_[hodoY1][i]==1) fibersOn++;
   }
   varplots[name]->Fill(fibersOn,1.);

 }else if(name=="nFibersOnX2"){

   int fibersOn=0;
   for(int i=0;i<64;i++){
     if(fibersOn_[hodoX2][i]==1) fibersOn++;
   }
   varplots[name]->Fill(fibersOn,1.);

 }else if(name=="nFibersOnY2"){

   int fibersOn=0;
   for(int i=0;i<64;i++){
     if(fibersOn_[hodoY2][i]==1) fibersOn++;
   }
   varplots[name]->Fill(fibersOn,1.);

 }else if(name=="beamProfileX1"){

   for(int i=0;i<64;i++){
     if(fibersOn_[hodoX1][i]==1) varplots[name]->Fill(1,i);
     else varplots[name]->Fill(0,1.,i);
   }

 }else if(name=="beamProfileY1"){

   for(int i=0;i<64;i++){
     if(fibersOn_[hodoY1][i]==1) varplots[name]->Fill(1,i);
     else varplots[name]->Fill(0,1.,i);
   }

 }else if(name=="beamProfileX2"){

   for(int i=0;i<64;i++){
     if(fibersOn_[hodoX2][i]==1) varplots[name]->Fill(1,i);
     else varplots[name]->Fill(0,1.,i);
   }

 }else if(name=="beamProfileY2"){

   for(int i=0;i<64;i++){
     if(fibersOn_[hodoY2][i]==1) varplots[name]->Fill(1,i);
     else varplots[name]->Fill(0,1.,i);
   }

 }else if(name=="beamProfileDrawX1"){

   for(int i=0;i<64;i++){
     if(fibersOn_[hodoX1][i]==1) varplots[name]->Fill(i,1.);
   }

 }else if(name=="beamProfileDrawY1"){

   for(int i=0;i<64;i++){
     if(fibersOn_[hodoY1][i]==1) varplots[name]->Fill(i,1.);
   }

 }else if(name=="beamProfileDrawX2"){

   for(int i=0;i<64;i++){
     if(fibersOn_[hodoX2][i]==1) varplots[name]->Fill(i,1.);
   }

 }else if(name=="beamProfileDrawY2"){

   for(int i=0;i<64;i++){
     if(fibersOn_[hodoY2][i]==1) varplots[name]->Fill(i,1.);
   }

 }else if(name=="beamPositionX1"){

   float pos=0;
   int nFibersOn=0;
   for (int i=0;i<64;++i){   
       if(fibersOn_[hodoX1][i]==1){
	 nFibersOn++;
	 pos+=i; 
       }
     }
   if(nFibersOn>1){
     pos=pos/nFibersOn;
   }else{
     pos=-1;
   }
   varplots[name]->Fill(pos,1.);
 }else if(name=="WCvsHodo"){

   float pos=0;
   int nFibersOn=0;
   for (int i=0;i<64;++i){   
       if(fibersOn_[hodoX1][i]==1){
	 nFibersOn++;
	 pos+=i; 
       }
     }
   if(nFibersOn>1){
     pos=pos/nFibersOn;
   }else{
     pos=-1;
   }
   varplots[name]->Fill2D(pos,tdc_recox,1.);


/*//TEST
 }else if(name=="beamPositionTEST"){

   float pos=0;
   int nFibersOn=0;
   for (int i=0;i<64;++i){   
       if(fibersOn_[hodoX1][i]==1){
	 nFibersOn++;
	 pos+=i; 
       }
     }
   if(nFibersOn>1){
     pos=pos/nFibersOn;
   }else{
     pos=-1;
   }
   varplots[name]->Fill(pos,1.);
//TEST END*/

 }else if(name=="beamPositionY1"){

   float pos=0;
   int nFibersOn=0;
   for (int i=0;i<64;++i){   
       if(fibersOn_[hodoY1][i]==1){
	 nFibersOn++;
	 pos+=i; 
       }
     }
   if(nFibersOn>1){
     pos=pos/nFibersOn;
   }else{
     pos=-1;
   }
   varplots[name]->Fill(pos,1.);


 }else if(name=="beamPositionX2"){

   float pos=0;
   int nFibersOn=0;
   for (int i=0;i<64;++i){   
       if(fibersOn_[hodoX2][i]==1){
	 nFibersOn++;
	 pos+=i; 
       }
     }
   if(nFibersOn>1){
     pos=pos/nFibersOn;
   }else{
     pos=-1;
   }
   varplots[name]->Fill(pos,1.);


 }else if(name=="beamPositionY2"){

   float pos=0;
   int nFibersOn=0;
   for (int i=0;i<64;++i){   
       if(fibersOn_[hodoY2][i]==1){
	 nFibersOn++;
	 pos+=i; 
       }
     }
   if(nFibersOn>1){
     pos=pos/nFibersOn;
   }else{
     pos=-1;
   }
   varplots[name]->Fill(pos,1.);


 }else if(name == "beamProfileSmallX"){//small hodo
   
   for(int i =0 ;i<nFibersSmallHodo;i++){
     if(fibersOnSmall_[hodoSmallX][i]==1) varplots[name]->Fill(1,1.,i);
     else varplots[name]->Fill(0,i);
   }
 }else if(name == "beamProfileSmallY"){

   for(int i =0 ;i<nFibersSmallHodo;i++){
     if(fibersOnSmall_[hodoSmallY][i]==1) varplots[name]->Fill(1,1.,i);
     else varplots[name]->Fill(0,i);
   }
 }else if(name == "beamProfileDrawSmallX"){//small hodo
   
   for(int i =0 ;i<nFibersSmallHodo;i++){
     if(fibersOnSmall_[hodoSmallX][i]==1) varplots[name]->Fill(i,1.);
   }
 }else if(name == "beamProfileDrawSmallY"){

   for(int i =0 ;i<nFibersSmallHodo;i++){
     if(fibersOnSmall_[hodoSmallY][i]==1) varplots[name]->Fill(i,1.);
   }
 }else if(name=="nFibersOnSmallX"){

   int fibersOn=0;
   for(int i=0;i<nFibersSmallHodo;i++){
     if(fibersOnSmall_[hodoSmallX][i]==1) fibersOn++;
   }
   varplots[name]->Fill(fibersOn,1.);

 }else if(name=="nFibersOnSmallY"){

   int fibersOn=0;
   for(int i=0;i<nFibersSmallHodo;i++){
     if(fibersOnSmall_[hodoSmallY][i]==1) fibersOn++;
   }
   varplots[name]->Fill(fibersOn,1.);

 }else if(name=="beamPositionSmallX"){

   float pos=0;
   int nFibersOn=0;
   
   for (int i=0;i<8;++i){   
     if(fibersOnSmall_[hodoSmallX][i]==1){
       nFibersOn++;
       pos+=i; 
     }
   }


   if(nFibersOn>1){
     pos=pos/nFibersOn;
   }else{
     pos=-1;
   }
   varplots[name]->Fill(pos,1.);

 }else if(name=="beamPositionSmallY"){

   float pos=0;
   int nFibersOn=0;
   
   for (int i=0;i<8;++i){   
     if(fibersOnSmall_[hodoSmallY][i]==1){
       nFibersOn++;
       pos+=i; 
     }
   }


   if(nFibersOn>1){
     pos=pos/nFibersOn;
   }else{
     pos=-1;
   }
   varplots[name]->Fill(pos,1.);

 }else if(name=="beamPositionX"){

   float pos=0;
   int nFibersOn=0;
   for(int j=0;j<4;j++){
     if(j!=hodoX1 && j!=hodoX2)continue;
     for (int i=0;i<64;++i){   
       if(fibersOn_[j][i]==1){
	 nFibersOn++;
	 pos+=i; 
       }
     }
   }

   if(nFibersOn>1){
     pos=pos/nFibersOn;
   }else{
     pos=-1;
   }
   varplots[name]->Fill(pos,1.);

 }else if(name=="beamPositionY"){

   float pos=0;
   int nFibersOn=0;
   for(int j=0;j<4;j++){
     if(j!=hodoY1 && j!=hodoY2)continue;
     for (int i=0;i<64;++i){   
       if(fibersOn_[j][i]==1){
	 nFibersOn++;
	 pos+=i; 
       }
     }
   }

   if(nFibersOn>1){
     pos=pos/nFibersOn;
   }else{
     pos=-1;
   }
   varplots[name]->Fill(pos,1.);

 }else if(name=="fractionTakenTrig"){//DAQ Status
   //   varplots[name]->Fill(((float)treeStruct_.scalerWord[2]/treeStruct_.scalerWord[1]),1.); before it was 2 and 1
   varplots[name]->Fill(((float)treeStruct_.scalerWord[4]/treeStruct_.scalerWord[3]),1.);
 }else if(name=="fractionTakenTrigHisto"){//DAQ Status
   varplots[name]->Fill(((float)treeStruct_.scalerWord[4]/treeStruct_.scalerWord[3]),1.);
 }else if(name=="deltaTime10"){
   varplots[name]->Fill(((int64_t)treeStruct_.evtTime[1]-(int64_t)treeStruct_.evtTime[0])-((int64_t)timeStart_[1]-(int64_t)timeStart_[0]),1.);

 }else if(name=="deltaTime20"){
   varplots[name]->Fill(((int64_t)treeStruct_.evtTime[2]-(int64_t)treeStruct_.evtTime[0])-((int64_t)timeStart_[2]-(int64_t)timeStart_[0]),1.);

 }else if(name=="deltaTime21"){
   varplots[name]->Fill(((int64_t)treeStruct_.evtTime[2]-(int64_t)treeStruct_.evtTime[1])-((int64_t)timeStart_[2]-(int64_t)timeStart_[1]),1.);

 }else if(name=="nTotalEvts"){
   varplots[name]->Fill(((float)1.),1.);
  
 }else if(name=="MULTILINE_time0"){
   varplots[name]->Fill((int64_t)treeStruct_.evtTime[0]-(int64_t)timeStart_[0],1.);
  
 }else if(name=="MULTILINE_time1"){
   varplots[name]->Fill((int64_t)treeStruct_.evtTime[1]-(int64_t)timeStart_[1],1.);
  
 }else if(name=="MULTILINE_time2"){
   varplots[name]->Fill((int64_t)treeStruct_.evtTime[2]-(int64_t)timeStart_[2],1.);
  }
 else if(name=="TDCinputTime1"){//TDC
   for (uint j=0; j<tdc_readings[0].size() && j<MaxTdcReadings; j++) varplots[name]->Fill(tdc_readings[0].at(j),1.);
 }
 else if(name=="TDCinputTime2"){
   for (uint j=0; j<tdc_readings[1].size() && j<MaxTdcReadings; j++) varplots[name]->Fill(tdc_readings[1].at(j),1.);
 }
 else if(name=="TDCinputTime3"){
   for (uint j=0; j<tdc_readings[2].size() && j<MaxTdcReadings; j++) varplots[name]->Fill(tdc_readings[2].at(j),1.);
 }
 else if(name=="TDCinputTime4"){
   for (uint j=0; j<tdc_readings[3].size() && j<MaxTdcReadings; j++) varplots[name]->Fill(tdc_readings[3].at(j),1.);
 }
 else if(name=="TDCrecoX"){
   varplots[name]->Fill(tdc_recox,1.);
 }
 else if(name=="TDCrecoY"){
   varplots[name]->Fill(tdc_recoy,1.);
 }else if(name=="nTdcChannels"){
   varplots[name]->Fill(treeStruct_.nTdcChannels,1.);
 } else if(name=="TDCrecoPos"){
   varplots[name]->Fill2D(tdc_recox,tdc_recoy,1.);
 }

 {
   std::map<TString,UInt_t*>::const_iterator it = adc_channelnames.find(name);
   if (it!=adc_channelnames.end()){
     varplots[name]->Fill(*(it->second),1.);
   }
 }



}


void plotterTools::fillObjects(){

  fillHodo();
  fillTdc();

}

void plotterTools::fillHodo(){


   for(int i=0;i<nPlanesHodo;++i){
     for(int j=0;j<nFibersHodo;++j){
       fibersOn_[i][j]=0;
     }
   }

   for(int i =0 ; i <nPlanesSmallHodo;++i){
     for(int j=0; j<nFibersSmallHodo;j++){
       fibersOnSmall_[i][j]=0;
     }
   }


   for(uint i=0;i<treeStruct_.nPatterns;++i){

     //     if(treeStruct_.patternBoard[i]==0x08030001 || treeStruct_.patternBoard[i]==0x08030002){  //in 2014 setup the board id was  0x08030002 maybe the crateid was different
     if(treeStruct_.patternBoard[i]==0x08020001 || treeStruct_.patternBoard[i]==0x08020002){
       int pos = -1; // here is where the real hodoscope mapping is done
       if (treeStruct_.patternBoard[i]==0x08020001){
	 pos = (treeStruct_.patternChannel[i]<2) ? hodoY2 : hodoX2;
       }
       else if (treeStruct_.patternBoard[i]==0x08020002){
	 pos = (treeStruct_.patternChannel[i]<2) ? hodoY1 : hodoX1;
       }
       std::vector<int> *fiberorder =(bool)( treeStruct_.patternChannel[i]&0b1) ? &fiberOrderB : &fiberOrderA;
       
       for (unsigned int j=0; j<32; j++){
	 bool thisfibon = (treeStruct_.pattern[i]>>j)&0b1;
	 fibersOn_[pos][fiberorder->at(j)-1]=thisfibon;
       }


//       // VERSION FOR THE WRONG CABLING OF THE HODOSCOPE
//
//       int planecouple = (treeStruct_.patternBoard[i]==0x08030001) ? 0 : 1;
//       for (unsigned int j=0; j<32; j++){
//	 //
//	 bool isX=(planecouple ==0);
//	 bool isY=(planecouple ==1);
//
//	 bool is1= ( ( treeStruct_.patternChannel[i]/2) == 0);
//	 bool is2= ( ( treeStruct_.patternChannel[i]/2) == 1);
//	 bool isA= ( ( treeStruct_.patternChannel[i]%2) == 0);
//	 bool isB= ( ( treeStruct_.patternChannel[i]%2) == 1);
//	 bool isC=false;
//	 bool isD=false;
//
//	 // this will map X1Y2 and X2Y1 change Y2 <-> X2
//	 if( isX && is2 ) { isX=false; isY=true;}
//	 else if( isY && is2 ) { isY=false; isX=true;}
//	 
//	 if (isX and is1 ) 
//	   {
//	     if (isA) { 
//	       isA=false;  //offset=32;
//	       isB=false;
//	       isC=false; //offset=32;
//	       isD=true; 
//	     }
//	     else if (isB) { 
//	       isB=false; //isA=true; 
//	       isA=false;
//	       isC=true;
//	     }
//	   }
//
//	 if (isX and is2)
//	   {
//	     if (isB) { isB=false;isD=true; }
//	   }
//	 if (!isA and !isB and !isC and !isD )
//	   {
//	     printf("No Fiber mapping\n");
//	     exit(0);
//	   }
//	 std::vector<int> *fiberorder ;
//	 int n=0;
//	 if (isA) { fiberorder = &fiberOrderA; ++n;}
//	 if (isB) { fiberorder = &fiberOrderB; ++n;}
//	 if (isC) { fiberorder = &fiberOrderC; ++n;}
//	 if (isD) { fiberorder = &fiberOrderD; ++n;}
//
//	 if (n != 1) printf("Number of matching is %d\n",n);
//
//	 bool thisfibon = (treeStruct_.pattern[i]>>j)&0b1;
//
//
//	 int pos=0;
//	 if (isY) pos+=2;
//	 if (is2) pos+=1;
//
//	 fibersOn_[pos][fiberorder->at(j)-1]=thisfibon;
//
//       }

     }
     //hodo read out by pattern unt
//    else if(treeStruct_.patternBoard[i]==0x08010001){
//    
//      if(treeStruct_.patternChannel[i]!=0) continue;
//
//      WORD wordX=(treeStruct_.pattern[i]& 0x0000FF00)>>8;
//      WORD wordY= (treeStruct_.pattern[i] & 0x000000FF);
//
//      for(int j=0;j<8;j++){
//	 fibersOnSmall_[0][j]=(bool)((wordX>>j)&0b1);
//	 fibersOnSmall_[1][j]=(bool)((wordY>>j)&0b1);
//	 //	 std::cout<<fibersOnSmall_[0][i]<<" "<<fibersOnSmall_[1][i]<<"----";
//      }
//    }


   
   }


   //hodo readout by adc
   float pedSmallX[8];
   float pedSmallY[8];
   float rmsSmallX[8];
   float rmsSmallY[8];

   int firstChannel=-1;   
   bool isFirstChannel=false;
   for (Int_t i=0; i<treeStruct_.nAdcChannels && i<MAX_ADC_CHANNELS; i++){
    if(treeStruct_.adcBoard[i]==0x6010001){  
      if(!isFirstChannel){
	isFirstChannel=true;
	firstChannel=i;
      }
       if(i==firstChannel){


       pedSmallX[0]=99.53;
       pedSmallX[1]=78.12;
       pedSmallX[2]=109;
       pedSmallX[3]=94.3;
       pedSmallX[4]=98.62;
       //      pedSmallX[3]=0.;
       //      pedSmallX[4]=0.;
       pedSmallX[5]=109.4;
       pedSmallX[6]=87.76;
       pedSmallX[7]=93.61;

       pedSmallY[0]=98.19;
       pedSmallY[1]=111.1;
       pedSmallY[2]=98.93;
       pedSmallY[3]=97.98;
       pedSmallY[4]=96.62;
       pedSmallY[5]=98.08;
       pedSmallY[6]=86.29;
       pedSmallY[7]=113.6;


       rmsSmallX[0]=0.5;
       rmsSmallX[1]=0.5;
       rmsSmallX[2]=0.5;
       rmsSmallX[3]=0.5;
       rmsSmallX[4]=0.5;
       rmsSmallX[5]=0.5;
       rmsSmallX[6]=0.5;
       rmsSmallX[7]=0.5;
		    
       rmsSmallY[0]=0.5;
       rmsSmallY[1]=0.5;
       rmsSmallY[2]=0.5;
       rmsSmallY[3]=0.5;
       rmsSmallY[4]=0.5;
       rmsSmallY[5]=0.5;
       rmsSmallY[6]=0.5;
       rmsSmallY[7]=0.5;
       }
       if(i-firstChannel<16){
	 //assuming channel 0 first channel of Y
	 if (i-firstChannel<8){
	   fibersOnSmall_[1][i-firstChannel]=(bool)(treeStruct_.adcChannel[i]>pedSmallX[i-firstChannel]+2*rmsSmallX[i-firstChannel]);
	 }else{
	   fibersOnSmall_[0][i-firstChannel-8]=(bool)(treeStruct_.adcChannel[i]>pedSmallY[i-firstChannel-8]+2*rmsSmallY[i-firstChannel-8]);
	 }

       }

     }


   }
}


void plotterTools::fillTdc(){

  for (uint j=0; j<MaxTdcChannels; j++){
    tdc_readings[j].clear();
  }
  tdc_recox=-999;
  tdc_recoy=-999;

  for (uint i=0; i<treeStruct_.nTdcChannels; i++){
    //    if (treeStruct_.tdcBoard[i]==0x07020001 && treeStruct_.tdcChannel[i]<MaxTdcChannels){
    if (treeStruct_.tdcChannel[i]<MaxTdcChannels){
      tdc_readings[treeStruct_.tdcChannel[i]].push_back((float)treeStruct_.tdcData[i]);
    }
  }

  
  if (tdc_readings[wcXl].size()!=0 && tdc_readings[wcXr].size()!=0){
    float TXl = *std::min_element(tdc_readings[wcXl].begin(),tdc_readings[wcXl].begin()+tdc_readings[wcXl].size());
    float TXr = *std::min_element(tdc_readings[wcXr].begin(),tdc_readings[wcXr].begin()+tdc_readings[wcXr].size());
    float X = (TXr-TXl)*0.005; // = /40./5./10. //position in cm 0.2mm/ns with 25ps LSB TDC
    tdc_recox = X;
  }
  if (tdc_readings[wcYd].size()!=0 && tdc_readings[wcYu].size()!=0){
    float TYd = *std::min_element(tdc_readings[wcYd].begin(),tdc_readings[wcYd].begin()+tdc_readings[wcYd].size());
    float TYu = *std::min_element(tdc_readings[wcYu].begin(),tdc_readings[wcYu].begin()+tdc_readings[wcYu].size());
    float Y = (TYu-TYd)*0.005; // = /40./5./10. //position in cm 0.2mm/ns with 25ps LSB TDC
    tdc_recoy = Y;
  }

}

void plotterTools::initAdcChannelNames(int nBinsHistory){

  group_ = "ADC";

  adc_channelnames.clear();

  std::map<int,int> physchmap; // map[physical_number]=logical_number
  for (UInt_t i=0; i<treeStruct_.nAdcChannels && i<MAX_ADC_CHANNELS; i++){
    physchmap[i]=i;
  }
  physchmap[24]=13; // channel BGO 14 is plugged to ADC ch. 25 (_24 in the file)
  physchmap[13]=24;

  for (UInt_t i=0; i<treeStruct_.nAdcChannels && i<MAX_ADC_CHANNELS; i++){
    TString name("ADC_board_");
    for (Int_t j=3; j>=0; j--){
      UInt_t field = ((treeStruct_.adcBoard[i])>>(8*j))&(0x000000FF);
      name+=field;
    }
    name+='_';
    name+=physchmap[treeStruct_.adcChannel[i]];
    adc_channelnames.insert(make_pair(name,&(treeStruct_.adcData[i])));
    addPlot(1,name.Data(),4096,0,4096,"1D",group_,module_);
    name.Append("_hist");
    adc_channelnames.insert(make_pair(name,&(treeStruct_.adcData[i])));
    addPlot(0,name.Data(), nBinsHistory, "history", group_,module_);
  }

  //  addPlot(1,"MatrixView",5,-2.5,2.5,5,-2.5,2.5,"X view from back","Y view from back","2D", group_, module_,false,true);

}


template <class T>
set<int>
plotterTools::listElements (T * array, int Nmax)
{
  set<int> elements ;
  for (int i = 0 ; i < Nmax ; ++i)
    {
      if (elements.find (array[i]) != elements.end ()) continue ;
      elements.insert (array[i]) ;
    }
  return elements ;
}


void plotterTools::initDigiPlots(){
  group_ = "digitizer";

  std::set<int> channels = listElements (treeStruct_.digiChannel,  treeStruct_.nDigiSamples) ;
  std::set<int> groups = listElements (treeStruct_.digiGroup,  treeStruct_.nDigiSamples) ;
  
  int xNbins = 1024 ;
  float xmin = 0 ;
  float xmax = 1024 ;

  int yNbins = 4096;
  float ymin = 0;
  float ymax = 4096;

  cout << "channel size " << channels.size() << endl; 
  cout << "Digigroup value is " << treeStruct_.digiGroup << endl;
  cout << "Digigroup position 0 is " << treeStruct_.digiGroup[0] << endl;
  cout << "Groups.size is " <<  groups.size() << endl;
  cout << "nActivedigi channels value is " << nActiveDigitizerChannels << endl;
  cout << "treeStruct_.nDigisamples is " << treeStruct_.nDigiSamples << endl;
  for (set<int>::iterator iGroup = groups.begin () ; 
       iGroup != groups.end () ; ++iGroup)
    {
	cout << "iGroup = " << *iGroup << endl;
      for (set<int>::iterator iChannel = channels.begin () ; 
           iChannel != channels.end () ; ++iChannel)
        { //Note to self, for run 7290, which should have 25 channels, only 7 are registered?
	  if (*iChannel>=nActiveDigitizerChannels) continue;

          TString name = getDigiChannelName(*iGroup,*iChannel);
	  int nametest = *iChannel;
          addPlot(1,Form("%s_pulse",name.Data()), xNbins, xmin, xmax, yNbins, ymin, ymax, "time", "voltage", "2D", group_, module_, 1, true) ;
	  addPlot(1,Form("%s_pedestal",name.Data()),4096,0,4096,"1D",group_,module_);
	  addPlot(1,Form("%s_pedestal_rms",name.Data()),200,0,50,"1D",group_,module_);
	  addPlot(1,Form("%s_max_amplitude",name.Data()),300,0,5000,"1D",group_,module_);
	  addPlot(1,Form("%s_charge_integrated",name.Data()),200,0,5e4,"1D",group_,module_);
	  addPlot(0,Form("%s_charge_integrated_vs_hodoX1",name.Data()),65,-1,64,-999999,999999,"1DProf",group_,module_);
	  addPlot(0,Form("%s_charge_integrated_vs_hodoY1",name.Data()),65,-1,64,-999999,999999,"1DProf",group_,module_);
	  addPlot(0,Form("%s_charge_integrated_vs_hodoX2",name.Data()),65,-1,64,-999999,999999,"1DProf",group_,module_);
	  addPlot(0,Form("%s_charge_integrated_vs_hodoY2",name.Data()),65,-1,64,-999999,999999,"1DProf",group_,module_);

	  addPlot(1,Form("%s_charge_integrated_vs_TDCrecoX",name.Data()),66,-33,33,-999999,999999,"1DProf",group_,module_);
	  addPlot(1,Form("%s_charge_integrated_vs_TDCrecoY",name.Data()),66,-33,33,-999999,999999,"1DProf",group_,module_);
          //addPlot(1,Form("MAXIMUMPLOTTEST",name.Data()),66,-33,33,-999999,999999,"1DProf",group_,module_);

	  addPlot(0,Form("%s_time_at_max",name.Data()),xNbins,xmin,xmax,"1D",group_,module_);
	  addPlot(0,Form("%s_time_at_frac30",name.Data()),xNbins,xmin,xmax,"1D",group_,module_);
	  addPlot(0,Form("%s_time_at_frac50",name.Data()),xNbins,xmin,xmax,"1D",group_,module_);
		  
	  //cout << "Here is name.Data() " << name.Data() << endl;
 	  //cout << "Here is nametest " << nametest << endl; //nametest works up here...

	  TString thisname = Form("%s_%s",name.Data(),"pulse");
 	  varplots[thisname]->waveform = new Waveform();
        }
    }
//  addPlot(1,"MAXIMUMPLOTTEST",300,0,5000,"1D",group_,module_);
  addPlot(1,"MAXIMUMPLOTTEST",66,-33,33,-999999,999999,"1DProf",group_,module_);
  addPlot(0,Form("allCh_charge_integrated_map"), 8, 0, 8, 8, 0, 8, -999999, 999999, "x", "y", "2DProf", group_, module_, 1, true) ;
  addPlot(0,Form("allCh_max_amplitude_map"), 8, 0, 8, 8, 0, 8, -999999, 999999, "x", "y", "2DProf", group_, module_, 1, true) ;
  addPlot(0,Form("allCh_pedestal_map"), 8, 0, 8, 8, 0, 8, 3200., 3800., "x", "y", "2DProf", group_, module_, 1, true) ;
  addPlot(0,Form("allCh_pedestal_rms_map"), 8, 0, 8, 8, 0, 8, 2.0, 4.0, "x", "y", "2DProf", group_, module_, 1, true) ;
  //addPlot(1,"MatrixViewCeF3",2,-1,1,2,-1,1,"X view from back","Y view from back","2D", group_, module_,false,true);
}

TString plotterTools::getDigiChannelName(int group, int channel){
  //TString name = "digi_gr" ;
  //name += group ;
  //name += "_ch" ;
  //name += channel ;
  TString name = Form("digi_ch%02d",group*8+channel) ;
  return name;
}


int plotterTools::getDigiChannelX(TString name){
  std::string nameStr = std::string(name);
  std::size_t pos = nameStr.find("ch");
  std::string chStr = nameStr.substr(pos+2,2);
  int chId = atoi(chStr.c_str());
  if(chId >= 0 && chId < 32) return chId;
/*  if( chId ==  0 ) return 0;
  if( chId ==  1 ) return 1;
  if( chId ==  2 ) return 3;
  if( chId ==  3 ) return 4;
  if( chId ==  4 ) return 6;
  if( chId ==  5 ) return 7;
  if( chId ==  6 ) return 2;
  if( chId ==  7 ) return 3;
  if( chId ==  8 ) return 4;
  if( chId ==  9 ) return 5;
  if( chId == 10 ) return 0;
  if( chId == 11 ) return 2;
  if( chId == 12 ) return 3;
  if( chId == 13 ) return 4;
  if( chId == 14 ) return 5;
  if( chId == 15 ) return 7;
  if( chId == 16 ) return 0;
  if( chId == 17 ) return 2;
  if( chId == 18 ) return 3;
  if( chId == 19 ) return 4;
  if( chId == 20 ) return 5;
  if( chId == 21 ) return 7;
  if( chId == 22 ) return 2;
  if( chId == 23 ) return 3;
  if( chId == 24 ) return 4;
  if( chId == 25 ) return 5;
  if( chId == 26 ) return 0;
  if( chId == 27 ) return 1;
  if( chId == 28 ) return 3;
  if( chId == 29 ) return 4;
  if( chId == 30 ) return 6;
  if( chId == 31 ) return 7; */
  return -1;
}


int plotterTools::getDigiChannelY(TString name){
  std::string nameStr = std::string(name);
  std::size_t pos = nameStr.find("ch");
  std::string chStr = nameStr.substr(pos+2,2);
  int chId = atoi(chStr.c_str());
  if(chId >= 0 && chId < 32) return chId;
/*  if( chId ==  0 ) return 7;
  if( chId ==  1 ) return 7;
  if( chId ==  2 ) return 7;
  if( chId ==  3 ) return 7;
  if( chId ==  4 ) return 7;
  if( chId ==  5 ) return 7;
  if( chId ==  6 ) return 5;
  if( chId ==  7 ) return 5;
  if( chId ==  8 ) return 5;
  if( chId ==  9 ) return 5;
  if( chId == 10 ) return 4;
  if( chId == 11 ) return 4;
  if( chId == 12 ) return 4;
  if( chId == 13 ) return 4;
  if( chId == 14 ) return 4;
  if( chId == 15 ) return 4;
  if( chId == 16 ) return 3;
  if( chId == 17 ) return 3;
  if( chId == 18 ) return 3;
  if( chId == 19 ) return 3;
  if( chId == 20 ) return 3;
  if( chId == 21 ) return 3;
  if( chId == 22 ) return 2;
  if( chId == 23 ) return 2;
  if( chId == 24 ) return 2;
  if( chId == 25 ) return 2;
  if( chId == 26 ) return 0;
  if( chId == 27 ) return 0;
  if( chId == 28 ) return 0;
  if( chId == 29 ) return 0;
  if( chId == 30 ) return 0;
  if( chId == 31 ) return 0; */
  return -1;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void  plotterTools::readInputTree ()
{
  //Instantiate the tree branches
  //  inputTree_->Print();

  inputTree_->SetBranchAddress("runNumber"    ,&treeStruct_.runNumber);
  inputTree_->SetBranchAddress("spillNumber"    ,&treeStruct_.spillNumber);
  inputTree_->SetBranchAddress("evtNumber"    ,&treeStruct_.evtNumber);
  inputTree_->SetBranchAddress("evtTimeDist"    ,&treeStruct_.evtTimeDist);
  inputTree_->SetBranchAddress("evtTimeStart"    ,&treeStruct_.evtTimeStart);

  inputTree_->SetBranchAddress("nEvtTimes"    ,&treeStruct_.nEvtTimes);
  inputTree_->SetBranchAddress("evtTime"    ,&treeStruct_.evtTime);
  inputTree_->SetBranchAddress("evtTimeBoard"    ,&treeStruct_.evtTimeBoard);

  inputTree_->SetBranchAddress("nAdcChannels"    ,&treeStruct_.nAdcChannels);
  inputTree_->SetBranchAddress("adcBoard"    ,treeStruct_.adcBoard);
  inputTree_->SetBranchAddress("adcChannel"    ,treeStruct_.adcChannel);
  inputTree_->SetBranchAddress("adcData"    ,treeStruct_.adcData);

  inputTree_->SetBranchAddress("nTdcChannels"    ,&treeStruct_.nTdcChannels);
  inputTree_->SetBranchAddress("tdcBoard"    ,treeStruct_.tdcBoard);
  inputTree_->SetBranchAddress("tdcChannel"    ,treeStruct_.tdcChannel);
  inputTree_->SetBranchAddress("tdcData"    ,treeStruct_.tdcData);

  inputTree_->SetBranchAddress("nDigiSamples"    ,&treeStruct_.nDigiSamples);
  inputTree_->SetBranchAddress("digiGroup"    ,treeStruct_.digiGroup);
  inputTree_->SetBranchAddress("digiChannel"    ,treeStruct_.digiChannel);
  inputTree_->SetBranchAddress("digiSampleIndex",treeStruct_.digiSampleIndex);
  inputTree_->SetBranchAddress("digiSampleValue",treeStruct_.digiSampleValue);
  inputTree_->SetBranchAddress("digiBoard"    ,treeStruct_.digiBoard);

  inputTree_->SetBranchAddress("nScalerWords"    ,&treeStruct_.nScalerWords);
  inputTree_->SetBranchAddress("scalerWord"    ,treeStruct_.scalerWord);
  inputTree_->SetBranchAddress("scalerBoard"    ,treeStruct_.scalerBoard);

  inputTree_->SetBranchAddress("nPatterns"    ,&treeStruct_.nPatterns);
  inputTree_->SetBranchAddress("pattern"    ,treeStruct_.pattern);
  inputTree_->SetBranchAddress("patternBoard"    ,treeStruct_.patternBoard);
  inputTree_->SetBranchAddress("patternChannel"    ,treeStruct_.patternChannel);

  inputTree_->SetBranchAddress("nTriggerWords"    ,&treeStruct_.nTriggerWords);
  inputTree_->SetBranchAddress("triggerWords"    ,treeStruct_.triggerWords);
  inputTree_->SetBranchAddress("triggerWordsBoard",treeStruct_.triggerWordsBoard);


  return ;
} 

void plotterTools::bookPlotsADC(){
  wantADCplots=true;
}

void plotterTools::bookPlotsDigitizer(){
  wantDigiplots=true;
}

void plotterTools::initOutputTree(){
  outputFile_->cd();
  std::cout << "Creating tree in file " << outputFile_->GetName() << std::endl;
  outputTree = new TTree("outputTree","outputTree");
  outputTree->Branch("runNumber",&treeStruct_.runNumber,"runNumber/i");
  outputTree->Branch("spillNumber",&treeStruct_.spillNumber,"spillNumber/i");
  outputTree->Branch("evtNumber",&treeStruct_.evtNumber,"evtNumber/i");
}

void plotterTools::initTreeDQMBranches(){
  for (std::map<TString,varPlot<float>*>::const_iterator iter = varplots.begin ();
       iter != varplots.end () ; ++iter)
    {
      if (iter->second->type==kPlot1D){
	outputTree->Branch(iter->second->name.Data(),&(iter->second->xptr));
      }
      else if (iter->second->type==kPlot2D){
	outputTree->Branch(Form("%s_X",iter->second->name.Data()),&(iter->second->xptr));
	outputTree->Branch(Form("%s_Y",iter->second->name.Data()),&(iter->second->yptr));
      }
    }
}

void plotterTools::initTreeVars(){

  outTreeBranch<float,float> *br;
  br = new outTreeBranch<float,float>("ADCvalues",&varplots);
  if (wantADCplots){
    for (int i=0; i<8; i++) br->addMember(Form("ADC_board_11201_%d",i)); //Caen Lecroy ADC 0
    for (int i=0; i<8; i++) br->addMember(Form("ADC_board_11202_%d",i)); //Caen Lecroy ADC 1
    for (int i=0; i<8; i++) br->addMember(Form("ADC_board_11203_%d",i)); //Caen Lecroy ADC 2
    for (int i=0; i<8; i++) br->addMember(Form("ADC_board_11204_%d",i)); //Caen Lecroy ADC 3
    for (int i=0; i<32; i++) br->addMember(Form("ADC_board_14201_%d",i)); //Caen peak ADC
    //    for (int i=0; i<24; i++) br->addMember(Form("ADC_board_6301_%d",i)); // BGO
    //    for (int i=4; i<8; i++) br->addMember(Form("ADC_board_11301_%d",i)); // BEAM SCINTILLATORS
  }
  else br->addDummy(64);
//  br->addMember("TDCrecoX"); br->addMember("TDCrecoY"); // WIRE CHAMBER
//  for (int i=0; i<64; i++) br->addMember("beamProfileX1",i); // BIG HODOSCOPE
//  for (int i=0; i<64; i++) br->addMember("beamProfileY1",i);  
//  for (int i=0; i<64; i++) br->addMember("beamProfileX2",i);  
//  for (int i=0; i<64; i++) br->addMember("beamProfileY2",i);
  //  for (int i=0; i<8; i++) br->addMember("beamProfileSmallX",i); // SMALL HODOSCOPE
  //  for (int i=0; i<8; i++) br->addMember("beamProfileSmallY",i);
  treevars[br->name]=br;
  
  br = new outTreeBranch<float,float>("digi_max_amplitude",&varplots);
  for (int j=0; j<4; j++){
    if (wantDigiplots) for (int i=0; i<nActiveDigitizerChannels; i++)  br->addMember(Form("digi_ch%02d_max_amplitude",j*8+i));
    else br->addDummy(nActiveDigitizerChannels);}
  treevars[br->name]=br;
  br = new outTreeBranch<float,float>("digi_charge_integrated",&varplots);
  for (int j=0; j<4; j++){
    if (wantDigiplots) for (int i=0; i<nActiveDigitizerChannels; i++)  br->addMember(Form("digi_ch%02d_charge_integrated",j*8+i));
    else br->addDummy(nActiveDigitizerChannels);}
  treevars[br->name]=br;
  br = new outTreeBranch<float,float>("digi_pedestal",&varplots);
  for (int j=0; j<4; j++){
    if (wantDigiplots) for (int i=0; i<nActiveDigitizerChannels; i++)  br->addMember(Form("digi_ch%02d_pedestal",j*8+i));
    else br->addDummy(nActiveDigitizerChannels);}
  treevars[br->name]=br;
  br = new outTreeBranch<float,float>("digi_pedestal_rms",&varplots);
  for (int j=0; j<4; j++){
    if (wantDigiplots) for (int i=0; i<nActiveDigitizerChannels; i++)  br->addMember(Form("digi_ch%02d_pedestal_rms",j*8+i));
    else br->addDummy(nActiveDigitizerChannels);}
  treevars[br->name]=br;
  br = new outTreeBranch<float,float>("digi_time_at_max",&varplots);
  for (int j=0; j<4; j++){
    if (wantDigiplots) for (int i=0; i<nActiveDigitizerChannels; i++)  br->addMember(Form("digi_ch%02d_time_at_max",j*8+i));
    else br->addDummy(nActiveDigitizerChannels);}
  treevars[br->name]=br;
  br = new outTreeBranch<float,float>("digi_time_at_frac30",&varplots);
  for (int j=0; j<4; j++){
    if (wantDigiplots) for (int i=0; i<nActiveDigitizerChannels; i++)  br->addMember(Form("digi_ch%02d_time_at_frac30",j*8+i));
    else br->addDummy(nActiveDigitizerChannels);}
  treevars[br->name]=br;
  br = new outTreeBranch<float,float>("digi_time_at_frac50",&varplots);
  for (int j=0; j<4; j++){
    if (wantDigiplots) for (int i=0; i<nActiveDigitizerChannels; i++)  br->addMember(Form("digi_ch%02d_time_at_frac50",j*8+i));
    else br->addDummy(nActiveDigitizerChannels);}
  treevars[br->name]=br;


//  br = new outTreeBranch<float,float>("BGOvalues",&varplots);
//  if (wantADCplots) for (int i=0; i<24; i++) br->addMember(Form("ADC_board_6301_%d",i)); // BGO
//  else br->addDummy(24);
//  treevars[br->name]=br;
//  br = new outTreeBranch<float,float>("SCINTvalues",&varplots);
//  if (wantADCplots) for (int i=4; i<8; i++) br->addMember(Form("ADC_board_11301_%d",i)); // BEAM SCINTILLATORS
//  else br->addDummy(4);
//  treevars[br->name]=br;
  br = new outTreeBranch<float,float>("TDCreco",&varplots);
  br->addMember("TDCrecoX"); br->addMember("TDCrecoY"); // WIRE CHAMBER  
  treevars[br->name]=br;

/*
//THIS IS A TEST
  br = new outTreeBranch<float,float>("HODO1reco",&varplots);
  br->addMember("beamPositionTEST"); // HODO1
  treevars[br->name]=br;
//END
*/

  br = new outTreeBranch<float,float>("HODO1reco",&varplots);
  br->addMember("beamPositionX1"); br->addMember("beamPositionY1"); // HODO1
  treevars[br->name]=br;

  br = new outTreeBranch<float,float>("HODO2reco",&varplots);
  br->addMember("beamPositionX2"); br->addMember("beamPositionY2"); // HODO2
  treevars[br->name]=br;

  br = new outTreeBranch<float,float>("nFibersOn1",&varplots);
  br->addMember("nFibersOnX1");  br->addMember("nFibersOnY1");
  treevars[br->name]=br;
  br = new outTreeBranch<float,float>("nFibersOn2",&varplots);
  br->addMember("nFibersOnX2");  br->addMember("nFibersOnY2");
  treevars[br->name]=br;


  outTreeBranch<bool,float> *br2 = NULL;
  br2 = new outTreeBranch<bool,float>("HODOX1",&varplots);
  for (int i=0; i<64; i++) br2->addMember("beamProfileX1",i);
  treevars2[br2->name]=br2;
  br2 = new outTreeBranch<bool,float>("HODOY1",&varplots);
  for (int i=0; i<64; i++) br2->addMember("beamProfileY1",i);
  treevars2[br2->name]=br2;
  br2 = new outTreeBranch<bool,float>("HODOX2",&varplots);
  for (int i=0; i<64; i++) br2->addMember("beamProfileX2",i);
  treevars2[br2->name]=br2;
  br2 = new outTreeBranch<bool,float>("HODOY2",&varplots);
  for (int i=0; i<64; i++) br2->addMember("beamProfileY2",i);
  treevars2[br2->name]=br2;




  for (std::map<TString,outTreeBranch<float,float>*>::const_iterator it = treevars.begin(); it!= treevars.end(); it++){
    outputTree->Branch(it->first.Data(),&(it->second->dataptr));
  }
  for (std::map<TString,outTreeBranch<bool,float>*>::const_iterator it = treevars2.begin(); it!= treevars2.end(); it++){
    outputTree->Branch(it->first.Data(),&(it->second->dataptr));
  }
}




void plotterTools::fillTreeVars(){
  for (std::map<TString,outTreeBranch<float,float>*>::const_iterator it = treevars.begin(); it!= treevars.end(); it++){
    it->second->Fill();
  }
  for (std::map<TString,outTreeBranch<bool,float>*>::const_iterator it = treevars2.begin(); it!= treevars2.end(); it++){
    it->second->Fill();
  }
}

void  plotterTools::Loop()
{

//THIS IS A TEST TO SEE WHAT HAPPENS
//cout << "SUCCESSFUL TEST" << endl;

 ifstream indata;
 char read;
 int num;
 bool b1 = 1;
 int numelems = 1;
 int tracker = 0;
 int count = 0;

  FILE*datapts;
  datapts = fopen("IC.txt","r");
  if(!datapts){ // file couldn't be opened
  std::cerr << "Error: the calibration file " << "IC.txt" << " could not be opened" << std::endl;
}
  read = getc(datapts);

while ( read != EOF ) { // keep reading until end-of-file
	if(read == '\t' && b1 == 1) numelems++;
	if(read == '\n') b1 = 0;
	read = getc(datapts);
}
  fclose(datapts);
  cout << numelems << endl;

  float channel_peak[numelems];
  float channel_x[numelems];
  float channel_y[numelems];
  float max_amp[numelems];
  float Int_C[numelems];
  int value = 0;

 indata.open("IC.txt"); // opens the file
 while ( indata>>num ) { // keep reading until end-of-file
//  cout << "IC number read is " << num << endl;
 if(tracker == 0) {channel_peak[count] = num; count++;}
 if(tracker == 1) {channel_x[count] = num; count++;}
 if(tracker == 2) {channel_y[count] = num; count++;}
 if(count == numelems) {tracker++; count = 0;}

}  indata.close();

//for(int i = 0; i < numelems; i++) cout << channel_peak[i] << endl;
//for(int i = 0; i < numelems; i++) cout << channel_x[i] << endl;
//for(int i = 0; i < numelems; i++) cout << channel_y[i] << endl;

	 //END TEST IT WORKED

  uint nentries = getTreeEntries();
  int nBinsHistory=nentries/getStepHistoryPlots();

  //  nentries=1;

  //loop and fill histos
  for (unsigned iEntry = 0 ; iEntry < nentries ; ++iEntry) 
    {
      inputTree_->GetEntry(iEntry);

      WORD triggerWord=treeStruct_.triggerWords[0];
      if ((~triggerWord & beamTriggerBitMask) && triggerType_!="beam"){//not of the triggerword is just a bit on, since beamWord=FE00, pedWord=FD00
	continue;
      }else if ((~triggerWord & pedTriggerBitMask)  && triggerType_!="ped") {
	continue;
      }else if ((~triggerWord & ledTriggerBitMask)  && triggerType_!="led") {
	continue;
      }


      if (iEntry%1000==0)
	std::cout<<"iEntry: "<<iEntry<<"/"<<nentries<<endl;
      

      if(iEntry==0){
	for(uint i =0;i<treeStruct_.nEvtTimes;++i)
	  timeStart_[i]=treeStruct_.evtTime[i];
      }
      if(iEntry==0 && wantADCplots) initAdcChannelNames(nBinsHistory);
      if(iEntry==0 && wantDigiplots) initDigiPlots();
      if(iEntry==0) {
	initOutputTree();
	//	initTreeDQMBranches();
	initTreeVars();
      }
      if(iEntry==(nentries -1)){
	for(uint i =0;i<treeStruct_.nEvtTimes;++i)
	  timeEnd_[i]=treeStruct_.evtTime[i];
      }



      fillObjects();

      for (std::map<TString,varPlot<float>*>::const_iterator iter = varplots.begin ();
           iter != varplots.end () ; ++iter)
        {

	  iter->second->ClearVectors();

	  if (iter->second->type!=kPlotGraph) computeVariable(iter->first);
	  else if (iEntry%historyStep_==0 && iEntry!=0 && (int)iEntry/historyStep_-1 < nBinsHistory){
	    iHistEntry = (int)iEntry/historyStep_-1;
	    iThisEntry = iEntry;
	    computeVariable(iter->first);
	  }
	}



      if (wantDigiplots){

	for (std::map<TString,varPlot<float>*>::iterator it=varplots.begin();it!=varplots.end();++it)
	  if (it->second->waveform) it->second->waveform->clear();
	
	for (uint iSample = 0 ; iSample < treeStruct_.nDigiSamples ; ++iSample)
	  {

	    if (treeStruct_.digiChannel[iSample]>=nActiveDigitizerChannels) continue;

	    TString thisname = getDigiChannelName(treeStruct_.digiGroup[iSample],treeStruct_.digiChannel[iSample]);
	    TString thisval = Form("%02d",treeStruct_.digiChannel[iSample]);
	    //cout << "My attempt at reading the thing... " << thisval.Data() << endl; //This works, but now I need to find out how to make sure I'm not falling for the super repeat. 

//	    int nametest = treeStruct_.digiChannel[iSample]; //This also works, but again, I gotta make sure I'm not falling for the super repeat. 
//	    cout << "nametest over here " << nametest << endl;

	    thisname=Form("%s_%s",thisname.Data(),"pulse");
	    varplots[thisname]->Fill2D(treeStruct_.digiSampleIndex[iSample], treeStruct_.digiSampleValue[iSample],1.);
	    varplots[thisname]->waveform->addTimeAndSample(treeStruct_.digiSampleIndex[iSample]*timeSampleUnit(treeStruct_.digiFrequency[iSample]),treeStruct_.digiSampleValue[iSample]);
	  }

	float sum_amplitudes = 0;

	//Add reconstruction of waveforms
	for (std::map<TString,varPlot<float>*>::iterator it=varplots.begin();it!=varplots.end();++it)
	  {

	    if (!(it->second->waveform)) continue;
	    
	    Waveform::baseline_informations wave_pedestal;
	    Waveform::max_amplitude_informations wave_max;
	    
	    wave_pedestal=it->second->waveform->baseline(5,44); //use 40 samples between 5-44 to get pedestal and RMS
	    it->second->waveform->offset(wave_pedestal.pedestal);

	    it->second->waveform->rescale(-1); 
	    wave_max=it->second->waveform->max_amplitude(50,900,5); //find max amplitude between 50 and 900 samples
	    if (wave_max.max_amplitude<20)
	      {
		//try not inverting if signal is positive
		it->second->waveform->rescale(-1); 
		Waveform::max_amplitude_informations wave_max_inv=it->second->waveform->max_amplitude(50,900,5); //find max amplitude between 50 and 900 samples
		if (wave_max_inv.max_amplitude>wave_max.max_amplitude)
		    wave_max=wave_max_inv;
		else //stay with negative signals
		  it->second->waveform->rescale(-1); 
	      }

	    TString thisname = it->second->name.ReplaceAll("_pulse","");
	    varplots[Form("%s_pedestal",thisname.Data())]->Fill(wave_pedestal.pedestal,1.);
	    varplots[Form("%s_pedestal_rms",thisname.Data())]->Fill(wave_pedestal.rms,1.);
	    varplots[Form("%s_max_amplitude",thisname.Data())]->Fill(wave_max.max_amplitude,1.);
	    varplots[Form("%s_charge_integrated",thisname.Data())]->Fill(it->second->waveform->charge_integrated(0,900),1.); // pedestal already subtracted
	    varplots[Form("%s_time_at_max",thisname.Data())]->Fill(wave_max.time_at_max*1.e9,1.);
	    varplots[Form("%s_time_at_frac30",thisname.Data())]->Fill(it->second->waveform->time_at_frac(wave_max.time_at_max-1.3e-8,wave_max.time_at_max,0.3,wave_max,7)*1.e9,1.);
	    varplots[Form("%s_time_at_frac50",thisname.Data())]->Fill(it->second->waveform->time_at_frac(wave_max.time_at_max-1.3e-8,wave_max.time_at_max,0.5,wave_max,7)*1.e9,1.);


	    int x1 = -1;
	    for(int i=0;i<64;i++){
	      if(fibersOn_[hodoX1][i]==1 && x1==-1) x1 = i;
	      if(fibersOn_[hodoX1][i]==1 && x1!=-1) { x1 = -1; break; }
	    }
	    int y1 = -1;
	    for(int i=0;i<64;i++){
	      if(fibersOn_[hodoY1][i]==1 && y1==-1) y1 = i;
	      if(fibersOn_[hodoY1][i]==1 && y1!=-1) { y1 = -1; break; }
	    }
	    int x2 = -1;
	    for(int i=0;i<64;i++){
	      if(fibersOn_[hodoX2][i]==1 && x2==-1) x2 = i;
	      if(fibersOn_[hodoX2][i]==1 && x2!=-1) { x2 = -1; break; }
	    }
	    int y2 = -1;
	    for(int i=0;i<64;i++){
	      if(fibersOn_[hodoY2][i]==1 && y2==-1) y2 = i;
	      if(fibersOn_[hodoY2][i]==1 && y2!=-1) { y2 = -1; break; }
	    }
	    
	    varplots[Form("%s_charge_integrated_vs_hodoX1",thisname.Data())]->Fill(x1,it->second->waveform->charge_integrated(0,900)); // pedestal already subtracted
	    varplots[Form("%s_charge_integrated_vs_hodoY1",thisname.Data())]->Fill(y1,it->second->waveform->charge_integrated(0,900)); // pedestal already subtracted
	    varplots[Form("%s_charge_integrated_vs_hodoX2",thisname.Data())]->Fill(x2,it->second->waveform->charge_integrated(0,900)); // pedestal already subtracted
	    varplots[Form("%s_charge_integrated_vs_hodoY2",thisname.Data())]->Fill(y2,it->second->waveform->charge_integrated(0,900)); // pedestal already subtracted

	    varplots[Form("%s_charge_integrated_vs_TDCrecoX",thisname.Data())]->Fill(tdc_recox,it->second->waveform->charge_integrated(0,900)); // pedestal already subtracted
	    varplots[Form("%s_charge_integrated_vs_TDCrecoY",thisname.Data())]->Fill(tdc_recoy,it->second->waveform->charge_integrated(0,900)); // pedestal already subtracted
//            varplots[Form("%s_charge_integrated_vs_TDCrecoY",thisname.Data())]->Fill(tdc_recoy,wave_max.max_amplitude); // testing a recoY versus maximum plot. 	   
 
	    int x = getDigiChannelX(it->second->name); //This gives the channel!
	    //cout << "x channel value versus the plot name " << x << "and then " << thisname.Data() << endl; //These two things should match up
	    max_amp[x] = wave_max.max_amplitude;
	    Int_C[x] = it->second->waveform->charge_integrated(0,900);
	    //max_amp[x] = it->second->waveform->charge_integrated(0,900);
	    int y = getDigiChannelY(it->second->name);
	    varplots["allCh_charge_integrated_map"]->Fill2D(x,y,it->second->waveform->charge_integrated(0,900)); // pedestal already subtracted
	    varplots["allCh_max_amplitude_map"]->Fill2D(x,y,wave_max.max_amplitude); // pedestal already subtracted
	    varplots["allCh_pedestal_map"]->Fill2D(x,y,wave_pedestal.pedestal); // pedestal already subtracted
	    varplots["allCh_pedestal_rms_map"]->Fill2D(x,y,wave_pedestal.rms); // pedestal already subtracted
	    sum_amplitudes+=wave_max.max_amplitude;
	  } // End of iterator loop
	
//MORE TESTING
    float energy_sum = 0;
    float position_weight_sum = 0;
    float position_weight[numelems];
    numelems = 5;
	/*for (int i = 0;i < numelems;i++)
	{
	cout << "channel peak " << channel_peak[i] << endl;
	cout << "channel_x " << channel_x[i] << endl;
	cout << "channel_y " << channel_y[i] << endl;
	}*/ //This check is successful. The peak values are reasonable. 

//    for (int i = 0;i < numelems;i++) cout << "max_amp of " << i << " is " << max_amp[i] << endl;
	//This bit works as well, but, of course, the channels 8-25 are blank. For now I'm going to set numelems = 8.
    for (int i = 0;i < numelems;i++) energy_sum += max_amp[i]/channel_peak[i];
//    cout << "Energy sum = " << energy_sum << endl; //Reasonable I think
    for(int channel = 0;channel < numelems;channel++){
      position_weight[channel] = 3.8 + log(max_amp[channel]/(channel_peak[channel]*energy_sum));
      if (position_weight[channel] < 0) position_weight[channel] = 0;
      position_weight_sum += position_weight[channel];
//      cout << "position weight of " << channel << " is " << position_weight[channel] << endl;
    }
//      cout << "Position weight sum = " << position_weight_sum << endl;

    if(position_weight_sum != 0){
    EA_X = 0;
    for(int channel = 0;channel < numelems;channel++) EA_X += 22.0 * channel_x[channel] * position_weight[channel]/position_weight_sum;
//    cout << "EA_X value calculated is " << EA_X << endl; //As of right now, values of mostly 0 are being calculated, but maybe that's expected seeing how we're on C3 o-o

    EA_Y = 0;
    for(int channel = 0;channel < numelems;channel++) EA_Y += 22.0 * channel_y[channel] * position_weight[channel]/position_weight_sum;
//    cout << "EA_Y value calculated is " << EA_Y << endl;
    }
//      cout << "EA_Y value calculated is " << EA_Y << endl;
//      cout << "tdc_recoy found is " << tdc_recoy << endl;
    varplots["MAXIMUMPLOTTEST"]->Fill(EA_Y, max_amp[2], 1.);

//END TESTING*/


//	  cout << "Place of the output " << endl;
	  //This will be the location of the EA_X and EA_Y calculations. Fill the max_amp[channel value] = maximum waveform
	  //And then with those values, you should be able to calculate things. 
      }

      fillTreeVars();
      outputTree->Fill();

    } // loop over the events

  //  fillMatrixView();

  std::cout << outputTree->GetEntries() << std::endl;

}

void plotterTools::bookPlotsScaler(int nBinsHistory){
  //in this function you define all the objects for the scaler
  addPlot(0,"nEvts", nBinsHistory, "history", group_,module_);//simple TGraph
  addPlot(1,"triggerEff",nBinsHistory, "history", group_,module_);//TGraph with more complex variable
  addPlot(0,"nTrigSPS", 100,0,3000, "1D",group_,module_);//simple TH1F
  addPlot(0,"nTrigSPSVsnTrig", 100,0,3000, 100,0,3000,"nTrigSPS","nTrig","2D",group_,module_);//simple TH2F
  addPlot(0,"nTrigSPSVsnTrig3D", 100,0,3000, "1D",group_,module_,3);// TH1F with more than one variable to fill per event
}

void plotterTools::bookPlotsHodo(int nBinsHistory){

  addPlot(0,"beamProfileX1", 64,-0.5, 63.5,"1D",group_,module_,64);//simple TH1F
  addPlot(0,"beamProfileY1", 64,-0.5, 63.5,"1D",group_,module_,64);//simple TH1F
  addPlot(0,"beamProfileX2", 64,-0.5, 63.5,"1D",group_,module_,64);//simple TH1F
  addPlot(0,"beamProfileY2", 64,-0.5, 63.5,"1D",group_,module_,64);//simple TH1F
  addPlot(1,"beamProfileDrawX1", 64,-0.5, 63.5,"1D",group_,module_,64);//simple TH1F
  addPlot(1,"beamProfileDrawY1", 64,-0.5, 63.5,"1D",group_,module_,64);//simple TH1F
  addPlot(1,"beamProfileDrawX2", 64,-0.5, 63.5,"1D",group_,module_,64);//simple TH1F
  addPlot(1,"beamProfileDrawY2", 64,-0.5, 63.5,"1D",group_,module_,64);//simple TH1F

  addPlot(1,"nFibersOnX1", 64,-0.5, 63.5,"1D",group_,module_);//simple TH1F
  addPlot(1,"nFibersOnY1", 64,-0.5, 63.5,"1D",group_,module_);//simple TH1F
  addPlot(1,"nFibersOnX2", 64,-0.5, 63.5,"1D",group_,module_);//simple TH1F
  addPlot(1,"nFibersOnY2", 64,-0.5, 63.5,"1D",group_,module_);//simple TH1F

  addPlot(1,"beamPositionX1", 64,-0.5, 63.5,"1D",group_,module_);//simple TH1F
//  addPlot(1,"beamPositionTEST", 64,-0.5, 63.5,"1D",group_,module_);//THIS IS A TEST
  addPlot(1,"WCvsHodo",100,-50,50,100,-50,50,"X","Y","2D",group_,module_); //THIS IS A TEST2
//  addPlot(1,"MAXIMUMPLOTTEST",100,-50,50,100,-50,50,"X","Y","2Dprof",group_,module_); //THIS IS A TEST2
  addPlot(1,"beamPositionX2", 64,-0.5, 63.5,"1D",group_,module_);//simple TH1F
  addPlot(1,"beamPositionY1", 64,-0.5, 63.5,"1D",group_,module_);//simple TH1F
  addPlot(1,"beamPositionY2", 64,-0.5, 63.5,"1D",group_,module_);//simple TH1F

  addPlot(0,"beamPositionX", 64,-0.5, 63.5,"1D",group_,module_);//simple TH1F
  addPlot(0,"beamPositionY", 64,-0.5, 63.5,"1D",group_,module_);//simple TH1F

}

void plotterTools::bookCombinedPlotsHodo(){
//
//  addPlotCombined(0,"hodoCorrelationX","beamProfileX1","beamProfileX2","2D",group_,module_);//correlation plots it uses TH1F done before to build this TH2
//  addPlotCombined(0,"hodoCorrelationY","beamProfileY1","beamProfileY2","2D",group_,module_);//correlation plots it uses TH1F done before to build this TH2
//
}

void plotterTools::bookPlotsSmallHodo(int nBinsHistory){

  addPlot(0,"beamProfileSmallX", 8,-0.5, 7.5,"1D",group_,module_,8);//simple TH1F
  addPlot(0,"beamProfileSmallY", 8,-0.5, 7.5,"1D",group_,module_,8);//simple TH1F
  addPlot(1,"beamProfileDrawSmallX", 8,-0.5, 7.5,"1D",group_,module_,8);//simple TH1F
  addPlot(1,"beamProfileDrawSmallY", 8,-0.5, 7.5,"1D",group_,module_,8);//simple TH1F

  addPlot(1,"nFibersOnSmallX", 8,-0.5, 7.5,"1D",group_,module_);//simple TH1F
  addPlot(1,"nFibersOnSmallY", 8,-0.5, 7.5,"1D",group_,module_);//simple TH1F

  addPlot(0,"beamPositionSmallX", 8,-0.5, 7.5,"1D",group_,module_);//simple TH1F
  addPlot(0,"beamPositionSmallY", 8,-0.5, 7.5,"1D",group_,module_);//simple TH1F


}


void plotterTools::bookPlotsDAQStatus(int nBinsHistory){
  addPlot(1,"fractionTakenTrig",nBinsHistory, "history", group_,module_);//TGraph with more complex variable
  addPlot(0,"fractionTakenTrigHisto",100,0,1,"1D",group_,module_);//simple TH1F
  addPlot(0,"nTotalEvts", 1,-0.5, 1.5,"1D",group_,module_);//simple TH1F
  addPlot(0,"deltaTime10", 100,-60.5, 59.5,"1D",group_,module_);//simple TH1F          
  addPlot(0,"deltaTime20", 100,-60.5, 59.5,"1D",group_,module_);//simple TH1F          
  addPlot(0,"deltaTime21", 100,-60.5, 59.5,"1D",group_,module_);//simple TH1F          
  addPlot(0,"MULTILINE_time0",500000,0,5000000,"1D",group_,module_);
  addPlot(0,"MULTILINE_time1",500000,0,5000000,"1D",group_,module_);
  addPlot(0,"MULTILINE_time2",500000,0,5000000,"1D",group_,module_);
 }

void plotterTools::bookPlotsTDC(int nBinsHistory){
  addPlot(0,"TDCinputTime1",100,0,50000,"1D",group_,module_,MaxTdcReadings);
  addPlot(0,"TDCinputTime2",100,0,50000,"1D",group_,module_,MaxTdcReadings);
  addPlot(0,"TDCinputTime3",100,0,50000,"1D",group_,module_,MaxTdcReadings);
  addPlot(0,"TDCinputTime4",100,0,50000,"1D",group_,module_,MaxTdcReadings);
  addPlot(1,"TDCrecoX",100,-50,50,"1D",group_,module_);
  addPlot(1,"TDCrecoY",100,-50,50,"1D",group_,module_);
  addPlot(1,"nTdcChannels",8,-0.5,8.5,"1D",group_,module_);
  addPlot(0,"TDChistoryRecoX",nBinsHistory,"history",group_,module_);
  addPlot(0,"TDChistoryRecoY",nBinsHistory,"history",group_,module_);
  addPlot(1,"TDCrecoPos",100,-50,50,100,-50,50,"X","Y","2D",group_,module_);
}

void plotterTools::bookCombinedPlots(){
////  addPlotCombined(0,"nTrigSPSVsnTrig3DvsnEvts","nTrigSPS","nTrigSPSVsnTrig3D","2D",group_,module_);//correlation plots it uses TH1F done before to build this TH2
////  addPlotCombined(0,"HodoWireCorrelationX","beamProfileX","TDCrecoX","2D",group_,module_); // TO BE ENABLED IF RUNNING ALL REQUIRED 1D PLOTTERS
////  addPlotCombined(0,"HodoWireCorrelationY","beamProfileY","TDCrecoY","2D",group_,module_);
//
}

void plotterTools::fitHisto(TString name,TString func){

  map<TString, varPlot<float>*>::iterator it;
  it=varplots.find(name);
  if(it!=varplots.end()) ((TH1F*) it->second->GetPlot())->Fit(func);

}

void plotterTools::addPlotCombined(bool doPlot, TString name, TString name1, TString name2,TString type, TString group , TString module){

  varPlot<float> *var = new varPlot<float>(&iThisEntry,&iHistEntry,kPlot2D);
  var->SetName(name);
  var->SetPlot((TObject*)  bookHistoCombined(name,name1,name2));
  var->doPlot = doPlot;
  var->SetGM(group,module);
  varplots[name]=var;
  
  ((TH2F*)var->GetPlot())->SetTitle(name);
  ((TH2F*)var->GetPlot())->SetName(name);
  ((TH2F*)var->GetPlot())->SetXTitle(((TH1F* )varplots[name1]->GetPlot())->GetTitle());
  ((TH2F*)var->GetPlot())->SetYTitle(((TH1F* )varplots[name2]->GetPlot())->GetTitle());

}

void plotterTools::setPlotAxisRange(TString name, TString axis,float min, float max){
  if (varplots.find(name)==varplots.end()) {
    std::cout << "NOT FOUND " << name.Data() << std::endl;
    for (std::map<TString,varPlot<float>*>::const_iterator it = varplots.begin(); it!=varplots.end(); it++) std::cout << it->second->name.Data() << std::endl;
    return;
  }
  if (varplots[name]->type==kPlot1D) ((TH1F*)(varplots[name]->GetPlot()))->SetAxisRange(min,max,axis);
  else if (varplots[name]->type==kPlotGraph) {
    if (axis=="X") ((TGraph*)(varplots[name]->GetPlot()))->GetXaxis()->SetRangeUser(min,max);
    else if (axis=="Y") ((TGraph*)(varplots[name]->GetPlot()))->GetYaxis()->SetRangeUser(min,max); 
  }
  else if (varplots[name]->type==kPlot2D) ((TH2F*)(varplots[name]->GetPlot()))->SetAxisRange(min,max,axis);
}

//for TGraph
TGraph *
plotterTools::addPlot(bool doPlot, TString name,int nPoints,TString type, TString group, TString module, bool vetoFill)
{
    if (vetoFill) vetoFillObjects[name]=true;
    else vetoFillObjects[name]=false;

    varPlot<float> *var = new varPlot<float>(&iThisEntry,&iHistEntry,kPlotGraph,false,nPoints);
    var->SetName(name);
    var->SetPlot((TObject*)  bookGraph(name,nPoints,type, group_,module_));
    var->doPlot = doPlot;
    var->SetGM(group,module);
    varplots[name]=var;
    
    return dynamic_cast<TGraph *> (var->GetPlot()) ;
}

//for TH1F
TH1F * plotterTools::addPlot(bool doPlot, TString name,int nBinsX, float xMin, float xMax, TString type, TString group, TString module, int varDim, bool vetoFill)
{
   if (vetoFill) vetoFillObjects[name]=true;
   else vetoFillObjects[name]=false;
   
   varPlot<float> *var = new varPlot<float>(&iThisEntry,&iHistEntry,kPlot1D,false,varDim);
   var->SetName(name);
   var->SetPlot((TObject*) bookHisto(name, nBinsX, xMin, xMax, type, group_,module_));
   var->doPlot = doPlot;
   var->SetGM(group,module);
   varplots[name]=var;
   
   return dynamic_cast<TH1F *> (var->GetPlot());
}

//for TProfile
TProfile * plotterTools::addPlot(bool doPlot, TString name,int nBinsX, float xMin, float xMax, float yMin, float yMax, TString type, TString group, TString module, int varDim, bool vetoFill)
{
   if (vetoFill) vetoFillObjects[name]=true;
   else vetoFillObjects[name]=false;
   
   varPlot<float> *var = new varPlot<float>(&iThisEntry,&iHistEntry,kPlot1DProf,false,varDim);
   var->SetName(name);
   var->SetPlot((TObject*) bookHistoProf(name, nBinsX, xMin, xMax, yMin, yMax, type, group_,module_));
   var->doPlot = doPlot;
   var->SetGM(group,module);
   varplots[name]=var;
   
   return dynamic_cast<TProfile *> (var->GetPlot());
}

//for TH2F
TH2F * plotterTools::addPlot(bool doPlot, TString name,int nBinsX, float xMin, float xMax, int nBinsY, float yMin, float yMax, 
                             TString xTitle, TString yTitle,TString type, TString group, TString module, bool addProfile, bool vetoFill)
{
   if (vetoFill) vetoFillObjects[name]=true;
   else vetoFillObjects[name]=false;

   varPlot<float> *var = new varPlot<float>(&iThisEntry,&iHistEntry,kPlot2D,addProfile);
   var->SetName(name);
   var->SetPlot((TObject*) bookHisto2D(name, nBinsX, xMin, xMax, nBinsY, yMin, yMax, xTitle, yTitle, type, group_,module_));
   var->doPlot = doPlot;
   var->SetGM(group,module);
   varplots[name]=var;
   
   return dynamic_cast<TH2F *> (var->GetPlot());
}


//for TProfile2D
TProfile2D * plotterTools::addPlot(bool doPlot, TString name,int nBinsX, float xMin, float xMax, int nBinsY, float yMin, float yMax, float zMin, float zMax,
				   TString xTitle, TString yTitle,TString type, TString group, TString module, bool addProfile, bool vetoFill)
{
   if (vetoFill) vetoFillObjects[name]=true;
   else vetoFillObjects[name]=false;

   varPlot<float> *var = new varPlot<float>(&iThisEntry,&iHistEntry,kPlot2DProf,addProfile);
   var->SetName(name);
   var->SetPlot((TObject*) bookHisto2DProf(name,nBinsX, xMin, xMax, nBinsY, yMin, yMax, zMin, zMax, xTitle,yTitle, type, group_,module_));
   var->doPlot = doPlot;
   var->SetGM(group,module);
   varplots[name]=var;
   
   return dynamic_cast<TProfile2D *> (var->GetPlot());
}

//TH1F
TH1F* plotterTools::bookHisto(TString name,int nBinsX,float xMin, float xMax, TString type, TString group, TString module){
  TH1F* histo = new TH1F (name, name , nBinsX, xMin, xMax);
  return histo;
}

//TProfile
TProfile* plotterTools::bookHistoProf(TString name,int nBinsX,float xMin, float xMax, float yMin, float yMax, TString type, TString group, TString module){
  TProfile* histo = new TProfile (name, name , nBinsX, xMin, xMax);
  if(yMin!=-999999 && yMax!=999999)
    {
      histo -> SetMinimum(yMin);
      histo -> SetMaximum(yMax);
    }
  return histo;
}

//TH2F
TH2F* plotterTools::bookHisto2D(TString name,int nBinsX,float xMin, float xMax,int nBinsY, float yMin, float yMax,TString xTitle, TString yTitle, TString type, TString group, TString module){
  TH2F* histo = new TH2F (name, name, nBinsX, xMin, xMax, nBinsY, yMin, yMax);
  histo->SetXTitle(xTitle);
  histo->SetYTitle(yTitle);
  return histo;
}

//TProfile2D
TProfile2D* plotterTools::bookHisto2DProf(TString name,int nBinsX,float xMin, float xMax,int nBinsY, float yMin, float yMax, float zMin, float zMax, TString xTitle, TString yTitle, TString type, TString group, TString module){
  TProfile2D* histo = new TProfile2D (name, name, nBinsX, xMin, xMax, nBinsY, yMin, yMax);
  if(zMin!=-999999 && zMax!=999999)
    {
      histo -> SetMinimum(zMin);
      histo -> SetMaximum(zMax);
    }
  histo->SetXTitle(xTitle);
  histo->SetYTitle(yTitle);
  return histo;
}

//combined plots
TH2F* plotterTools::bookHistoCombined(TString name,TString name1, TString name2){

  int  nBinsX=((TH1F* )varplots[name1]->GetPlot())->GetNbinsX();
  float xMin=((TH1F* )varplots[name1]->GetPlot())->GetXaxis()->GetBinLowEdge(1);
  float xMax=((TH1F* )varplots[name1]->GetPlot())->GetXaxis()->GetBinLowEdge(nBinsX)+((TH1F* )varplots[name1]->GetPlot())->GetXaxis()->GetBinWidth(nBinsX);
                                        
  int nBinsY=((TH1F* )varplots[name2]->GetPlot())->GetNbinsX();
  float yMin=((TH1F* )varplots[name2]->GetPlot())->GetXaxis()->GetBinLowEdge(1);
  float yMax=((TH1F* )varplots[name2]->GetPlot())->GetXaxis()->GetBinLowEdge(nBinsY)+((TH1F* )varplots[name2]->GetPlot())->GetXaxis()->GetBinWidth(nBinsY);

  TH2F* histo = new TH2F (name, name ,nBinsX, xMin,xMax, nBinsY, yMin, yMax);
  for(int i =1;i<=nBinsX;i++){
    for(int j =1;j<=nBinsY;j++){
      float X=((TH1F* )varplots[name1]->GetPlot())->GetBinContent(i);
      float Y=((TH1F* )varplots[name2]->GetPlot())->GetBinContent(j);
      histo->SetBinContent(i,j,X*Y);
    }
  }

  return histo;

}

//TGraph
TGraph* plotterTools::bookGraph(TString name,int nPoints,TString type, TString group, TString module){

  TGraph* graph=new TGraph (nPoints);
  

  graph->SetTitle(name);
  graph->GetXaxis()->SetTitle("Event");

  graph->SetName(group+TString("_")+module+TString("_")+type+TString("_")+TString(graph->GetTitle()));

  return graph;

}

void plotterTools::setModule(TString module){
  module_=module;
}

void plotterTools::setGroup(TString group){
  group_=group;
}

void plotterTools::setStepHistoryPlots(int n){
  historyStep_=n;
}

int plotterTools::getTreeEntries(){
  return  inputTree_->GetEntries();
}

int plotterTools::getStepHistoryPlots(){
  return  historyStep_;
}

void plotterTools::fillMatrixView(){

  int matr[5][5];
  matr[0][4]=9;
  matr[1][4]=10;
  matr[2][4]=11;
  matr[3][4]=12;
  matr[4][4]=13;
  matr[0][3]=14;
  matr[1][3]=1;
  matr[2][3]=2;
  matr[3][3]=3;
  matr[4][3]=15;
  matr[0][2]=16;
  matr[1][2]=4;
  matr[2][2]=-1; // CeF3
  matr[3][2]=5;
  matr[4][2]=17;
  matr[0][1]=18;
  matr[1][1]=6;
  matr[2][1]=7;
  matr[3][1]=8;
  matr[4][1]=19;
  matr[0][0]=20;
  matr[1][0]=21;
  matr[2][0]=22;
  matr[3][0]=23;
  matr[4][0]=24;


  // from pedestal run 397
  float ped[24];
  ped[0]=108.37;
  ped[1]=65.01;
  ped[2]=144.58;
  ped[3]=63.42;
  ped[4]=44.89;
  ped[5]=66.46;
  ped[6]=46.56;
  ped[7]=50.91;
  ped[8]=156.00;
  ped[9]=72.05;
  ped[10]=52.00;
  ped[11]=58.21;
  ped[12]=43.09;
  ped[13]=54.01;
  ped[14]=38.01;
  ped[15]=61.97;
  ped[16]=59.68;
  ped[17]=224.20;
  ped[18]=70.16;
  ped[19]=56.14;
  ped[20]=40.94;
  ped[21]=73.53;
  ped[22]=66.24;
  ped[23]=90.68;

  TH2F *h = (TH2F*)(varplots["MatrixView"]->GetPlot());
  for (int i=0; i<5; i++){
    for (int j=0; j<5; j++){
      if (i!=2 || j!=2) {
	TString name = Form("ADC_board_6301_%d",matr[i][j]-1);
	h->SetBinContent(i+1,j+1,((TH1F*)(varplots[name]->GetPlot()))->GetMean()-ped[matr[i][j]-1]);
	h->SetBinError(i+1,j+1,((TH1F*)(varplots[name]->GetPlot()))->GetRMS());
      }
      else {
	h->SetBinContent(i+1,j+1,0);
	h->SetBinError(i+1,j+1,0);
      }
    }
  }

  int matrcef3[2][2];
  // 1 2
  // 4 3  TO BE CHECKED!!!
  matrcef3[0][1]=1;
  matrcef3[1][1]=2;
  matrcef3[0][0]=4;
  matrcef3[1][0]=3;

  h= (TH2F*)(varplots["MatrixViewCeF3"]->GetPlot());
  for (int i=0; i<2; i++){
    for (int j=0; j<2; j++){
      TString name = Form("digi_gr0_ch%d_max_amplitude",matrcef3[i][j]-1);
      h->SetBinContent(i+1,j+1,((TH1F*)(varplots[name]->GetPlot()))->GetMean());
      h->SetBinError(i+1,j+1,((TH1F*)(varplots[name]->GetPlot()))->GetRMS());
    }
  }
  

}

void plotterTools::saveHistos(){
  if(VERBOSE){  std::cout << "==================== Saving histograms =======================" << std::endl;
    std::cout << "outputFile " << outputFile_->GetName() << " opened" << std::endl;}
  outputFile_->cd();
  for (std::map<TString,varPlot<float>*>::const_iterator out=varplots.begin();out!=varplots.end();++out)
    out->second->GetPlot()->Write(out->second->name);
  outputTree->Write();
  outputFile_->Close();
  if(VERBOSE){  std::cout << "outputFile " << outputFile_->GetName() << " closed" << std::endl;
  std::cout << "==================== DQM analysis is done =======================" << std::endl;
  }
}


pair<int, string> plotterTools::execute (const string & command) 
{
    FILE *in;
    char buff[512] ;
    if (!(in = popen (command.c_str (), "r")))
      {
        return pair<int, string> (-99, "") ;
      }

    std::string result, tempo ;
    while (fgets (buff, sizeof (buff), in)!=NULL)
      {
        tempo = buff ;
//        result += tempo.substr (0, tempo.size () - 1) ;
        result += tempo ;
      }
    int returnCode = pclose (in) ;
 
    return pair<int, string> (returnCode, result) ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotterTools::plotHistos(){
  if(VERBOSE)  std::cout << "==================== Plotting histograms =======================" << std::endl;
  pair <int, string> outCode = execute ("ls " + string(outputDir_)) ;
  if (outCode.first != 0) outCode = execute ("mkdir " + string(outputDir_)) ;
  if (outCode.first != 0) 
    {
      cerr << "[PLOTTER] ERROR FILE " << outputFile_ 
           << ", problems creating the output folder:\n"
           << outCode.second << "\n"
           << "exiting\n" ;
      exit (1) ;
    }

  for (std::map<TString,varPlot<float>*>::const_iterator out=varplots.begin();out!=varplots.end();++out){
    if(out->second->doPlot==false) continue;
    if(out->first=="MatrixView") setPlotAxisRange(out->first,"Z",0,2000);
    if(out->first=="MatrixViewCeF3") setPlotAxisRange(out->first,"Z",0,400);
    if(out->second->type==kPlotGraph)  {
      setAxisTitles((TGraph*)out->second->GetPlot(),"Event",out->second->name.Data());
      plotMe((TGraph*)out->second->GetPlot(), Form("%s_%s",out->second->group.Data(),out->first.Data()));
    }else if(out->second->type==kPlot1D || out->second->type==kPlot1DProf)  {
      setAxisTitles((TH1F*)out->second->GetPlot(),out->second->name.Data(),"Events");
      plotMe((TH1F*)out->second->GetPlot(),Form("%s_%s",out->second->group.Data(),out->first.Data()));
    }else if(out->second->type==kPlot2D || out->second->type==kPlot2DProf)  {
      setAxisTitles((TH2F*)out->second->GetPlot(),((TAxis*)((TH2F*)out->second->GetPlot())->GetXaxis())->GetTitle(),((TAxis*)((TH2F*)out->second->GetPlot())->GetYaxis())->GetTitle());      
      plotMe((TH2F*)out->second->GetPlot(), Form("%s_%s",out->second->group.Data(),out->first.Data()));
    }
  if(VERBOSE)   std::cout << "==================== Canvas saved in \" "<< outputDir_<<"\" =======================" << std::endl;
}
}

void plotterTools::printHistos(){
  if(VERBOSE){
    std::cout << "==================== Booked histograms =======================" << std::endl;
    for (std::map<TString,varPlot<float>*>::const_iterator out=varplots.begin();out!=varplots.end();++out)
      std::cout << out->second->GetPlot()->GetName() << std::endl;
    std::cout << "==================== Loop over events =======================" << std::endl;
  }
}

void plotterTools::fillFiberOrder(){

  fiberOrderA.clear();
  fiberOrderB.clear();
//  fiberOrderC.clear();
//  fiberOrderD.clear();

  fiberOrderA.push_back(31);
  fiberOrderA.push_back(29);
  fiberOrderA.push_back(23);
  fiberOrderA.push_back(21);
  fiberOrderA.push_back(5);
  fiberOrderA.push_back(7);
  fiberOrderA.push_back(15);
  fiberOrderA.push_back(13);
  fiberOrderA.push_back(1);
  fiberOrderA.push_back(3);
  fiberOrderA.push_back(11);
  fiberOrderA.push_back(9);
  fiberOrderA.push_back(6);
  fiberOrderA.push_back(8);
  fiberOrderA.push_back(16);
  fiberOrderA.push_back(14);
  fiberOrderA.push_back(17);
  fiberOrderA.push_back(27);
  fiberOrderA.push_back(19);
  fiberOrderA.push_back(25);
  fiberOrderA.push_back(24);
  fiberOrderA.push_back(22);
  fiberOrderA.push_back(32);
  fiberOrderA.push_back(30);
  fiberOrderA.push_back(4);
  fiberOrderA.push_back(2);
  fiberOrderA.push_back(12);
  fiberOrderA.push_back(10);
  fiberOrderA.push_back(20);
  fiberOrderA.push_back(18);
  fiberOrderA.push_back(28);
  fiberOrderA.push_back(26);

  fiberOrderB.push_back(54);
  fiberOrderB.push_back(64);
  fiberOrderB.push_back(56);
  fiberOrderB.push_back(62);
  fiberOrderB.push_back(49);
  fiberOrderB.push_back(51);
  fiberOrderB.push_back(59);
  fiberOrderB.push_back(57);
  fiberOrderB.push_back(53);
  fiberOrderB.push_back(55);
  fiberOrderB.push_back(63);
  fiberOrderB.push_back(61);
  fiberOrderB.push_back(45);
  fiberOrderB.push_back(47);
  fiberOrderB.push_back(37);
  fiberOrderB.push_back(39);
  fiberOrderB.push_back(34);
  fiberOrderB.push_back(42);
  fiberOrderB.push_back(36);
  fiberOrderB.push_back(44);
  fiberOrderB.push_back(50);
  fiberOrderB.push_back(52);
  fiberOrderB.push_back(58);
  fiberOrderB.push_back(60);
  fiberOrderB.push_back(38);
  fiberOrderB.push_back(48);
  fiberOrderB.push_back(40);
  fiberOrderB.push_back(46);
  fiberOrderB.push_back(41);
  fiberOrderB.push_back(43);
  fiberOrderB.push_back(33);
  fiberOrderB.push_back(35);

//  fiberOrderC.clear();
//  fiberOrderD.clear();
//
//	// ORDER A  [A1,A2]-> [A2,A1]	
//	fiberOrderC.push_back(17);//12
//	fiberOrderC.push_back(27);//22
//	fiberOrderC.push_back(19);//15 --14
//	fiberOrderC.push_back(25); //21 --20
//	fiberOrderC.push_back(24); //20 -- 19
//	fiberOrderC.push_back(22); // 17
//	fiberOrderC.push_back(32);
//	fiberOrderC.push_back(30);//26,25
//	fiberOrderC.push_back(4  );// XXXX
//	fiberOrderC.push_back(2  );// XXXX
//	fiberOrderC.push_back(12);
//	fiberOrderC.push_back(10);//6
//	fiberOrderC.push_back(20); 
//	fiberOrderC.push_back(18);//13
//	fiberOrderC.push_back(28);//24,23
//	fiberOrderC.push_back(26);//21
//
//	fiberOrderC.push_back(31);//26
//	fiberOrderC.push_back(29);//24
//	fiberOrderC.push_back(23);
//	fiberOrderC.push_back(21);
//	fiberOrderC.push_back(5);
//	fiberOrderC.push_back(7);
//	fiberOrderC.push_back(15);
//	fiberOrderC.push_back(13);
//	fiberOrderC.push_back(1   ); //XXXX
//	fiberOrderC.push_back(3   ); //XXXX
//	fiberOrderC.push_back(11);
//	fiberOrderC.push_back(9);
//	fiberOrderC.push_back(6);
//	fiberOrderC.push_back(8);
//	fiberOrderC.push_back(16); //12
//	fiberOrderC.push_back(14);
//
//	// B [B1,B2] -> [B2,B1]
//	fiberOrderD.push_back(34);
//	fiberOrderD.push_back(42);
//	fiberOrderD.push_back(36);
//	fiberOrderD.push_back(44);
//	fiberOrderD.push_back(50);
//	fiberOrderD.push_back(52);
//	fiberOrderD.push_back(58);
//	fiberOrderD.push_back(60);
//	fiberOrderD.push_back(38);
//	fiberOrderD.push_back(48);
//	fiberOrderD.push_back(40);
//	fiberOrderD.push_back(46);
//	fiberOrderD.push_back(41);
//	fiberOrderD.push_back(43);
//	fiberOrderD.push_back(33);
//	fiberOrderD.push_back(35);
//
//	fiberOrderD.push_back(54);
//	fiberOrderD.push_back(64);
//	fiberOrderD.push_back(56);
//	fiberOrderD.push_back(62);
//	fiberOrderD.push_back(49);
//	fiberOrderD.push_back(51);
//	fiberOrderD.push_back(59);
//	fiberOrderD.push_back(57);
//	fiberOrderD.push_back(53);
//	fiberOrderD.push_back(55);
//	fiberOrderD.push_back(63);
//	fiberOrderD.push_back(61);
//	fiberOrderD.push_back(45);
//	fiberOrderD.push_back(47);
//	fiberOrderD.push_back(37);
//	fiberOrderD.push_back(39);

	return ;
}

int plotterTools::findPosition(std::vector<int>* fiberVec, int n){

  std::vector<int>::iterator iter=  std::find(fiberVec->begin(), fiberVec->end(),n);

  if(iter==fiberVec->end()){
    return -1;
  }else{
    return iter-fiberVec->begin();
  }

}
