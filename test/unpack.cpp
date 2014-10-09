#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <map>


#include "interface/SpillUnpack.hpp"

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
//        result += tempo.substr (0, tempo.size () - 1) ;
        result += tempo ;
      }
    int returnCode = pclose (in) ;
 
    return pair<int, string> (returnCode, result) ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void cannotOpenFile (const string & file)
{
  cerr << "[UNPACKER] ERROR File " << file << " cannot be opened\n" ;
  exit (-1) ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int main(int argc, char *argv[])
{
  if (argc < 3)
    {
      cerr << "[UNPACKER] ERROR usage: unpack runNumber spillNumber\n " ;
      exit (-1) ;
    }

  const string RAW_FOLDER = "/data2/govoni/data/raw/" ;
  const string DIGI_FOLDER = "/data2/govoni/data/digi/" ;

  stringstream filename ;
  filename << RAW_FOLDER << "/" << argv[1] << "/" << argv[2] << ".raw" ;
  stringstream outfname ;
  outfname << DIGI_FOLDER << "/" << argv[1] << "/" << argv[2] << ".root" ;
  
  pair <int, string> outCode = execute ("ls " + DIGI_FOLDER + "/" + argv[1]) ;
  if (outCode.first != 0) outCode = execute ("mkdir " + DIGI_FOLDER + "/" + argv[1]) ;
  if (outCode.first != 0) 
    {
      cerr << "[UNPACKER] ERROR RUN " << argv[1] << ", SPILL " << argv[2]
           << ", problems creating the output folder:\n"
           << outCode.second << "\n"
           << "exiting\n" ;
      exit (1) ;
    }

  ifstream * rawFile = new ifstream (filename.str ().c_str (), std::ios::in | std::ios::binary) ;
  
  if (!rawFile->is_open ()) cannotOpenFile (filename.str ()) ;

  std::streampos size ;
  rawFile->seekg (0, std::ios::end) ;
  size = rawFile->tellg () ;
  cout << "[UNPACKER] File " << filename.str () << " opened with size " << size << "\n" ;

  rawFile->seekg(0,std::ios::beg);

  TFile * outFile = TFile::Open (outfname.str ().c_str (), "RECREATE") ;  
  if (!outFile->IsOpen ()) cannotOpenFile (outfname.str ()) ;

  TTree * outTree = new TTree ("H4tree", "H4 testbeam tree") ;

  SpillUnpack *unpacker = new SpillUnpack (rawFile,outFile, outTree) ;

  unpacker->Unpack (-1) ;

  outFile->ls () ;
  outFile->cd () ;
  outTree->Write () ;
  outFile->Close () ;
  cout << "[UNPACKER] " << outfname << " is closed." << endl;

  rawFile->close () ;
  cout << "[UNPACKER] " <<  filename << " is closed." << endl;
}
