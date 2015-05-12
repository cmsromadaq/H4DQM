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
//      cout << min << " " << p->GetBinContent (i) << endl ;
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



plotterTools::plotterTools(TString filename, TString outfname, TString outdname){

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

  fillFiberOrder();

  fillPmtInToOutMap();
  fillPmtOutToHodoXMap();
  fillPmtOutToHodoYMap();

  pedestalCut();

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
  
  if(hX_info!=NULL){
    
    integratedPlots_["hodo_meanX_spill"]=(TH1F*)integratedFile_->Get("hodo_meanX_spill");       
    integratedPlots_["hodo_meanY_spill"]=(TH1F*)integratedFile_->Get("hodo_meanY_spill");
    integratedPlots_["hodo_RMSX_spill"]=(TH1F*)integratedFile_->Get("hodo_RMSX_spill");
    integratedPlots_["hodo_RMSY_spill"]=(TH1F*)integratedFile_->Get("hodo_RMSY_spill"); 
    
    if(integratedPlots_["hodo_meanX_spill"]==NULL)
      integratedPlots_["hodo_meanX_spill"]=new TH1F("hodo_meanX_spill","hodo_meanX_spill",200,0,200);
    if(integratedPlots_["hodo_meanY_spill"]==NULL)
      integratedPlots_["hodo_meanY_spill"]=new TH1F("hodo_meanY_spill","hodo_meanY_spill",200,0,200);
    if(integratedPlots_["hodo_RMSX_spill"]==NULL)
      integratedPlots_["hodo_RMSX_spill"]=new TH1F("hodo_RMSX_spill","hodo_RMSX_spill",200,0,200);
    if(integratedPlots_["hodo_RMSY_spill"]==NULL)
      integratedPlots_["hodo_RMSY_spill"]=new TH1F("hodo_RMSY_spill","hodo_RMSY_spill",200,0,200);
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
      integratedPlots_["DAQStatus_nTotalEvtsPerSpillHisto"]=new TH1F("DAQStatus_nTotalEvtsPerSpillHisto","DAQStatus_nTotalEvtsPerSpillHisto",2000,100,10000);
      integratedPlots_["DAQStatus_fractionTakenTrigPerSpill"]=new TH1F("DAQStatus_fractionTakenTrigPerSpill","DAQStatus_fractionTakenTrigPerSpill",200,0,200);
      integratedPlots_["DAQStatus_triggerRateHisto"]=new TH1F("DAQStatus_triggerRateHisto","DAQStatus_triggerRateHisto",2000,0,1000);
      integratedPlots_["DAQStatus_triggerRatePerSpill"]=new TH1F("DAQStatus_triggerRatePerSpill","DAQStatus_triggerRatePerSpill",2000,0,1000);
      integratedPlots_["DAQStatus_growingEventPlot"]=new TH1F("DAQStatus_growingEventPlot","DAQStatus_growingEventPlot",2000,0,1000);
    }
  }
  
  if(hX_info!=NULL){
    
    int iBin=0;
    for(iBin=1;iBin<integratedPlots_["hodo_meanX_spill"]->GetNbinsX() && integratedPlots_["hodo_meanX_spill"]->GetBinContent(iBin)>0; ++iBin){}  // ???? 
    integratedPlots_["hodo_meanX_spill"]->SetBinContent(iBin,hX_info->GetMean());                                                                
    integratedPlots_["hodo_meanX_spill"]->SetBinError(iBin,hX_info->GetRMS());
  
    integratedPlots_["hodo_meanY_spill"]->SetBinContent(iBin,hY_info->GetMean());
    integratedPlots_["hodo_meanY_spill"]->SetBinError(iBin,hY_info->GetRMS());
    
    integratedPlots_["hodo_RMSX_spill"]->SetBinContent(iBin,hX_info->GetRMS());
    integratedPlots_["hodo_RMSY_spill"]->SetBinContent(iBin,hX_info->GetRMS());
  
    setAxisTitles(integratedPlots_["hodo_meanX_spill"], "nSpill","mean X" );
    setAxisTitles(integratedPlots_["hodo_meanY_spill"], "nSpill","mean Y" );
  
    setAxisTitles(integratedPlots_["hodo_RMSX_spill"], "nSpill","RMS X" );
    setAxisTitles(integratedPlots_["hodo_RMSY_spill"], "nSpill","RMS Y" );

    plotMe(integratedPlots_["hodo_meanX_spill"]);
    plotMe(integratedPlots_["hodo_meanY_spill"]);
    
    plotMe(integratedPlots_["hodo_RMSX_spill"]);
    plotMe(integratedPlots_["hodo_RMSY_spill"]);
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


  integratedPlots_["DAQStatus_triggerRatePerSpill"]->SetBinContent(iBin,100000*evt_info->GetEntries()/(timeEnd_[0]-timeStart_[0]));//it's in Hz
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

  integratedPlots_["DAQStatus_triggerRateHisto"]->Fill(100000*evt_info->GetEntries()/(timeEnd_[0]-timeStart_[0]));//it's in Hz
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
    histo->GetYaxis()->SetRangeUser(2000,3700);
    histo->SetStats(kFALSE);
    histo->Draw ("colz") ;
  }
  else { 
    histo->Draw ("colz") ;
  }
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
  graph->Draw ("ALP") ;
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

 } else if(name=="nFibersOnX"){

   int fibersOn=0;
   for(int i=0;i<32;i++){
     if(fibersOn_[hodoX][i]==1) fibersOn++;
   }
   varplots[name]->Fill(fibersOn,1.);     
   
 } else if(name=="nFibersOnY"){

   int fibersOn=0;
   for(int i=0;i<32;i++){
     if(fibersOn_[hodoY][i]==1) fibersOn++;
   }
   varplots[name]->Fill(fibersOn,1.);

 } else if(name=="beamProfileX2014"){    

   for(int i=0;i<32;i++){
     if(fibersOn_[hodoX][i]==1) varplots[name]->Fill(1,i);
     else varplots[name]->Fill(0,1.,i);
   }

 } else if(name=="beamProfileY2014"){    
   
   for(int i=0;i<32;i++){
     if(fibersOn_[hodoY][i]==1) varplots[name]->Fill(1,i);
     else varplots[name]->Fill(0,1.,i);
   }

 } else if(name=="beamProfileDrawX"){

   for(int i=0;i<32;i++){
     if(fibersOn_[hodoX][i]==1) varplots[name]->Fill(i,1.);
   }
   
 } else if(name=="beamProfileDrawY"){

   for(int i=0;i<32;i++){
     if(fibersOn_[hodoY][i]==1) varplots[name]->Fill(i,1.);
   }

 } else if(name=="beamProfileScatter"){

   for(int i=0;i<32;i++){
     for(int j=0;j<32;j++){
       if(fibersOn_[hodoX][i]==1 && fibersOn_[hodoY][j]==1) varplots[name]->Fill2D(i,j,1);
     }
   }
   
 } else if(name=="beamPositionX2014"){  
   
   float pos=0;
   int nFibersOn=0;
   for (int i=0;i<32;++i){   
     if(fibersOn_[hodoX][i]==1){
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
   
 } else if(name=="beamPositionY2014"){
   
   float pos=0;
   int nFibersOn=0;
   for (int i=0;i<32;++i){   
     if(fibersOn_[hodoY][i]==1){
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

 } else if(name=="beamPositionX"){

   onFibersX.clear();
   for (int i=0;i<32;++i){
     if (fibersOn_[hodoX][i]==1) {
       onFibersX.insert(std::pair<int,int>(i,fibersAdc_[hodoX][i]));
     }
   }

   int nClustersOn=0;
   
   if (onFibersX.size()>1) {

     map<int, int>::iterator iter;
	  
     float eneSum = 0.;
     float pos = 0.;
     int nFibersOn = 0;
     int theOneBefore = -1;
     int thisOne = -1;
     int thisValue = -1;

     for (iter = onFibersX.begin(); iter != onFibersX.end(); iter++) {
       
       if (theOneBefore==-1) {   // first round
	 thisOne   = iter->first;
	 thisValue = iter->second;
	 pos+=thisOne*(float)thisValue; 
	 eneSum+=(float)thisValue; 
	 nFibersOn++;
       } else {                 
	 thisOne = iter->first;
	 if (thisOne==(theOneBefore+1)) {       // adiacent fibers
	   thisValue = iter->second;
	   pos+=thisOne*(float)thisValue; 
	   eneSum+=(float)thisValue; 
	   nFibersOn++;
	 } else {            // there is a hole
	   if (nFibersOn>1 && nFibersOn<=3) {  // close the previous cluster
	     nClustersOn++;
	     pos=pos/eneSum;
	     varplots["beamPositionX"]->Fill(pos,1.);  
	     varplots["nFibersPerClusterX"]->Fill(nFibersOn,1);
	   } 
	   // start a new cluster
	   thisValue = iter->second;
	   nFibersOn = 1;
	   eneSum = (float)thisValue;
	   pos = thisOne*(float)thisValue;
	 }
       }
       theOneBefore = thisOne;
     }
     // close the last cluster if any
     if (nFibersOn>1 && nFibersOn<=3) {
       nClustersOn++;
       pos=pos/eneSum;
       varplots["beamPositionX"]->Fill(pos,1.);  
       varplots["nFibersPerClusterX"]->Fill(nFibersOn,1);
     }
   }
   varplots["nClustersX"]->Fill(nClustersOn,1.);     

 } else if(name=="beamPositionY"){
   
   onFibersY.clear();
   for (int i=0;i<32;++i){
     if (fibersOn_[hodoY][i]==1) {
       onFibersY.insert(std::pair<int,int>(i,fibersAdc_[hodoY][i]));
     }
   }

   int nClustersOn=0;
   
   if (onFibersY.size()>1) {
     
     map<int, int>::iterator iter;
	  
     float eneSum = 0.;
     float pos = 0.;
     int nFibersOn = 0;
     int theOneBefore = -1;
     int thisOne = -1;
     int thisValue = -1;

     for (iter = onFibersY.begin(); iter != onFibersY.end(); iter++) {

       if (theOneBefore==-1) {   // first round
	 thisOne   = iter->first;
	 thisValue = iter->second;
	 pos+=thisOne*(float)thisValue; 
	 eneSum+=(float)thisValue; 
	 nFibersOn++;
       } else {                 
	 thisOne = iter->first;
	 if (thisOne==(theOneBefore+1)) {       // adiacent fibers
	   thisValue = iter->second;
	   pos+=thisOne*(float)thisValue; 
	   eneSum+=(float)thisValue; 
	   nFibersOn++;
	 } else {            // there is a hole
	   if (nFibersOn>1 && nFibersOn<=3) {  // close the previous cluster
	     nClustersOn++;
	     pos=pos/eneSum;
	     varplots["beamPositionY"]->Fill(pos,1.);  
	     varplots["nFibersPerClusterY"]->Fill(nFibersOn,1);
	   } 
	   // start a new cluster
	   thisValue = iter->second;
	   nFibersOn = 1;
	   eneSum = (float)thisValue;
	   pos = thisOne*(float)thisValue;
	 }
       }
       theOneBefore = thisOne;
     }
     // close the last cluster if any
     if (nFibersOn>1 && nFibersOn<=3) {
       nClustersOn++;
       pos=pos/eneSum;
       varplots["beamPositionY"]->Fill(pos,1.);  
       varplots["nFibersPerClusterY"]->Fill(nFibersOn,1);
     }
   }
   varplots["nClustersY"]->Fill(nClustersOn,1.);     

 }else if(name=="fractionTakenTrig"){//DAQ Status
   //   varplots[name]->Fill(((float)treeStruct_.scalerWord[2]/treeStruct_.scalerWord[1]),1.);
 }else if(name=="fractionTakenTrigHisto"){//DAQ Status
   //   varplots[name]->Fill(((float)treeStruct_.scalerWord[2]/treeStruct_.scalerWord[1]),1.);
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
      fibersAdc_[i][j]=0;                
    }
  }
  
  for (UInt_t j=0; j<treeStruct_.nAdcChannels; j++) {
    
    if(treeStruct_.adcBoard[j]==201392129){
      
      int theAdcValue   = treeStruct_.adcData[j];
      int theAdcChannel = treeStruct_.adcChannel[j];
      int thePmtOut     = (PmtInToOut.find(theAdcChannel))->second;

      if (theAdcChannel>63) continue;          // spare channels 

      float pedValue = (B64ped.find(theAdcChannel))->second;
      float pedRms   = (B64rms.find(theAdcChannel))->second;
      float pedThres = (B64thr.find(theAdcChannel))->second;

      int theHodoPixel = -999; 
      if (thePmtOut>=0 && thePmtOut<32) { 
	theHodoPixel = (PmtIntoHodoX.find(thePmtOut))->second;
	fibersAdc_[0][theHodoPixel]=theAdcValue-(int)pedValue;
	if (theAdcValue>pedThres) fibersOn_[0][theHodoPixel] = 1;
      } else if (thePmtOut>=32 && thePmtOut<64) {
	theHodoPixel = (PmtIntoHodoY.find(thePmtOut))->second;
	fibersAdc_[1][theHodoPixel]=theAdcValue-(int)pedValue;
	if (theAdcValue>pedThres) fibersOn_[1][theHodoPixel] = 1;
      } else {
	std::cout << "problem with hodo mapping! " << std::endl;
      }
    }
  }
}

// 2015 hodo mapping
void plotterTools::fillPmtInToOutMap() {
  
  // PMT In - PNT Out  [both 0->63]
  PmtInToOut.clear();
  PmtInToOut.insert(std::pair<int,int>(0,35));
  PmtInToOut.insert(std::pair<int,int>(1,41));
  PmtInToOut.insert(std::pair<int,int>(2,48));
  PmtInToOut.insert(std::pair<int,int>(3,56));
  PmtInToOut.insert(std::pair<int,int>(4,57));
  PmtInToOut.insert(std::pair<int,int>(5,49));
  PmtInToOut.insert(std::pair<int,int>(6,58));
  PmtInToOut.insert(std::pair<int,int>(7,50));
  PmtInToOut.insert(std::pair<int,int>(8,32));
  PmtInToOut.insert(std::pair<int,int>(9,59));
  PmtInToOut.insert(std::pair<int,int>(10,33));
  PmtInToOut.insert(std::pair<int,int>(11,51));
  PmtInToOut.insert(std::pair<int,int>(12,40));
  PmtInToOut.insert(std::pair<int,int>(13,60));
  PmtInToOut.insert(std::pair<int,int>(14,34));
  PmtInToOut.insert(std::pair<int,int>(15,52));
  PmtInToOut.insert(std::pair<int,int>(16,42));
  PmtInToOut.insert(std::pair<int,int>(17,61));
  PmtInToOut.insert(std::pair<int,int>(18,43));
  PmtInToOut.insert(std::pair<int,int>(19,53));
  PmtInToOut.insert(std::pair<int,int>(20,62));
  PmtInToOut.insert(std::pair<int,int>(21,44));
  PmtInToOut.insert(std::pair<int,int>(22,63));
  PmtInToOut.insert(std::pair<int,int>(23,54));
  PmtInToOut.insert(std::pair<int,int>(24,36));
  PmtInToOut.insert(std::pair<int,int>(25,55));
  PmtInToOut.insert(std::pair<int,int>(26,46));
  PmtInToOut.insert(std::pair<int,int>(27,45));
  PmtInToOut.insert(std::pair<int,int>(28,47));
  PmtInToOut.insert(std::pair<int,int>(29,38));
  PmtInToOut.insert(std::pair<int,int>(30,37));
  PmtInToOut.insert(std::pair<int,int>(31,39));
  PmtInToOut.insert(std::pair<int,int>(32,29));
  PmtInToOut.insert(std::pair<int,int>(33,31));
  PmtInToOut.insert(std::pair<int,int>(34,21));
  PmtInToOut.insert(std::pair<int,int>(35,30));
  PmtInToOut.insert(std::pair<int,int>(36,23));
  PmtInToOut.insert(std::pair<int,int>(37,28));
  PmtInToOut.insert(std::pair<int,int>(38,22));
  PmtInToOut.insert(std::pair<int,int>(39,15));
  PmtInToOut.insert(std::pair<int,int>(40,20));
  PmtInToOut.insert(std::pair<int,int>(41,14));
  PmtInToOut.insert(std::pair<int,int>(42,7));
  PmtInToOut.insert(std::pair<int,int>(43,27));
  PmtInToOut.insert(std::pair<int,int>(44,6));
  PmtInToOut.insert(std::pair<int,int>(45,13));
  PmtInToOut.insert(std::pair<int,int>(46,3));
  PmtInToOut.insert(std::pair<int,int>(47,5));
  PmtInToOut.insert(std::pair<int,int>(48,12));
  PmtInToOut.insert(std::pair<int,int>(49,4));
  PmtInToOut.insert(std::pair<int,int>(50,11));
  PmtInToOut.insert(std::pair<int,int>(51,19));
  PmtInToOut.insert(std::pair<int,int>(52,10));
  PmtInToOut.insert(std::pair<int,int>(53,18));
  PmtInToOut.insert(std::pair<int,int>(54,2));
  PmtInToOut.insert(std::pair<int,int>(55,17));
  PmtInToOut.insert(std::pair<int,int>(56,9));
  PmtInToOut.insert(std::pair<int,int>(57,16));
  PmtInToOut.insert(std::pair<int,int>(58,1));
  PmtInToOut.insert(std::pair<int,int>(59,0));
  PmtInToOut.insert(std::pair<int,int>(60,25));
  PmtInToOut.insert(std::pair<int,int>(61,8));
  PmtInToOut.insert(std::pair<int,int>(62,26));
  PmtInToOut.insert(std::pair<int,int>(63,24));
}

// 2015 hodo mapping
void plotterTools::fillPmtOutToHodoXMap() {

  // PMT Out - Hodo X  [both 0->63]
  PmtIntoHodoX.clear();
  PmtIntoHodoX.insert(std::pair<int,int>(0,28));
  PmtIntoHodoX.insert(std::pair<int,int>(1,17));
  PmtIntoHodoX.insert(std::pair<int,int>(2,16));
  PmtIntoHodoX.insert(std::pair<int,int>(3,15));
  PmtIntoHodoX.insert(std::pair<int,int>(4,14));
  PmtIntoHodoX.insert(std::pair<int,int>(5,13));
  PmtIntoHodoX.insert(std::pair<int,int>(6,12));
  PmtIntoHodoX.insert(std::pair<int,int>(7,0));
  PmtIntoHodoX.insert(std::pair<int,int>(8,29));
  PmtIntoHodoX.insert(std::pair<int,int>(9,18));
  PmtIntoHodoX.insert(std::pair<int,int>(10,19));
  PmtIntoHodoX.insert(std::pair<int,int>(11,20));
  PmtIntoHodoX.insert(std::pair<int,int>(12,9));
  PmtIntoHodoX.insert(std::pair<int,int>(13,10));
  PmtIntoHodoX.insert(std::pair<int,int>(14,11));
  PmtIntoHodoX.insert(std::pair<int,int>(15,1));
  PmtIntoHodoX.insert(std::pair<int,int>(16,30));
  PmtIntoHodoX.insert(std::pair<int,int>(17,23));
  PmtIntoHodoX.insert(std::pair<int,int>(18,22));
  PmtIntoHodoX.insert(std::pair<int,int>(19,21));
  PmtIntoHodoX.insert(std::pair<int,int>(20,8));
  PmtIntoHodoX.insert(std::pair<int,int>(21,7));
  PmtIntoHodoX.insert(std::pair<int,int>(22,6));
  PmtIntoHodoX.insert(std::pair<int,int>(23,2));
  PmtIntoHodoX.insert(std::pair<int,int>(24,31));
  PmtIntoHodoX.insert(std::pair<int,int>(25,24));
  PmtIntoHodoX.insert(std::pair<int,int>(26,25));
  PmtIntoHodoX.insert(std::pair<int,int>(27,26));
  PmtIntoHodoX.insert(std::pair<int,int>(28,27));
  PmtIntoHodoX.insert(std::pair<int,int>(29,5));
  PmtIntoHodoX.insert(std::pair<int,int>(30,4));
  PmtIntoHodoX.insert(std::pair<int,int>(31,3));
}

// 2015 hodo mapping
void plotterTools::fillPmtOutToHodoYMap() {

  // PMT Out - Hodo Y  [both 0->63]
  PmtIntoHodoY.clear();
  PmtIntoHodoY.insert(std::pair<int,int>(32,3));
  PmtIntoHodoY.insert(std::pair<int,int>(33,4));
  PmtIntoHodoY.insert(std::pair<int,int>(34,5));
  PmtIntoHodoY.insert(std::pair<int,int>(35,27));
  PmtIntoHodoY.insert(std::pair<int,int>(36,26));
  PmtIntoHodoY.insert(std::pair<int,int>(37,25));
  PmtIntoHodoY.insert(std::pair<int,int>(38,24));
  PmtIntoHodoY.insert(std::pair<int,int>(39,31));
  PmtIntoHodoY.insert(std::pair<int,int>(40,2));
  PmtIntoHodoY.insert(std::pair<int,int>(41,6));
  PmtIntoHodoY.insert(std::pair<int,int>(42,7));
  PmtIntoHodoY.insert(std::pair<int,int>(43,8));
  PmtIntoHodoY.insert(std::pair<int,int>(44,21));
  PmtIntoHodoY.insert(std::pair<int,int>(45,22));
  PmtIntoHodoY.insert(std::pair<int,int>(46,23));
  PmtIntoHodoY.insert(std::pair<int,int>(47,30));
  PmtIntoHodoY.insert(std::pair<int,int>(48,1));
  PmtIntoHodoY.insert(std::pair<int,int>(49,11));
  PmtIntoHodoY.insert(std::pair<int,int>(50,10));
  PmtIntoHodoY.insert(std::pair<int,int>(51,9));
  PmtIntoHodoY.insert(std::pair<int,int>(52,20));
  PmtIntoHodoY.insert(std::pair<int,int>(53,19));
  PmtIntoHodoY.insert(std::pair<int,int>(54,18));
  PmtIntoHodoY.insert(std::pair<int,int>(55,29));
  PmtIntoHodoY.insert(std::pair<int,int>(56,0));
  PmtIntoHodoY.insert(std::pair<int,int>(57,12));
  PmtIntoHodoY.insert(std::pair<int,int>(58,13));
  PmtIntoHodoY.insert(std::pair<int,int>(59,14));
  PmtIntoHodoY.insert(std::pair<int,int>(60,15));
  PmtIntoHodoY.insert(std::pair<int,int>(61,16));
  PmtIntoHodoY.insert(std::pair<int,int>(62,17));
  PmtIntoHodoY.insert(std::pair<int,int>(63,28));
}

// 2015: map with pedestal mean and rms per crystal
void plotterTools::pedestalCut(){

  B64ped.clear();
  B64rms.clear();
  B64thr.clear();

  char Buffer[800];  
  int theBoard[66];
  int theChannel[66];
  float thePed[66];
  float thePedRms[66];
  float theChi2[66];
  float theThreshold[66];
  int iChannel=0;
  ifstream *inputFile = new ifstream("../test/pedestalScan.txt");   
  while( !(inputFile->eof()) ) {
    inputFile->getline(Buffer,800);
    if (!strstr(Buffer,"#") && !(strspn(Buffer," ") == strlen(Buffer))) {
      sscanf(Buffer,"%d %d %f %f %f %f",&theBoard[iChannel],&theChannel[iChannel],&thePed[iChannel],&thePedRms[iChannel],&theChi2[iChannel],&theThreshold[iChannel]);
      if (theBoard[iChannel]==201392129) {
	B64ped.insert(std::pair<int,float>(theChannel[iChannel],thePed[iChannel]));
	B64rms.insert(std::pair<int,float>(theChannel[iChannel],thePedRms[iChannel]));
	B64thr.insert(std::pair<int,float>(theChannel[iChannel],theThreshold[iChannel]));
      }
      iChannel++;
    }
  }
  inputFile->close();
  delete inputFile;
}

void plotterTools::fillTdc(){

  for (uint j=0; j<MaxTdcChannels; j++){
    tdc_readings[j].clear();
  }
  tdc_recox=-999;
  tdc_recoy=-999;

  for (uint i=0; i<treeStruct_.nTdcChannels; i++){
    if (treeStruct_.tdcBoard[i]==0x07030001 && treeStruct_.tdcChannel[i]<MaxTdcChannels){
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
  
  for (set<int>::iterator iGroup = groups.begin () ; 
       iGroup != groups.end () ; ++iGroup)
    {
      for (set<int>::iterator iChannel = channels.begin () ; 
           iChannel != channels.end () ; ++iChannel)
        {
	  if (*iChannel>=nActiveDigitizerChannels) continue;
	  
          TString name = getDigiChannelName(*iGroup,*iChannel);
          addPlot(1,Form("%s_pulse",name.Data()), xNbins, xmin, xmax, yNbins, ymin, ymax, "time", "voltage", "2D", group_, module_, 1, true) ;
	  addPlot(1,Form("%s_pedestal",name.Data()),4096,0,4096,"1D",group_,module_);
	  addPlot(1,Form("%s_pedestal_rms",name.Data()),200,0,50,"1D",group_,module_);
	  addPlot(1,Form("%s_max_amplitude",name.Data()),300,0,3000,"1D",group_,module_);
	  addPlot(1,Form("%s_charge_integrated",name.Data()),200,0,5e4,"1D",group_,module_);
	  addPlot(1,Form("%s_charge_integrated_vs_hodoX",name.Data()),33,-1,32,-999999,999999,"1DProf",group_,module_);
	  addPlot(1,Form("%s_charge_integrated_vs_hodoY",name.Data()),33,-1,32,-999999,999999,"1DProf",group_,module_);
	  addPlot(0,Form("%s_time_at_max",name.Data()),xNbins,xmin,xmax,"1D",group_,module_);
	  addPlot(0,Form("%s_time_at_frac30",name.Data()),xNbins,xmin,xmax,"1D",group_,module_);
	  addPlot(0,Form("%s_time_at_frac50",name.Data()),xNbins,xmin,xmax,"1D",group_,module_);
	  
	  TString thisname = Form("%s_%s",name.Data(),"pulse");
 	  varplots[thisname]->waveform = new Waveform();
        }
    }
  
  addPlot(1,Form("allCh_charge_integrated_map"), 8, 0, 8, 8, 0, 8, -999999, 999999, "x", "y", "2DProf", group_, module_, 1, true) ;
  addPlot(1,Form("allCh_max_amplitude_map"), 8, 0, 8, 8, 0, 8, -999999, 999999, "x", "y", "2DProf", group_, module_, 1, true) ;
  addPlot(1,Form("allCh_pedestal_map"), 8, 0, 8, 8, 0, 8, 3200., 3800., "x", "y", "2DProf", group_, module_, 1, true) ;
  addPlot(1,Form("allCh_pedestal_rms_map"), 8, 0, 8, 8, 0, 8, 2.0, 4.0, "x", "y", "2DProf", group_, module_, 1, true) ;
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
  if( chId ==  0 ) return 0;
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
  if( chId == 31 ) return 7;
  return -1;
}


int plotterTools::getDigiChannelY(TString name){
  std::string nameStr = std::string(name);
  std::size_t pos = nameStr.find("ch");
  std::string chStr = nameStr.substr(pos+2,2);
  int chId = atoi(chStr.c_str());
  if( chId ==  0 ) return 7;
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
  if( chId == 31 ) return 0;
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
//  br = new outTreeBranch<float,float>("ADCvalues",&varplots);
//  if (wantADCplots){
//    //    for (int i=0; i<24; i++) br->addMember(Form("ADC_board_6301_%d",i)); // BGO
//    for (int i=4; i<8; i++) br->addMember(Form("ADC_board_11301_%d",i)); // BEAM SCINTILLATORS
//  }
//  else br->addDummy(28);
//  br->addMember("TDCrecoX"); br->addMember("TDCrecoY"); // WIRE CHAMBER
//  for (int i=0; i<64; i++) br->addMember("beamProfileX",i); 
//  for (int i=0; i<64; i++) br->addMember("beamProfileY",i);  
//  treevars[br->name]=br;
  
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
  br = new outTreeBranch<float,float>("SCINTvalues",&varplots);
  if (wantADCplots) for (int i=4; i<8; i++) br->addMember(Form("ADC_board_11301_%d",i)); // BEAM SCINTILLATORS
  else br->addDummy(4);
  treevars[br->name]=br;
  br = new outTreeBranch<float,float>("TDCreco",&varplots);
  br->addMember("TDCrecoX"); br->addMember("TDCrecoY"); // WIRE CHAMBER  
  treevars[br->name]=br;

  outTreeBranch<bool,float> *br2 = NULL;
  br2 = new outTreeBranch<bool,float>("HODOX",&varplots);
  for (int i=0; i<32; i++) br2->addMember("beamProfileX",i);
  treevars2[br2->name]=br2;
  br2 = new outTreeBranch<bool,float>("HODOY",&varplots);
  for (int i=0; i<32; i++) br2->addMember("beamProfileY",i);
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

  uint nentries = getTreeEntries();
  int nBinsHistory=nentries/getStepHistoryPlots();

  std::cout << "entries = " << nentries << endl;

  // nentries=1;

  //loop and fill histos
  for (unsigned iEntry = 0 ; iEntry < nentries ; ++iEntry)    
  {
      inputTree_->GetEntry(iEntry);

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
	    
	    TString thisname = it->second->name.ReplaceAll("_pulse","");
	    varplots[Form("%s_pedestal",thisname.Data())]->Fill(wave_pedestal.pedestal,1.);
	    varplots[Form("%s_pedestal_rms",thisname.Data())]->Fill(wave_pedestal.rms,1.);
	    varplots[Form("%s_max_amplitude",thisname.Data())]->Fill(wave_max.max_amplitude,1.);
	    varplots[Form("%s_charge_integrated",thisname.Data())]->Fill(it->second->waveform->charge_integrated(0,900),1.); // pedestal already subtracted
	    varplots[Form("%s_time_at_max",thisname.Data())]->Fill(wave_max.time_at_max*1.e9,1.);
	    varplots[Form("%s_time_at_frac30",thisname.Data())]->Fill(it->second->waveform->time_at_frac(wave_max.time_at_max-3.e-9,wave_max.time_at_max,0.3,wave_max,7)*1.e9,1.);
	    varplots[Form("%s_time_at_frac50",thisname.Data())]->Fill(it->second->waveform->time_at_frac(wave_max.time_at_max-3.e-9,wave_max.time_at_max,0.5,wave_max,7)*1.e9,1.);
	   
	    int x1 = -1;
	    for(int i=0;i<32;i++){
	      if(fibersOn_[hodoX][i]==1 && x1==-1) x1 = i;
	      if(fibersOn_[hodoX][i]==1 && x1!=-1) { x1 = -1; break; }
	    }
	    int y1 = -1;
	    for(int i=0;i<32;i++){
	      if(fibersOn_[hodoY][i]==1 && y1==-1) y1 = i;
	      if(fibersOn_[hodoY][i]==1 && y1!=-1) { y1 = -1; break; }
	    }

	    varplots[Form("%s_charge_integrated_vs_hodoX",thisname.Data())]->Fill(x1,it->second->waveform->charge_integrated(0,900)); // pedestal already subtracted
	    varplots[Form("%s_charge_integrated_vs_hodoY",thisname.Data())]->Fill(y1,it->second->waveform->charge_integrated(0,900)); // pedestal already subtracted
	    
	    int x = getDigiChannelX(it->second->name);
	    int y = getDigiChannelY(it->second->name);
	    varplots["allCh_charge_integrated_map"]->Fill2D(x,y,it->second->waveform->charge_integrated(0,900)); // pedestal already subtracted	
	    varplots["allCh_max_amplitude_map"]->Fill2D(x,y,wave_max.max_amplitude); // pedestal already subtracted
	    varplots["allCh_pedestal_map"]->Fill2D(x,y,wave_pedestal.pedestal); // pedestal already subtracted
	    varplots["allCh_pedestal_rms_map"]->Fill2D(x,y,wave_pedestal.rms); // pedestal already subtracted
	    sum_amplitudes+=wave_max.max_amplitude;
	  }
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

  addPlot(0,"beamProfileX2014", 32,-0.5, 31.5,"1D",group_,module_,32);//simple TH1F
  addPlot(0,"beamProfileY2014", 32,-0.5, 31.5,"1D",group_,module_,32);//simple TH1F

  addPlot(1,"beamProfileDrawX", 32,-0.5, 31.5,"1D",group_,module_,32);//simple TH1F    
  addPlot(1,"beamProfileDrawY", 32,-0.5, 31.5,"1D",group_,module_,32);//simple TH1F    

  addPlot(1,"beamProfileScatter", 32,-0.5, 31.5, 32,-0.5, 31.5,"","","2D",group_,module_,0,0);

  addPlot(1,"nFibersOnX", 32,-0.5, 31.5,"1D",group_,module_);//simple TH1F       
  addPlot(1,"nFibersOnY", 32,-0.5, 31.5,"1D",group_,module_);//simple TH1F       

  addPlot(0,"beamPositionX2014", 32,-0.5, 31.5,"1D",group_,module_);//simple TH1F
  addPlot(0,"beamPositionY2014", 32,-0.5, 31.5,"1D",group_,module_);//simple TH1F

  addPlot(1,"beamPositionX", 32,-0.5, 31.5,"1D",group_,module_);//simple TH1F  
  addPlot(1,"beamPositionY", 32,-0.5, 31.5,"1D",group_,module_);//simple TH1F  

  addPlot(1,"nClustersX", 16,-0.5, 15.5,"1D",group_,module_); //simple TH1F    
  addPlot(1,"nClustersY", 16,-0.5, 15.5,"1D",group_,module_); //simple TH1F    

  addPlot(1,"nFibersPerClusterX", 2,1.5, 3.5,"1D",group_,module_); //simple TH1F    
  addPlot(1,"nFibersPerClusterY", 2,1.5, 3.5,"1D",group_,module_); //simple TH1F    
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
  addPlot(0,"TDCrecoX",100,-50,50,"1D",group_,module_);
  addPlot(0,"TDCrecoY",100,-50,50,"1D",group_,module_);
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
