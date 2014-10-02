#include "interface/SpillUnpack.hpp"

WORD spillHeaderValue;
WORD spillTrailerValue;
WORD eventHeaderValue;
WORD eventTrailerValue;
WORD boardHeaderValue;
WORD boardTrailerValue;

SpillUnpack::SpillUnpack( std::ifstream* in, TFile* out ) {
  rawFile=in;
  outFile=out;

  spillHeaderValue = *((uint32_t*)"SPLH");
  spillTrailerValue = *((uint32_t*)"SPLT");
  eventHeaderValue = *((uint32_t*)"EVTH");
  eventTrailerValue = *((uint32_t*)"EVNT");
  boardHeaderValue = *((uint32_t*)"BRDH");
  boardTrailerValue = *((uint32_t*)"BRDT");
}

int SpillUnpack::Unpack(){

  WORD word;

  while (!rawFile->eof()) {
      spillHeader spillH;

      rawFile->read ((char*)&word, WORDSIZE);
      
      cout << " read " << word << endl;
      
      if (word==spillHeaderValue) {
        rawFile->read ((char*)&spillH.runNumber, WORDSIZE);
        rawFile->read ((char*)&spillH.spillNumber, WORDSIZE);
        rawFile->read ((char*)&spillH.spillSize, WORDSIZE);
        rawFile->read ((char*)&spillH.nEvents, WORDSIZE);
      
        if (DEBUG_UNPACKER) {
          cout <<  " ======= BEGIN SPILL ======= " << endl;
          cout << "Spill " << spillH.spillNumber << endl;
          cout << "Events in spill " << spillH.nEvents << endl;
        }
        
        UnpackEvents(spillH.nEvents);
      } 
      else 
      { 
        cout << " ERROR corrupt RAW file. Expecting spill header, read " << word << endl;
        return(0);
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

void SpillUnpack::UnpackEvents(int nevents) {

  WORD word;
  WORD nevt=0;

  while (!rawFile->eof()) {
    eventHeader eventH;

    rawFile->read ((char*)&word, WORDSIZE);
    if (word==eventHeaderValue) {
      nevt++;
      rawFile->read ((char*)&eventH.eventNumber, WORDSIZE);
      rawFile->read ((char*)&eventH.eventSize, WORDSIZE);
      rawFile->read ((char*)&eventH.nBoards, WORDSIZE);

      if (DEBUG_UNPACKER) {
        cout << " ======== EVENT START ======= " << std::endl;
        cout << "  Event " << eventH.eventNumber << std::endl;
        cout << "  Boards in event " << eventH.nBoards << std::endl;
      }
      
      if (eventH.eventNumber!=nevt) {
        cout << " ERROR event numbering inconsistent! " << endl;
      }
      
      UnpackBoards(eventH.nBoards);
      continue;
    } 
      
    if (word==eventTrailerValue) {
      if (DEBUG_UNPACKER) std::cout << " ========= EVENT END ======== " << std::endl;
      continue;
    } 
    
    if (word==spillTrailerValue) {
      if (DEBUG_UNPACKER) std::cout << " ========= SPILL END ======== " << std::endl;
      if (DEBUG_UNPACKER || nevents!=nevt) cout << "Read " << nevt << " events. Expected " << nevents << endl;
      break;
    }

    cout << " ERROR corrupt RAW file. Read " << word << endl;

  }    
}

void SpillUnpack::UnpackBoards(int nboards) {

  WORD word;
  WORD nbrd=0;

  while (!rawFile->eof()) {
    boardHeader bH;

    rawFile->read ((char*)&word, WORDSIZE);
    if (word==boardHeaderValue) {
      nbrd++;
      rawFile->read ((char*)&bH.boardID, WORDSIZE);
      rawFile->read ((char*)&bH.boardSize, WORDSIZE);

      if (DEBUG_UNPACKER) {
        cout << " ======== BOARD START ======= " << std::endl;
        cout << "  Board " << nbrd << "  Type " << bH.boardID << endl;
      }
      continue;
    } 

    if (word==eventTrailerValue) {
      if (DEBUG_UNPACKER) std::cout << " ========= EVENT END ======== " << std::endl;
      if (DEBUG_UNPACKER || nboards!=nbrd) cout << " Read " << nbrd << " boards. Expected " << nboards << endl;
      break;
    }

    if (word==boardTrailerValue) {
      if (DEBUG_UNPACKER) std::cout << " ========= BOARD END ======== " << std::endl;
      continue;
    } 
          
    cout << "Read " << word << endl;
  }    
}
