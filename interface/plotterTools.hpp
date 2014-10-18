#ifndef PLOT_TOOLS
#define PLOT_TOOLS

#include <TStyle.h>
#include <TROOT.h>
#include <TColor.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TGraph.h>
#include <TRandom.h>
#include <TChain.h>
#include <TString.h>
#include <TLegend.h>

#include <Event.hpp>

#define nPlanesHodo 4
#define nFibersHodo 64
//check if it is true this schema
#define hodoX1 0
#define hodoY1 1
#define hodoX2 2
#define hodoY2 3

#define hodoSmallX 0
#define hodoSmallY 1

#define nPlanesSmallHodo 2
#define nFibersSmallHodo 8


class plotterTools{

public:

  plotterTools (TString filename, TString outfname, TString outdname);



  TChain* inputTree_;
  TFile* inputFile_;
  TFile* outputFile_;
  TFile* integratedFile_;
  TString outputDir_;

  TString module_, group_;
  int historyStep_;//set the step of events for history plots
  bool isIntegratedNew;
  std::map<TString,TH1F*> integratedPlots_;

  treeStructData treeStruct_;

  ULong64_t timeStart_[MAX_RO];
  ULong64_t timeEnd_[MAX_RO];

  std::vector<int> fiberOrderA;
  std::vector<int> fiberOrderB;
  std::map<TString,int> makeProfile_;
  std::map<TString,TObject*> outObjects_;
  std::map<TString,TString> plotShortNames_;
  std::map<TString,TString> plotLongNames_;
  std::vector<float> variables_;//<value,dimension>
  std::map<TString,float*> variablesMap_;
  std::map<TString,int> variablesIterator_;
  std::map<TString,std::vector<float> > variablesContainer_;
  std::map<TString,TString > variablesContainerTitles_;


  //fibers
  bool fibersOn_[nPlanesHodo][nFibersHodo];
  bool fibersOnSmall_[nPlanesSmallHodo][nFibersSmallHodo];

  //tdc readings
  static const UInt_t MaxTdcChannels = 4;
  static const UInt_t MaxTdcReadings = 20;
  std::vector<float> tdc_readings[MaxTdcChannels];

  //adc channel names
  std::map<TString,UInt_t*> adc_channelnames;
  void initAdcChannelNames(int nBinsHistory);

  void bookPlotsADC();
  bool wantADCplots;

  void fillObjects();
  void fillHodo();
  void initTdc();
  void fillFiberOrder();
  void set_plot_blue ();
  void set_palette_fancy ();
  void setPlotsFormat ();
  void plotMe (TH1F * histo);
  void plotMe (TH2F * histo, int makeProfile = 0);
  void plotMe (TGraph * graph, const TString & name);
  void setAxisTitles (TH1 * histo, const TString  xTitle, const TString  yTitle);
  void setAxisTitles (TH2 * histo, const TString  xTitle, const TString  yTitle);
  void setAxisTitles (TGraph * graph, const TString  xTitle, const TString  yTitle);
  void setPlotAxisRange(TString name, TString axis,float min, float max);
  void readInputTree ();
  void Loop();
  void saveHistos();
  void plotHistos();
  void printHistos();
  void bookPlotsScaler (int nBinsHistory);
  void bookPlotsHodo (int nBinsHistory);
  void bookPlotsSmallHodo (int nBinsHistory);
  void bookPlotsDAQStatus (int nBinsHistory);
  void bookPlotsTDC(int nBinsHistory);
  void bookCombinedPlots ();
  void bookCombinedPlotsHodo ();
  TGraph* bookGraph (TString name, int nPoints, TString type, TString group, TString module);
  void setGroup(TString group);
  void setModule(TString module);
  void setStepHistoryPlots(int n);
  int getTreeEntries();
  int getStepHistoryPlots();
  void FillPlot(TString name, int point, float X);//TGraph
  void FillPlot(TString name, bool is2D=false,int varDim=1);//TH1F
  TGraph * addPlot(TString name,int nPoints,TString type, TString group, TString module);//TGraph
  TH1F * addPlot(TString name,int nBinsX, float xMin, float xMax, TString type, TString group, TString module, int varDim=1);//TH1F
  TH2F * addPlot (TString name,int nBinsX, float xMin, float xMax, int nBinsY, float yMin, float yMax, 
                  TString xTitle, TString yTitle, TString type, TString group, TString module, int addProfile = 0);//TH2F
  void addPlotCombined(TString name, TString name1, TString name2,TString type, TString group , TString module);
  TH1F* bookHisto(TString name,int nBinsX,float xMin, float xMax, TString type, TString group, TString module);
  TH2F* bookHisto2D(TString name,int nBinsX,float xMin, float xMax,int nBinsY, float yMin, float yMax,TString xTitle, TString yTitle, TString type, TString group, TString module);
  TH2F* bookHistoCombined(TString name,TString name1, TString name2);
  void initVariable(TString name, int varDim=1);
  void computeVariable(TString name, int varDim=1);
  pair<int, string> execute (const string & command);
  void fitHisto(TString name, TString func);
  float getMinimumP (TProfile * p) ;
  float getMaximumP (TProfile * p) ;

  //fibers
  int findPosition(std::vector<int>* fiberVec, int n);

  //integratedfile
  void initIntegrated(TString nameFile);
};

#endif
