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
  BoardTypes_t boardTypes;
  for (int i=0;i<(int)_MAXBOARDTYPE_;++i){
    switch(i){
    case _TIME_:
      boards_[i] = new TIME;
      break;
    case _CAENVX718_:
      boards_[i]= new CAENVX718;
      break;
    case _CAENVX1742_:
      boards_[i]= new VX1742;
      break;
    case _CAENVX513_:
      boards_[i]= new VX513;
      break;
    case _CAENVX262_:
      boards_[i]= new VX262;
      break;
    case _CAENVX792_:
      boards_[i]= new VX792;
      break;
    case _CAENVX1290_:
      boards_[i]= new VX1290;
      break;
    case _CAENVX1495PU_:
      boards_[i]= new VX1495PU;
      break;
    case _CAENVX560_:
      boards_[i]= new VX560;
      break;
    case _UNKWN_:
      //TO DO decide what to do.continue?
      break;


    }
  }


}

