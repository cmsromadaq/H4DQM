#include <plotterTools.hpp>

int main (int argc, char ** argv) 
{

  gROOT->ProcessLine("#include <vector>");

  string ROOT_FOLDER ="";
  string OUT_FOLDER  ="";
  string run="";
  string spill="";
  string integratedfname ="";

  int c;
  while ((c = getopt (argc, argv, "i:o:r:s:I:")) != -1)
    switch (c)
      {
      case 'i':
	ROOT_FOLDER = string(optarg);
	break;
     case 'o':
       OUT_FOLDER =  string(optarg);
	break;
      case 'r':
	run =  string(optarg);
	break;
      case 's':
	spill =  string(optarg);
	break;
      case 'I':
	integratedfname= string(optarg);
	break;

      case '?':
	if (optopt == 'i'|| optopt == 'o'|| optopt == 'r'|| optopt == 's'||optopt == 'I')
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

  string filename=ROOT_FOLDER+"/"+run+"/"+spill+".root";
  string outdname=OUT_FOLDER+"/"+run+"/"+spill+"/DAQ/";
  string outfname=outdname+"dqmPlots"+"DAQStatus"+".root";
  string integratedName=OUT_FOLDER+"/"+run+"/"+integratedfname;

  system( Form("mkdir -p %s", outdname.c_str()) );


  std::cout<<filename<<" "<<outdname<<" "<<outfname<<" "<<integratedfname<<endl;



  plotterTools plotter(filename,outfname,outdname);

  plotter.setPlotsFormat () ;
  plotter.readInputTree();  
  int nentries = plotter.getTreeEntries();
  plotter.setStepHistoryPlots(20);
  plotter.setGroup("DAQStatus");
  plotter.setModule("beam");
  plotter.bookPlotsDAQStatus(nentries/plotter.getStepHistoryPlots());
  plotter.printHistos();

  plotter.Loop();

  //  plotter.bookCombinedPlots();

  if(integratedfname != "")  plotter.initIntegrated(integratedName); 

  plotter.fitHisto("deltaTime10","gaus");
  plotter.fitHisto("deltaTime20","gaus");
  plotter.fitHisto("deltaTime21","gaus");

  plotter.setPlotAxisRange("fractionTakenTrig","Y",0,1.1);

  plotter.plotHistos();
  plotter.saveHistos();
 

  return 0 ;


}
