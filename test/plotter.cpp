#include <plotterTools.hpp>

int main (int argc, char ** argv) 
{
  char *filename = NULL;
  char *outfname = NULL;
  int c;

  while ((c = getopt (argc, argv, "f:o:")) != -1)
    switch (c)
      {
      case 'f':
	filename = optarg;
	break;
     case 'o':
	outfname = optarg;
	break;


      case '?':
	if (optopt == 'f'|| optopt == 'o')
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

  plotterTools plotter(filename,outfname);
  plotter.setPlotsFormat () ;
  

  plotter.Loop();



  TH1F * testH1 = new TH1F ("testH1", "test histogram", 10, 0., 10.) ;
  plotter.setAxisTitles (testH1, "x axis", "y axis") ;
  testH1->Fill (2) ;
  testH1->Fill (3) ;
  plotter.plotMe (testH1) ;
  
  TH2F * testH2  = new TH2F ("testH2", "py vs px", 40, -4, 4, 40, -4, 4) ;
  plotter.setAxisTitles (testH2, "x axis", "y axis") ;
  Float_t px, py ;
  for (Int_t i = 0; i < 25000; ++i) 
    {
      gRandom->Rannor (px,py) ;
      testH2->Fill (px,py) ;
    }  
  plotter.plotMe (testH2) ;  

  Double_t x[100], y[100] ;
  Int_t n = 20 ;
  for (Int_t i = 0 ; i < n ; ++i) 
    {
      x[i] = i * 0.1 ;
      y[i] = 10 * sin (x[i] + 0.2) ;
    }
  TGraph * testG = new TGraph (n,x,y) ;
  plotter.setAxisTitles (testG->GetHistogram (), "x axis", "y axis") ;
  plotter.plotMe (testG, "testG") ;  
  
  return 0 ;


}
