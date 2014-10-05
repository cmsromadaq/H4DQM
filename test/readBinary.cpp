#include <fstream>
#include <iostream>
#include <vector>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "interface/SpillUnpack.hpp"

void cannotOpenFile(char * file)
{
  fprintf (stderr,
	   "File %s cannot be opened\n",
	   file);
  exit(-1);

}

int main(int argc, char *argv[])
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
	if (optopt == 'f' || optopt == 'o')
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

  ifstream *rawFile = new ifstream(filename, std::ios::in | std::ios::binary);
  
  if (!rawFile->is_open())
    cannotOpenFile(filename);

  std::streampos size;
  rawFile->seekg(0,std::ios::end);
  size=rawFile->tellg();

  fprintf(stdout,
	  "File %s opened with size %d\n",
	  filename,(int)size
	  );

  rawFile->seekg(0,std::ios::beg);

  TFile *outFile = TFile::Open(outfname,"RECREATE");  
  if (!outFile->IsOpen())
    cannotOpenFile(outfname);

  SpillUnpack *unpacker = new SpillUnpack (rawFile,outFile) ;

  unpacker->Unpack (1) ;

  outFile->Close();
  std::cout << outfname << " is closed." << std::endl;

  rawFile->close();
  std::cout << filename << " is closed." << std::endl;
}
