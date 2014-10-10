#include <plotterTools.hpp>

int main (int argc, char ** argv) 
{
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

  plotterTools plotter(filename,outfname,outdname);

  plotter.setPlotsFormat () ;
  plotter.readInputTree();  
  int nentries = plotter.getTreeEntries();
  plotter.setStepHistoryPlots(20);
  plotter.setGroup("scaler");
  plotter.setModule("beam");
  plotter.bookPlotsScaler(nentries/plotter.getStepHistoryPlots());
  plotter.printHistos();

  plotter.Loop();

  plotter.plotHistos();
  plotter.saveHistos();
 

  return 0 ;


}
