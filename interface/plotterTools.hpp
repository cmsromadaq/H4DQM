#ifndef PLOT_TOOLS
#define PLOT_TOOLS

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

class plotterTools{

public:

  plotterTools(char* filename, char*outfname, char*outdname);


  TChain* inputTree_;
  TFile* inputFile_;
  TFile* outputFile_;
  TString outputDir_;

  std::map<TString,TObject*> outObjects_;

  void set_plot_blue ();
  void setPlotsFormat ();
  void plotMe (TH1F * histo);
  void plotMe (TH2F * histo);
  void plotMe (TGraph * graph, const TString & name);
  void setAxisTitles (TH1 * histo, const TString & xTitle, const TString & yTitle);
  void setAxisTitles (TH2 * histo, const TString & xTitle, const TString & yTitle);
  void setAxisTitles (TGraph * graph, const TString & xTitle, const TString & yTitle);
  void readInputTree (treeStructData& treeData);
  void Loop();
  void saveHistos();
  void printHistos();
  void bookGraphs (std::map<TString,TGraph*> &graphs, int nBinsHistory);
  //TGraph* bookGraph(TSTring name, int nPoints,  TString module, TString type);
  TGraph* bookGraph (TString name, int nPoints,  TString module="beam", TString type="history");


};

#endif
