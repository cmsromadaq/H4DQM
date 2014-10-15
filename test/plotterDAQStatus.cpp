#include <plotterTools.hpp>

int main (int argc, char ** argv) 
{
  char *filename = NULL;
  char *outfname = NULL;
  char *outdname = NULL;
  char *integratedfname = NULL;
  int c;

  while ((c = getopt (argc, argv, "f:d:o:I:")) != -1)
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
      case 'I':
	integratedfname=optarg;
	break;

      case '?':
	if (optopt == 'f'|| optopt == 'o'|| optopt == 'd'||optopt == 'I')
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

    if(integratedfname)  plotter.initIntegrated(integratedfname); 

  plotter.plotHistos();
  plotter.saveHistos();
 

  return 0 ;


}
