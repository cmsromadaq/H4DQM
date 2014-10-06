#include "interface/SpillUnpack.hpp"

WORD spillHeaderValue;
WORD spillTrailerValue;
WORD eventHeaderValue;
WORD eventTrailerValue;
WORD boardHeaderValue;
WORD boardTrailerValue;


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


SpillUnpack::SpillUnpack(ifstream* in, TFile* out, TTree * outTree) {
  rawFile = in ;
  outFile_ = out ;
  outTree_ = outTree ;

  spillHeaderValue = *((uint32_t*)"SPLH");
  spillTrailerValue = *((uint32_t*)"SPLT");
  eventHeaderValue = *((uint32_t*)"EVTH");
  eventTrailerValue = *((uint32_t*)"EVNT");
  boardHeaderValue = *((uint32_t*)"BRDH");
  boardTrailerValue = *((uint32_t*)"BRDT");

  event_ = new Event (outFile_, outTree) ;

}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


SpillUnpack::~SpillUnpack ()
{
  delete event_ ;
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
        if (-1 != events) return 0 ; 
      } 
      else 
      { 
        cout << "[SpillUnpack][Unpack]      | ERROR corrupted RAW file. "
             << "Expecting spill header, read " << word << endl ;
        return 1 ;
      }
  } // while loop

  return 0 ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void SpillUnpack::UnpackEvents (WORD nevents) {

  WORD word;
  WORD nevt = 1 ;

  // loop over the input file
  while (!rawFile->eof()) {
    
    if (nevt > nevents) break ;

    eventHeader eventH;

    rawFile->read ((char*)&word, WORDSIZE);
    
    if (word == eventHeaderValue) {

      rawFile->read ((char*)&eventH.eventNumber, WORDSIZE);
      rawFile->read ((char*)&eventH.eventSize, WORDSIZE);
      rawFile->read ((char*)&eventH.nBoards, WORDSIZE);

      event_->evtNumber = eventH.eventNumber ;

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
      
    if (word == eventTrailerValue) {
      event_->Fill () ;
      ++nevt ;
      if (DEBUG_UNPACKER) cout << "[SpillUnpack][UnpackEvents]|  ========= EVENT END ======== \n" ;
      continue;
    } 
    
    if (word == spillTrailerValue) 
      {
        if (DEBUG_UNPACKER) cout << "[SpillUnpack][UnpackEvents]| ========= SPILL END ======== \n" ;
        if (DEBUG_UNPACKER || nevents!=nevt) cout << "[SpillUnpack][UnpackEvents]| Read " 
                                                  << nevt << " events. Expected " 
                                                  << nevents << "\n" ;
        break;
      }

    cout << "[SpillUnpack][UnpackEvents]| ERROR corrupt RAW file. Reading " << word << endl;
    break ;

  } // loop over the input file    
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int SpillUnpack::AddBoard (boardHeader bH)
{
  WORD boardType = GetBoardTypeId (bH.boardID) ;
  
  if (DEBUG_VERBOSE_UNPACKER) {
    cout << "[SpillUnpack][AddBoard]    | Creating new board type " << boardType << endl;
  }
  
  switch(boardType){
  case _TIME_:
    boards_[bH.boardID] = new TIME;
    break;
  case _CAENVX718_:
    boards_[bH.boardID]= new CAEN_VX718;
    break;
  case _CAENV1742_:
    boards_[bH.boardID]= new CAEN_V1742 (bH.boardSize) ;
    break;
  case _CAENV513_:
    boards_[bH.boardID]= new CAEN_V513;
    break;
  case _CAENV262_:
    boards_[bH.boardID]= new CAEN_V262;
    break;
  case _CAENV792_:
    boards_[bH.boardID]= new CAEN_V792;
    break;
  case _CAENV1290_:
    boards_[bH.boardID]= new CAEN_V1290;
    break;
  case _CAENV1495PU_:
    boards_[bH.boardID]= new CAEN_V1495PU;
    break;
  case _CAENV560_:
    boards_[bH.boardID]= new CAEN_V560;
    break;
  case _UNKWN_:
    boards_[bH.boardID]= new DummyBoard;
    //TO DO decide what to do.continue?
    break;
  }
  return boards_.size () ;
} 


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


WORD SpillUnpack::GetBoardTypeId (WORD compactId)
{
  WORD myResult= (compactId & static_cast<WORD>(0xFF000000))>>24;
  return myResult;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void SpillUnpack::UnpackBoards(WORD nboards) {

  WORD word ;
  WORD nbrd = 0 ;

  // loop on boards to be read
  for (unsigned int iBoard = 0 ; iBoard < nboards ; ++iBoard) 
  {
    rawFile->read ((char*)&word, WORDSIZE);

    if (word == boardHeaderValue) 
      {
        boardHeader bH;
        rawFile->read ((char*)&bH.boardID, WORDSIZE);
        rawFile->read ((char*)&bH.boardSize, WORDSIZE);
        int boardType = GetBoardTypeId (bH.boardID) ;

        if (DEBUG_UNPACKER) 
          {
            cout << "[SpillUnpack][UnpackBoards]| ======== BOARD START ======= \n" ;
            cout << "[SpillUnpack][UnpackBoards]| Board " << iBoard << "/" << nboards
                 << "  ID " << bH.boardID
                 << "  type " << boardType
                 << "  size " << bH.boardSize << "\n" ;
          }

        if (boards_.find (bH.boardID) == boards_.end ()) // not found the board creates a new one
          {
            AddBoard (bH) ;
          }
        //PG unpack the board boards_[bH.boardID]
        boards_[bH.boardID]->Unpack (*rawFile, event_) ;

        nbrd++;
      } 
    else
      {
        cout << "[SpillUnpack][UnpackBoards]| ERROR :" 
             << " header for board " << iBoard << " not found\n" ;  
        break ;     
      }  

    rawFile->read ((char*)&word, WORDSIZE) ;
    if (word == boardTrailerValue) 
      {
        if (DEBUG_UNPACKER) cout << "[SpillUnpack][UnpackBoards]| ========= BOARD END ======== " << endl;
        continue;
      } 
    else
      {
        cout << "[SpillUnpack][UnpackBoards]| ERROR :" 
             << " trailer for board " << iBoard << " not found\n" ; 
        break ; 
      }  




//     if (word == eventTrailerValue) {
//       if (DEBUG_UNPACKER) cout << "[SpillUnpack][UnpackBoards]| ========= EVENT END ======== \n" ;
//       if (DEBUG_UNPACKER || nboards != nbrd) 
//          cout << "[SpillUnpack][UnpackBoards]| [ERROR] Read " 
//               << nbrd << " boards. Expected " 
//               << nboards << "\n" ;
//       break;
//     }

          
//     if (DEBUG_VERBOSE_UNPACKER) { 
//         cout << "[SpillUnpack][UnpackBoards]| ERROR Something went wrong, read word " << word << endl;
//       }
  } // loop on boards to be read
}
