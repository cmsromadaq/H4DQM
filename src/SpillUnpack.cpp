#include "interface/SpillUnpack.hpp"

WORD spillHeaderValue;
WORD spillTrailerValue;

SpillUnpack::SpillUnpack( std::ifstream* in, TFile* out ) {
  rawFile=in;
  outFile=out;

  spillHeaderValue = *((uint32_t*)"SPLH");
  spillTrailerValue = *((uint32_t*)"SPLT");
}

int SpillUnpack::Unpack(){

  WORD word;

  while (!rawFile->eof()) {
      spillHeader spillH;

      rawFile->read ((char*)&spillH.header, WORDSIZE);
      if (spillH.header==spillTrailerValue) {
        if (DEBUG_UNPACKER) std::cout << " ========= SPILL END ======== " << std::endl;
        break;
      }
      if (spillH.header!=spillHeaderValue) {
        std::cout << "   ERROR spill header incorrect. Read: " << spillH.header << std::endl;
        continue;
      }
      
      if (DEBUG_UNPACKER) std::cout <<  " ======= BEGIN SPILL ======= " << std::endl;
      rawFile->read ((char*)&spillH.runNumber, WORDSIZE);
      rawFile->read ((char*)&spillH.spillNumber, WORDSIZE);
      rawFile->read ((char*)&spillH.nEvents, WORDSIZE);
      
      if (DEBUG_UNPACKER) {
        std::cout << "Spill " << spillH.spillNumber << std::endl;
        std::cout << "Events in spill " << spillH.nEvents << std::endl;
      }
  }

}

void SpillUnpack::InitBoards(){
  //init differend kind of boards
  for (int i=0;i<nBoardTypes_;++i){
    switch(i){
    case 0:
//      boards_[i] = new ;//TODO init different kind of boards. each one is a class inheriting from Board
      break;
    case 1:
//      boards_[i]= new ;
      break;
    }
  }

}

