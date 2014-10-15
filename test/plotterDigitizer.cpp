//#include <libxml/parser.h>
//#include <libxml/tree.h>

#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TTree.h>
#include <TChain.h>
#include <TBranch.h>

#include <set>

#include <plotterTools.hpp>

using namespace std ;

// get the histos ranges from a cfg file, written in XML


// what are the channels which are turned on? i.e. how can I know it?

void addToPersistency (TH2F * pPlot, TChain * event, 
                       int digiGroup, int digiChannel)
{
  TString command = "digiSampleValue:digiSampleIndex>>" ;
  command += pPlot->GetName () ;
  TString selection = "digiGroup==" ;
  selection += digiGroup ;
  selection += "digiChannel=" ;
  selection += digiChannel ;
  event->Draw (command, selection) ;
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


set<int>
listElements (UInt_t * array, int max)
{
  set<int> elements ;
  for (int i = 0 ; i < max ; ++i)
    {
      if (elements.find (array[i]) != elements.end ()) continue ;
      elements.insert (array[i]) ;
    }
  return elements ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int main (int argc, char ** argv) 
{
  // read the info from command line
  // ---- ---- ---- ---- ---- ---- ---- ----
  
  char *filename = NULL;
  char *outfname = NULL;
  char *outdname = NULL;
  int c;

  while ((c = getopt (argc, argv, "f:d:o:")) != -1)
    switch (c)
      {
          case 'f':
            filename = optarg;
            break;
         case 'o':
            outfname = optarg;
            break;
          case 'd':
            outdname = optarg;
            break;
          case '?':
            if (optopt == 'f'|| optopt == 'o'|| optopt == 'd')
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
  // open the digi file
  // ---- ---- ---- ---- ---- ---- ---- ----

  plotterTools plotter (filename,outfname,outdname) ;
  plotter.setPlotsFormat () ;
  plotter.setModule ("pulses") ;
  plotter.setGroup ("digitizer") ;
  
  // get the number of channels and groups used from the digi file
  // ---- ---- ---- ---- ---- ---- ---- ----
  UInt_t          digiGroup[9216] ;
  TBranch        * b_digiGroup ;
  plotter.inputTree_->SetBranchAddress ("digiGroup", digiGroup, &b_digiGroup) ;
  UInt_t          digiChannel[9216] ;
  TBranch        * b_digiChannel ;
  plotter.inputTree_->SetBranchAddress ("digiChannel", digiChannel, &b_digiChannel) ;
  UInt_t          nDigiSamples ;
  TBranch        *b_nDigiSamples ;
  plotter.inputTree_->SetBranchAddress("nDigiSamples", &nDigiSamples, &b_nDigiSamples) ;

  plotter.inputTree_->GetEvent (0) ;
  set<int> channels = listElements (digiChannel,  nDigiSamples) ;
  set<int> groups = listElements (digiGroup,  nDigiSamples) ;

  // read the expected ranges for histos from xml file, or from the digi info?
  // ---- ---- ---- ---- ---- ---- ---- ----
  
  int xNbins = 100 ;
  float xmin = 0 ;
  float xmax = 100. ;
      
  int yNbins = 100 ;
  float ymin = 0 ;
  float ymax = 100. ;

  // prepare and fill the plots
  // ---- ---- ---- ---- ---- ---- ---- ----
  
  vector<TH2F *> histos ;
  for (set<int>::iterator iGroup = groups.begin () ; 
       iGroup != groups.end () ; ++iGroup)
    {
      for (set<int>::iterator iChannel = channels.begin () ; 
           iChannel != channels.end () ; ++iChannel)
        {
          TString name = "digiPersPlot_gr" ;
          name += *iGroup ;
          name += "_ch" ;
          name += *iChannel ;
//          TH2F * dummy = new TH2F (name, name, xNbins, xmin, xmax, yNbins, ymin, ymax) ;
          TH2F * dummy = plotter.addPlot (name, xNbins, xmin, xmax, yNbins, ymin, ymax, 
                           "time", "voltage",
                           "2D", plotter.group_, plotter.module_) ;
                           
          addToPersistency (dummy, plotter.inputTree_, *iGroup, *iChannel) ;
          plotter.plotMe (dummy) ;
          histos.push_back (dummy) ;
        }
    }
  plotter.plotHistos () ;
  plotter.saveHistos () ;  
  return 0 ;
}