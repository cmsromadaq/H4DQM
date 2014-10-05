#include <fstream>
#include <iostream>
#include <vector>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include <TFile.h>

#include "interface/CAEN_V1742.hpp"

using namespace std ;

void cannotOpenFile (char * file)
{
  fprintf (stderr,
	   "File %s cannot be opened\n",
	   file) ;
  exit (-1) ;

}

int main (int argc, char *argv[])
{
  CAEN_V1742 * CAEN_V1742_unpacker = new CAEN_V1742 () ;
  delete CAEN_V1742_unpacker ;
}
