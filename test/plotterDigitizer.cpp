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
#include <map>
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
  if (outCode.first != 0) outCode = execute ("mkdir -p " + folderName) ;
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



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int main (int argc, char ** argv) 
{
  // read the info from command line
  // ---- ---- ---- ---- ---- ---- ---- ----
  
  string PLOTS_FOLDER = "/data2/govoni/data/plots/" ;
  string DIGI_FOLDER = "/data2/govoni/data/digi/" ;
  string run;
  string spill;
  unsigned numReduction=1;

  static struct option long_options[] = {
    {"plotsfolder", required_argument, 0,  'o' },
    {"digifolder",  required_argument, 0,  'i' },
    {"run",         required_argument, 0,  'r' },
    {"spill",       required_argument, 0,  's' },
    {"numberReduction",       optional_argument, 0,  'n' },
    {0,             0,                 0,  0   }
  };
 
 int long_index =0;
 int opt;

 while ((opt = getopt_long(argc, argv,"i:o:r:s:n:", 
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
   case 'n' : numReduction=atoi(optarg);
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
  plotter.readInputTree();  
  int nentries = plotter.getTreeEntries();
  plotter.setStepHistoryPlots(20);
  plotter.setGroup ("digitizer") ;
  plotter.setModule("beam");
  plotter.bookPlotsDigitizer();
  plotter.printHistos();

  plotter.Loop();

  plotter.plotHistos();
  plotter.saveHistos();
 



  return 0 ;
}
