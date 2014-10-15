/* usage example

./bin/plotterDigitizer -f /data2/govoni/data/digi/99/119.root -d /data2/govoni/data/plots -o plots_119.root
 
*/

//#include <libxml/parser.h>
//#include <libxml/tree.h>

#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TTree.h>
#include <TChain.h>
#include <TBranch.h>

#include <set>
#include <getopt.h>

#include <plotterTools.hpp>

using namespace std ;


pair<int, string>
execute (const string & command) 
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
        result += tempo ;
      }
    int returnCode = pclose (in) ;
 
    return pair<int, string> (returnCode, result) ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void checkFolder (string folderName)
{
  pair <int, string> outCode = execute ("ls " + folderName) ;
  if (outCode.first != 0) outCode = execute ("mkdir " + folderName) ;
  if (outCode.first != 0) 
    {
      cerr << "[plotDigitizer] ERROR cannot create " << folderName << ":\n"
           << outCode.second << "\n"
           << "exiting\n" ;
      exit (1) ;
    }
  return ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void addToPersistency (TH2F * pPlot, TChain * event, 
                       int digiGroup, int digiChannel)
{
  TString command = "digiSampleValue:digiSampleIndex>>" ;
  command += pPlot->GetName () ;
  TString selection = "digiGroup==" ;
  selection += digiGroup ;
  selection += "&&digiChannel==" ;
  selection += digiChannel ;
  event->Draw (command, selection) ;
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


template <class T>
pair<float, float>
getRange (T * array, int Nmax)
{
  float min = array[0] ;
  float max = array[0] ;
  for (int i = 0 ; i < Nmax ; ++i)
    {
      if (array[i] > max) max = array[i] ;
      if (array[i] < min) min = array[i] ;
    }
  return pair<float, float> (min, max) ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


template <class T>
set<int>
listElements (T * array, int Nmax)
{
  set<int> elements ;
  for (int i = 0 ; i < Nmax ; ++i)
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
  
  string PLOTS_FOLDER = "/data2/govoni/data/plots/" ;
  string DIGI_FOLDER = "/data2/govoni/data/digi/" ;
  string run;
  string spill;

  static struct option long_options[] = {
    {"plotsfolder", required_argument, 0,  'o' },
    {"digifolder",  required_argument, 0,  'i' },
    {"run",         required_argument, 0,  'r' },
    {"spill",       required_argument, 0,  's' },
    {0,             0,                 0,  0   }
  };
 
 int long_index =0;
 int opt;

 while ((opt = getopt_long(argc, argv,"i:o:r:s:", 
			   long_options, &long_index )) != -1) {
   switch (opt) {
   case 'i' : DIGI_FOLDER=string(optarg);
     break;
   case 'o' : PLOTS_FOLDER=string(optarg);
     break;
   case 'r' : run=string(optarg);
     break;
   case 's' : spill=string(optarg);
     break;
   case '?':
     /* getopt_long already printed an error message. */
     //print_usage(); 
     //exit(EXIT_FAILURE);
   default: 
     //print_usage(); 
     exit(1);
   } // switch
 } // while
  
  cout <<" PLOTS FOLDER IS: "<<PLOTS_FOLDER<<endl;
  cout <<" DIGI FOLDER IS: "<<DIGI_FOLDER<<endl;

  // open the digi file
  // ---- ---- ---- ---- ---- ---- ---- ----

  string filename = DIGI_FOLDER + "/" + run + "/" + spill + ".root" ;
  string outdname = PLOTS_FOLDER + "/" + run + "/" + spill + "/digitizer/" ;
  string outfname = outdname + "plots_" + spill + ".root" ;

  checkFolder (PLOTS_FOLDER + "/" + run) ;
  checkFolder (PLOTS_FOLDER + "/" + run + "/" + spill) ;
  checkFolder (PLOTS_FOLDER + "/" + run + "/" + spill + "/digitizer/") ;

  plotterTools plotter (filename.c_str (), outfname.c_str (), outdname.c_str ()) ;
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
  Float_t         digiSampleValue[9216] ;
  TBranch        *b_digiSampleValue ;
  plotter.inputTree_->SetBranchAddress("digiSampleValue", digiSampleValue, &b_digiSampleValue) ;
  UInt_t          digiSampleIndex[9216] ;
  TBranch        *b_digiSampleIndex ;
  plotter.inputTree_->SetBranchAddress("digiSampleIndex", digiSampleIndex, &b_digiSampleIndex) ;

  plotter.inputTree_->GetEvent (0) ;
  set<int> channels = listElements (digiChannel,  nDigiSamples) ;
  set<int> groups = listElements (digiGroup,  nDigiSamples) ;

  // read the expected ranges for histos from xml file, or from the digi info?
  // ---- ---- ---- ---- ---- ---- ---- ----
  // get them from the file itself, by doing the first plot.

  pair<float, float> xRange = getRange (digiSampleIndex, nDigiSamples) ;
  int xNbins = nDigiSamples ;
  float xmin = xRange.first ;
  float xmax = xRange.second ;

  pair<float, float> yRange = getRange (digiSampleValue, nDigiSamples) ;
  int yNbins = 100 ;
  float ymin = yRange.first - 0.1 * fabs (yRange.first) ;
  float ymax = yRange.second + 0.1 * fabs (yRange.second) ;

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