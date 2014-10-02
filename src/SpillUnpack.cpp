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
  for (int c=0;c<(BoardTypes_t)_MAXBOARDTYPE_;++c){

    BoardTypes_t i = static_cast<BoardTypes_t>( c );

    switch(i){
    case _TIME_:
      boards_[i] = new TIME;
      break;
    case _CAENVX718_:
      boards_[i]= new CAEN_VX718;
      break;
    case _CAENV1742_:
      boards_[i]= new CAEN_V1742;
      break;
    case _CAENV513_:
      boards_[i]= new CAEN_V513;
      break;
    case _CAENV262_:
      boards_[i]= new CAEN_V262;
      break;
    case _CAENV792_:
      boards_[i]= new CAEN_V792;
      break;
    case _CAENV1290_:
      boards_[i]= new CAEN_V1290;
      break;
    case _CAENV1495PU_:
      boards_[i]= new CAEN_V1495PU;
      break;
    case _CAENV560_:
      boards_[i]= new CAEN_V560;
      break;
    case _UNKWN_:
      //TO DO decide what to do.continue?
      break;


    }
  }


}

