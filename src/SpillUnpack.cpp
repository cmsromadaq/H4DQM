#include "interface/SpillUnpack.hpp"

WORD spillHeaderValue;
WORD spillTrailerValue;
WORD eventHeaderValue;
WORD eventTrailerValue;
WORD boardHeaderValue;
WORD boardTrailerValue;


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


SpillUnpack::SpillUnpack( ifstream* in, TFile* out ) {
  rawFile=in;
  outFile=out;

  spillHeaderValue = *((uint32_t*)"SPLH");
  spillTrailerValue = *((uint32_t*)"SPLT");
  eventHeaderValue = *((uint32_t*)"EVTH");
  eventTrailerValue = *((uint32_t*)"EVNT");
  boardHeaderValue = *((uint32_t*)"BRDH");
  boardTrailerValue = *((uint32_t*)"BRDT");
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int SpillUnpack::Unpack(int events = -1){

  WORD word;

  while (!rawFile->eof()) {
      spillHeader spillH;

      rawFile->read ((char*)&word, WORDSIZE);
      
      cout << "[SpillUnpack][Unpack]      | reading word " << word << endl;
      
      if (word==spillHeaderValue) {
        rawFile->read ((char*)&spillH.runNumber, WORDSIZE);
        rawFile->read ((char*)&spillH.spillNumber, WORDSIZE);
        rawFile->read ((char*)&spillH.spillSize, WORDSIZE);
        rawFile->read ((char*)&spillH.nEvents, WORDSIZE);
      
        if (DEBUG_UNPACKER) {
          cout << "[SpillUnpack][Unpack]      | ======= BEGIN SPILL ======= \n" ;
          cout << "[SpillUnpack][Unpack]      | Spill " << spillH.spillNumber << "\n" ;
          cout << "[SpillUnpack][Unpack]      | Events in spill " << spillH.nEvents << "\n" ;
          cout << "[SpillUnpack][Unpack]      | unpacking " << events << " events" << endl ;
        }
        
        if (-1 == events) events = spillH.nEvents ; 
        UnpackEvents (events) ;
      } 
      else 
      { 
        cout << "[SpillUnpack][Unpack]      | ERROR corrupt RAW file. "
             << "Expecting spill header, read " << word << endl ;
        return 1 ;
      }
  }

}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void SpillUnpack::InitBoards(){

  if (DEBUG_VERBOSE_UNPACKER) cout << "[SpillUnpack][InitBoards]" ;

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


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void SpillUnpack::UnpackEvents (int nevents) {

  WORD word;
  WORD nevt=0;

  // loop over the input file
  while (!rawFile->eof()) {
    
    if (nevt > nevents) break ;

    eventHeader eventH;

    rawFile->read ((char*)&word, WORDSIZE);
    if (word == eventHeaderValue) {
      if (DEBUG_VERBOSE_UNPACKER) {
          cout << "[SpillUnpack][UnpackEvents]| reading event header\n" ;
        }

      nevt++;
      rawFile->read ((char*)&eventH.eventNumber, WORDSIZE);
      rawFile->read ((char*)&eventH.eventSize, WORDSIZE);
      rawFile->read ((char*)&eventH.nBoards, WORDSIZE);

      if (DEBUG_UNPACKER) {
        cout << "[SpillUnpack][UnpackEvents]| ======== EVENT START ======= " << endl;
        cout << "[SpillUnpack][UnpackEvents]|  Event " << eventH.eventNumber << endl;
        cout << "[SpillUnpack][UnpackEvents]|  Boards in event " << eventH.nBoards << endl;
      }
      
      if (eventH.eventNumber!=nevt) {
        cout << "[SpillUnpack][UnpackEvents]| ERROR event numbering inconsistent! " << endl ;
      }
      
      UnpackBoards (eventH.nBoards) ;
      continue;
    } 
      
    if (word==eventTrailerValue) {
      if (DEBUG_VERBOSE_UNPACKER) {
          cout << "[SpillUnpack][UnpackEvents]| reading event trailer\n" ;
        }
      if (DEBUG_UNPACKER) cout << "[SpillUnpack][UnpackEvents]|  ========= EVENT END ======== \n" ;
      continue;
    } 
    
    if (word==spillTrailerValue) {
      if (DEBUG_VERBOSE_UNPACKER) {
          cout << "[SpillUnpack][UnpackEvents]| reading spill trailer\n" ;
        }
      if (DEBUG_UNPACKER) cout << "[SpillUnpack][UnpackEvents]| ========= SPILL END ======== \n" ;
      if (DEBUG_UNPACKER || nevents!=nevt) cout << "[SpillUnpack][UnpackEvents]| Read " 
                                                << nevt << " events. Expected " 
                                                << nevents << "\n" ;
      break;
    }

//    cout << " ERROR corrupt RAW file. Read " << word << endl;

  } // loop over the input file    
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void SpillUnpack::UnpackBoards(int nboards) {

  WORD word;
  WORD nbrd=0;

  // loop on boards to be read
  for (int iBoard = 0 ; iBoard < nboards ; ++iBoard) {
    boardHeader bH;

    rawFile->read ((char*)&word, WORDSIZE);
    if (word==boardHeaderValue) {
      nbrd++;
      rawFile->read ((char*)&bH.boardID, WORDSIZE);
      rawFile->read ((char*)&bH.boardSize, WORDSIZE);

      if (DEBUG_UNPACKER) {
        cout << "[SpillUnpack][UnpackBoards]| ======== BOARD START ======= \n" ;
        cout << "[SpillUnpack][UnpackBoards]| Board " << nbrd 
             << "  Type " << bH.boardID << "\n" ;
      }
      continue;
    } 

    if (word == eventTrailerValue) {
      if (DEBUG_UNPACKER) cout << "[SpillUnpack][UnpackBoards]| ========= EVENT END ======== \n" ;
      if (DEBUG_UNPACKER || nboards!=nbrd) 
         cout << " [SpillUnpack][UnpackBoards]| [ERROR] Read " 
              << nbrd << " boards. Expected " 
              << nboards << "\n" ;
      break;
    }

    if (word==boardTrailerValue) {
      if (DEBUG_UNPACKER) cout << "[SpillUnpack][UnpackBoards]| ========= BOARD END ======== " << endl;
      continue;
    } 
          
    if (DEBUG_VERBOSE_UNPACKER) { 
        cout << "[SpillUnpack][UnpackBoards]| Something went wrong, read word " << word << endl;
      }
  }
}
