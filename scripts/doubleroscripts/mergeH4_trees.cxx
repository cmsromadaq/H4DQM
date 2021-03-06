#include <string.h>
#include <sstream>
#include "TChain.h"
#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TKey.h"
#include "Riostream.h"
#include "MergeRootFiles.cxx"
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>     /* atoi */

int main(int argc, char** argv)
{
  
  TList *FileList;
  TFile *Target;

	int iRun = atoi( argv[1] );
	std::cout << " iRun = " << iRun << std::endl;
	
	int NFILES = 300;   

	char fileNameTarget[150];
	sprintf(fileNameTarget, "/tmp/run%d-H4-DRO.root", iRun);
   	Target = TFile::Open(fileNameTarget, "RECREATE" );
	
	char fileName[50];
	

   	FileList = new TList();
		
	for (int iFile = 0; iFile < NFILES; iFile++)
	{
	  sprintf(fileName, "/data/DQM/%d/%d.root", iRun, iFile+1);
	  std::cout << " fileName = " << fileName << std::endl;
	  
	  stringstream ss;
	  string s;
	  
	  ss << fileName;
	  ss >> s;
	  ifstream stream(s.c_str());
	  
	  if (stream)
	  {
	    //TFile file(fileName, "READ");
	    FileList->Add( TFile::Open(fileName));
// 	    NFILES_eff++;
// 	    cout << " NFILES_eff = " << NFILES_eff << endl;
	  }
     
	  else
	  {
// 	    std::cout << "file: " << fileName << " doesn't exist " << std::endl;
	    break;	    
	  }
	}

//	std::cout << "Writing root file: " << fileNameTarget << std::endl;
	MergeRootfile( Target, FileList );
	
	return 0;

}



