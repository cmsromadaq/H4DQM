#include <plotterTools.hpp>


plotterTools::plotterTools(char* filename, char*outfname, char* outdname){

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

};


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


void  plotterTools::plotMe (TH1F * histo)
{
  TString hname = histo->GetName () ;
  TString canvasName = outputDir_+ "/"+hname + "_small.png" ;
  TCanvas * c1 = new TCanvas ("c1", "c1", 300, 300) ;
  histo->Draw () ;
  c1->Print (canvasName, "png") ;
  canvasName =outputDir_+ "/"+ hname + "_large.png" ;
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

  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadBottomMargin(0.13);//0.13);
  gStyle->SetPadLeftMargin(0.17);//0.16);
  gStyle->SetPadRightMargin(0.13);//0.02);
  
  gStyle->SetStatX (0.85) ;
  gStyle->SetStatY (0.45) ;
  gStyle->SetStatW (0.2) ;
  gStyle->SetStatH (0.15) ;




  TString hname = histo->GetName () ;
  TString canvasName =  outputDir_+ "/"+hname + "_small.png" ;
  TCanvas * c1 = new TCanvas ("c1", "c1", 300, 300) ;
  histo->Draw ("colz") ;
  c1->Print (canvasName, "png") ;
  canvasName = outputDir_+ "/"+hname + "_large.png" ;
  delete c1 ;
  c1 = new TCanvas ("c1", "c1", 800, 800) ;
  histo->Draw ("colz") ;
  c1->Print (canvasName, "png") ;
  delete c1 ;
  return ;

  setPlotsFormat();

}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void  plotterTools::plotMe (TGraph * graph, const TString & name)
{
  TString canvasName =  outputDir_+ "/" +name + "_small.png" ;
  TCanvas * c1 = new TCanvas ("c1", "c1", 300, 300) ;
  graph->SetMarkerStyle (8) ;
  graph->SetMarkerSize (1.5) ;
  graph->SetMarkerColor (kBlue) ;  
  graph->Draw ("ALP") ;
  c1->Print (canvasName, "png") ;
  canvasName = outputDir_+ "/"+name + "_large.png" ;

  delete c1 ;
  c1 = new TCanvas ("c1", "c1", 800, 800) ;
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

void plotterTools::initVariable(TString name,int varDim){
  variablesIterator_[name]=variables_.size();
  variables_.resize(variables_.size()+1);
  variablesMap_[name]=&variables_[variables_.size()];
  variablesContainer_[variablesIterator_[name]].resize(varDim);
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

void plotterTools::computeVariable(TString name, int varDim){
  //TODO use same structure for multi and unidimensional variables
  if(varDim != 1) {
    variablesContainer_[variablesIterator_[name]].resize(varDim);
    variables_[variablesIterator_[name]]=variablesContainer_[variablesIterator_[name]][0];
  }
 
 if(name=="triggerEff"){
    variables_[variablesIterator_[name]]=((float)treeStruct_.scalerWord[2]/treeStruct_.scalerWord[1]);
    //    *(variablesMap_.find(name)->second)=((float)treeStruct_.scalerWord[2]/treeStruct_.scalerWord[1]);
  }else if(name=="nEvts"){
    variables_[variablesIterator_[name]]=((float)treeStruct_.evtNumber);
  }else if(name=="nTrigSPS"){
    variables_[variablesIterator_[name]]=((float)treeStruct_.scalerWord[1]);
  }else if(name=="nTrigSPSVsnTrig"){
    variablesContainer_[variablesIterator_[name]][0]=((float)treeStruct_.scalerWord[1]);
    variablesContainer_[variablesIterator_[name]][1]=((float)treeStruct_.scalerWord[2]);
  }else if(name=="nTrigSPSVsnTrig3D"){
    variablesContainer_[variablesIterator_[name]][0]=((float)treeStruct_.scalerWord[1]);
    variablesContainer_[variablesIterator_[name]][1]=((float)treeStruct_.scalerWord[2]);
    variablesContainer_[variablesIterator_[name]][2]=((float)treeStruct_.scalerWord[0]);
 }else if(name=="nFibersOn"){

   const int nPlanes=4;
   const int nFibers=64;

   bool fibersOn[nPlanes][nFibers];

   const int nPlanesSmall=2;
   const int nFibersSmall=8;

   bool fibersOnSmall[nPlanesSmall][nFibersSmall];



   for(int i=0;i<treeStruct_.nPatterns;++i){

   if(treeStruct_.patternBoard[i]==0x08030001 || treeStruct_.patternBoard[i]==0x08030002){
     int word = (treeStruct_.patternBoard[i]==0x08030001) ? 0 : 1;
     for(int j=1;j<=64;j++){
       fibersOn[2*word+treeStruct_.patternChannel[i]/2][j-1]=0;
       std::vector<int> *x =(bool)( treeStruct_.patternChannel[i]&0b1) ? &fiberOrderA : &fiberOrderB;
       int y=findPosition(x,j);
       if(y<0) continue;
       fibersOn[word*2+treeStruct_.patternChannel[i]%2][j-1]=(treeStruct_.pattern[i]>>(uint)y)&0b1;
     }
   }else if(treeStruct_.patternBoard[i]==0x08010001){

       WORD wordX=(treeStruct_.pattern[9]& 0x0000FF00)>>8;
       WORD wordY= (treeStruct_.pattern[9] & 0x000000FF);


       for(int i=0;i<8;++i){
	 fibersOnSmall[0][i]=(bool)((wordX>>i)&0b1);
	 fibersOnSmall[1][i]=(bool)((wordY>>i)&0b1);
	 std::cout<<fibersOnSmall[0][i]<<" "<<fibersOnSmall[1][i]<<"----";
       }
     }
     //     }
   
   }





   for(int i=0;i<8;i++)	 std::cout<<fibersOnSmall[0][i]<<" "<<fibersOnSmall[1][i]<<"----";

 }

}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void  plotterTools::readInputTree ()
{
  //Instantiate the tree branches
  inputTree_->Print();

  inputTree_->SetBranchAddress("evtNumber"    ,&treeStruct_.evtNumber);
  inputTree_->SetBranchAddress("evtTimeDist"    ,&treeStruct_.evtTimeDist);
  inputTree_->SetBranchAddress("evtTimeStart"    ,&treeStruct_.evtTimeStart);

  inputTree_->SetBranchAddress("evtTime1"    ,&treeStruct_.evtTime1);
  inputTree_->SetBranchAddress("evtTime2"    ,&treeStruct_.evtTime2);
  inputTree_->SetBranchAddress("evtTime3"    ,&treeStruct_.evtTime3);

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


void  plotterTools::Loop()
{

  int nentries = getTreeEntries();
  nentries=1;
  int nBinsHistory=nentries/getStepHistoryPlots();

  //loop and fill histos
  for (unsigned iEntry = 0 ; iEntry < nentries ; ++iEntry) 
    {
      inputTree_->GetEntry(iEntry);
  
      for (std::map<TString,float*>::const_iterator iter = variablesMap_.begin ();
           iter != variablesMap_.end () ; ++iter)
        {
          if(plotLongNames_[iter->first].Contains("1D"))
            {
              if (iEntry%1000==0) std::cout<<"iEntry: "<<iEntry<<"/"<<nentries<<endl;
              FillPlot (iter->first, false, variablesContainer_[variablesIterator_[iter->first]].size ()) ;
            } else if (plotLongNames_[iter->first].Contains ("2D"))
            {
              FillPlot (iter->first, true) ;
            }
           
          if (iEntry%historyStep_==0 && iEntry!=0)
            {
              if ( (int)iEntry/historyStep_-1 < nBinsHistory) //all history plots should go here
                {
                  if (plotLongNames_[iter->first].Contains("history"))
                    {
                      FillPlot(iter->first,(int)iEntry/historyStep_-1,iEntry);
                    }
                }
            }
        }
    } // loop over the events
}


//for TGraph
void plotterTools::FillPlot(TString name, int point, float X)
{
  computeVariable (name) ;
  ((TGraph*) outObjects_[plotLongNames_[name]])->SetPoint(point, X, variables_[variablesIterator_[name]]) ;
}

//for TH1F and TH2F
void plotterTools::FillPlot(TString name, bool is2D, int varDim){
  if(!is2D){
    computeVariable(name,varDim);  
    if(!(varDim>1)){
      ((TH1F*) outObjects_[plotLongNames_[name]])->Fill(variables_[variablesIterator_[name]]);
    }else{
      for(int i=0;i<varDim;i++)      ((TH1F*) outObjects_[plotLongNames_[name]])->Fill(variablesContainer_[variablesIterator_[name]][i]);
    }
  }else {
    computeVariable(name,2);
    ((TH2F*) outObjects_[plotLongNames_[name]])->Fill(variablesContainer_[variablesIterator_[name]][0],variablesContainer_[variablesIterator_[name]][1]);
  }
  
}


void plotterTools::bookPlotsScaler(int nBinsHistory){
  //in this function you define all the objects for the scaler
  addPlot("nEvts", nBinsHistory, "history", group_,module_);//simple TGraph
  addPlot("triggerEff",nBinsHistory, "history", group_,module_);//TGraph with more complex variable
  addPlot("nTrigSPS", 100,0,3000, "1D",group_,module_);//simple TH1F
  addPlot("nTrigSPSVsnTrig", 100,0,3000, 100,0,3000,"nTrigSPS","nTrig","2D",group_,module_);//simple TH2F
  addPlot("nTrigSPSVsnTrig3D", 100,0,3000, "1D",group_,module_,3);// TH1F with more than one variable to fill per event

  addPlot("nFibersOn", 64,0, 64,"1D",group_,module_);//simple TH1F

}

void plotterTools::bookCombinedPlots(){
  addPlotCombined("nTrigSPSVsnTrig3DvsnEvts","nTrigSPS","nTrigSPSVsnTrig3D","2D",group_,module_);//correlation plots it uses TH1F done before to build this TH2

}


void plotterTools::addPlotCombined(TString name, TString name1, TString name2,TString type, TString group , TString module){
  initVariable(name);
  TString longName=group+TString("_")+module+TString("_")+type+TString("_")+name;
  outObjects_[longName]=((TObject*) bookHistoCombined(name,name1,name2));
  plotLongNames_[name]=longName;
  plotShortNames_[longName]=name;

  ((TH2F*)outObjects_[longName])->SetTitle(name);
  ((TH2F*)outObjects_[longName])->SetName(plotLongNames_[name]);


  ((TH2F*)outObjects_[longName])->SetXTitle(((TH1F* )outObjects_[plotLongNames_[name1]])->GetTitle());
  ((TH2F*)outObjects_[longName])->SetYTitle(((TH1F* )outObjects_[plotLongNames_[name2]])->GetTitle());

}

//for TGraph
TGraph *
plotterTools::addPlot(TString name,int nPoints,TString type, TString group, TString module){

  initVariable(name);

    TString longName=group+TString("_")+module+TString("_")+type+TString("_")+name;
    outObjects_[longName]=((TObject*)  bookGraph(name,nPoints,type, group_,module_));
    plotLongNames_[name]=longName;
    plotShortNames_[longName]=name;
    return dynamic_cast<TGraph *> (outObjects_[longName]) ;
}

//for TH1F
TH1F * plotterTools::addPlot(TString name,int nBinsX, float xMin, float xMax, TString type, TString group, TString module, int varDim){
  initVariable(name,varDim);

   TString longName=group+TString("_")+module+TString("_")+type+TString("_")+name;
   outObjects_[longName]=((TObject*) bookHisto(name,nBinsX, xMin, xMax, type, group_,module_));
   plotLongNames_[name]=longName;
   plotShortNames_[longName]=name;
   return dynamic_cast<TH1F *> (outObjects_[longName]) ;


}

//for TH2F
TH2F * plotterTools::addPlot(TString name,int nBinsX, float xMin, float xMax, int nBinsY, float yMin, float yMax, TString xTitle, TString yTitle,TString type, TString group, TString module){

  initVariable(name);

   TString longName=group+TString("_")+module+TString("_")+type+TString("_")+name;
   outObjects_[longName]=((TObject*) bookHisto2D(name,nBinsX, xMin, xMax,nBinsY,yMin, yMax,xTitle,yTitle, type, group_,module_));
   plotLongNames_[name]=longName;
   plotShortNames_[longName]=name;
   return dynamic_cast<TH2F *> (outObjects_[longName]) ;

}


//TH1F
TH1F* plotterTools::bookHisto(TString name,int nBinsX,float xMin, float xMax, TString type, TString group, TString module){
  TString longName=group+TString("_")+module+TString("_")+type+TString("_")+name;
  TH1F* histo = new TH1F (name, longName , nBinsX, xMin, xMax);
  histo->SetTitle(name);
  histo->SetName(longName);

  return histo;

}

//TH2F
TH2F* plotterTools::bookHisto2D(TString name,int nBinsX,float xMin, float xMax,int nBinsY, float yMin, float yMax,TString xTitle, TString yTitle, TString type, TString group, TString module){
  TString longName=group+TString("_")+module+TString("_")+type+TString("_")+name;
  TH2F* histo = new TH2F (name, longName , nBinsX, xMin, xMax, nBinsY, yMin, yMax);
  histo->SetTitle(name);
  histo->SetName(longName);
  histo->SetXTitle(xTitle);
  histo->SetYTitle(yTitle);
  return histo;

}

//combined plots
TH2F* plotterTools::bookHistoCombined(TString name,TString name1, TString name2){

  int  nBinsX=((TH1F* )outObjects_[plotLongNames_[name1]])->GetNbinsX();
  float xMin=((TH1F* )outObjects_[plotLongNames_[name1]])->GetXaxis()->GetBinLowEdge(1);
  float xMax=((TH1F* )outObjects_[plotLongNames_[name1]])->GetXaxis()->GetBinLowEdge(nBinsX)+((TH1F* )outObjects_[plotLongNames_[name1]])->GetXaxis()->GetBinWidth(nBinsX);
                                        
  int nBinsY=((TH1F* )outObjects_[plotLongNames_[name2]])->GetNbinsX();
  float yMin=((TH1F* )outObjects_[plotLongNames_[name2]])->GetXaxis()->GetBinLowEdge(1);
  float yMax=((TH1F* )outObjects_[plotLongNames_[name2]])->GetXaxis()->GetBinLowEdge(nBinsY)+((TH1F* )outObjects_[plotLongNames_[name2]])->GetXaxis()->GetBinWidth(nBinsY);

  TH2F* histo = new TH2F (name, plotLongNames_[name] ,nBinsX, xMin,xMax, nBinsY, yMin, yMax);
  for(int i =1;i<=nBinsX;i++){
    for(int j =1;j<=nBinsY;j++){
      float X=((TH1F* )outObjects_[plotLongNames_[name1]])->GetBinContent(i);
      float Y=((TH1F* )outObjects_[plotLongNames_[name2]])->GetBinContent(j);
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


void plotterTools::saveHistos(){
  std::cout << "==================== Saving histograms =======================" << std::endl;
  std::cout << "outputFile " << outputFile_->GetName() << " opened" << std::endl;
  outputFile_->cd();
  for (std::map<TString,TObject*>::const_iterator out=outObjects_.begin();out!=outObjects_.end();++out)
    out->second->Write(out->first);
  outputFile_->Close();
  std::cout << "outputFile " << outputFile_->GetName() << " closed" << std::endl;
  std::cout << "==================== DQM analysis is done =======================" << std::endl;
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
  std::cout << "==================== Plotting histograms =======================" << std::endl;
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



  for (std::map<TString,TObject*>::const_iterator out=outObjects_.begin();out!=outObjects_.end();++out){
    if(out->first.Contains("history"))  {
      setAxisTitles((TGraph*)out->second,"Event",plotShortNames_[out->first]);
      plotMe((TGraph*)out->second, out->first);
    }else if(out->first.Contains("1D"))  {
      setAxisTitles((TH1F*)out->second,plotShortNames_[out->first],"Events");
      plotMe((TH1F*)out->second);
    }else if(out->first.Contains("2D"))  {
      setAxisTitles((TH2F*)out->second,((TAxis*)((TH2F*)out->second)->GetXaxis())->GetTitle(),((TAxis*)((TH2F*)out->second)->GetYaxis())->GetTitle());
      plotMe((TH2F*)out->second);
    }
  std::cout << "==================== Canvas saved in \" "<< outputDir_<<"\" =======================" << std::endl;
}
}

void plotterTools::printHistos(){
  std::cout << "==================== Booked histograms =======================" << std::endl;
  for (std::map<TString,TObject*>::const_iterator out=outObjects_.begin();out!=outObjects_.end();++out)
    std::cout << out->second->GetName() << std::endl;
  std::cout << "==================== Loop over events =======================" << std::endl;
}

void::plotterTools::fillFiberOrder(){
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

}

int plotterTools::findPosition(std::vector<int>* fiberVec, int n){

  std::vector<int>::iterator iter=  std::find(fiberVec->begin(), fiberVec->end(),n);

  if(iter==fiberVec->end()){
    return -1;
  }else{
    return iter-fiberVec->begin();
  }

}
