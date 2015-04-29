#include "interface/SpillUnpack.hpp"

WORD spillHeaderValue;
WORD spillTrailerValue;
WORD eventHeaderValue;
WORD eventTrailerValue;
WORD boardHeaderValue;
WORD boardTrailerValue;


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


SpillUnpack::SpillUnpack(ifstream* in, TFile* out, TTree * outTree,int prescale) {
  rawFile = in ;
  outFile_ = out ;
  outTree_ = outTree ;
  prescale_ = prescale;

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


int SpillUnpack::AddBoard (boardHeader bH)
{
  // we create a basic structure to read the board content. The unpacker will define what is effectively meaningful
  WORD boardType = GetBoardTypeId (bH.boardID) ;
  //WORD boardId   = GetBoardId (bH.boardID) ;
  //WORD crateId   = GetBoardCrateId (bH.boardID) ;
  
  if (DEBUG_VERBOSE_UNPACKER) {
    cout << "[SpillUnpack][AddBoard]    | Creating new board type " << boardType 
	//	<< " board Id: "<<boardId
	//	<< " crate Id: "<<crateId
		<< endl;
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
    boards_[bH.boardID]= new CAEN_V792(bH.boardSize);
    break;
  case _CAENV1290_:
    boards_[bH.boardID]= new CAEN_V1290 () ;
    break;
  case _CAENV1495PU_:
    boards_[bH.boardID]= new CAEN_V1495PU (bH.boardSize) ;
    break;
  case _CAENV560_:
    boards_[bH.boardID]= new CAEN_V560(bH.boardSize);
    break;
  case _LECROY1182_:
    boards_[bH.boardID]= new LECROY_1182(bH.boardSize);
    break;
  case _CAENV785_:      //785 event format exacly as 792
    boards_[bH.boardID]= new CAEN_V792(bH.boardSize);
    break;
  case _UNKWN_:
    boards_[bH.boardID]= new DummyBoard;
    std::cout << "UNKNOWN BOARD " << std::hex << bH.boardID << std::dec << std::endl;
    //TO DO decide what to do.continue?
    break;
  default:
    boards_[bH.boardID]= new DummyBoard;
    std::cout << "DEFAULT DUMMYBOARD " << std::hex << bH.boardID << std::dec << std::endl;
    //TO DO decide what to do.continue?
  }
  return boards_.size () ;
} 


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

// TODO use EventBuilder:: functions static, to auto update if something is changed
WORD SpillUnpack::GetBoardTypeId (WORD compactId)
{
  WORD myResult= (compactId & static_cast<WORD>(0xFF000000))>>24;
  return myResult;
}
WORD SpillUnpack::GetBoardId (WORD compactId)
{
  WORD myResult= (compactId & static_cast<WORD>(0x0000FFFF));
  return myResult;
}
WORD SpillUnpack::GetBoardCrateId (WORD compactId)
{
  WORD myResult= (compactId & static_cast<WORD>(0x00FF0000))>>16;
  return myResult;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int SpillUnpack::Unpack(int events = -1){

  WORD word;

  while (!rawFile->eof()) {
      spillHeader spillH;

      rawFile->read ((char*)&word, WORDSIZE);

      if (word != spillHeaderValue ) 
      	{ 
      	  cout << "[SpillUnpack][Unpack]      | ERROR corrupted RAW file. "
      	       << "Expecting spill header, read " << word << endl ;
      	  return 1 ;
      	}
     
      if (word==spillHeaderValue) {
        rawFile->read ((char*)&spillH.runNumber, WORDSIZE);
        rawFile->read ((char*)&spillH.spillNumber, WORDSIZE);
        rawFile->read ((char*)&spillH.spillSize, WORDSIZE);
        rawFile->read ((char*)&spillH.nEvents, WORDSIZE);

        if (DEBUG_UNPACKER) {
          cout << "[SpillUnpack][Unpack]      | ======= BEGIN SPILL ======= \n" ;
          cout << "[SpillUnpack][Unpack]      | Spill " 		<< spillH.spillNumber << "\n" ;
          cout << "[SpillUnpack][Unpack]      | Events in spill " 	<< spillH.nEvents << "\n" ;
          cout << "[SpillUnpack][Unpack]      | unpacking " 		<< events << " events" << endl ;
        }
        
        if (-1 == events) events = spillH.nEvents ; 
        UnpackEvents (events,&spillH) ;
      } 

      rawFile->read ((char*)&word, WORDSIZE);

      if (word == spillTrailerValue) 
        {
          if (DEBUG_UNPACKER) cout << "[SpillUnpack][UnpackEvents]| ========= SPILL END ======== \n" ;
          return 1;
        }
      else 
        { 
          cout << "[SpillUnpack][Unpack]      | ERROR corrupted RAW file. "
               << "Expecting spill trailer, read " << word << endl ;
          return -1;
        }

  } // while loop
  return 0 ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int SpillUnpack::UnpackEvents (WORD nevents, spillHeader *this_spill) {

  WORD word;
  WORD nevt = 1 ;

  // loop over the input file
  while (!rawFile->eof()) 
    {
      if (nevt > nevents) break ;
      eventHeader eventH;
      rawFile->read ((char*)&word, WORDSIZE);
      
      if (word == eventHeaderValue) 
        {
          event_->clear () ;
	  std::streampos eventStartPos=rawFile->tellg();
          rawFile->read ((char*)&eventH.eventNumber, WORDSIZE);
          rawFile->read ((char*)&eventH.eventSize, WORDSIZE);
          rawFile->read ((char*)&eventH.nBoards, WORDSIZE);
	  if (((eventH.eventNumber+(rand()%999))% prescale_)!=0)
	    {
	      //Skipping events with prescale factor
	      rawFile->seekg(std::streampos(eventH.eventSize-4*WORDSIZE),ios_base::cur);
	      ++nevt;
	      continue;
	    }

	  event_->id.runNumber = this_spill->runNumber;
	  event_->id.spillNumber = this_spill->spillNumber;
          event_->id.evtNumber = eventH.eventNumber ;
    
          if (DEBUG_UNPACKER) 
            {
              cout << "[SpillUnpack][UnpackEvents]| ======== EVENT START ======= " << endl;
              cout << "[SpillUnpack][UnpackEvents]|  Event " << eventH.eventNumber << endl;
              cout << "[SpillUnpack][UnpackEvents]|  Boards in event " << eventH.nBoards << endl;
            }
          
          if (eventH.eventNumber!=nevt) 
            {
              cout << "[SpillUnpack][UnpackEvents]| ERROR event numbering inconsistent! " << endl ;
            }
          
          UnpackBoards (eventH.nBoards) ;
        } 
      else
        {
          cout << "[SpillUnpack][UnpackEvents]| ERROR event header not found " << endl ;
          return 1 ;
        }  
        
      rawFile->read ((char*)&word, WORDSIZE);

      if (word == eventTrailerValue) 
        {
          event_->Fill () ;
          ++nevt ;
          if (DEBUG_UNPACKER) cout << "[SpillUnpack][UnpackEvents]|  ========= EVENT END ======== \n" ;
        } 
      else
        {
          cout << "[SpillUnpack][UnpackEvents]| ERROR event trailer not found " << endl ;
          return 1 ;
        }  
    } // loop over the input file    

  return 0 ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int SpillUnpack::UnpackBoards(WORD nboards) {

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

	bH.Update();

        if (DEBUG_UNPACKER) 
          {
            cout << "[SpillUnpack][UnpackBoards]| ======== BOARD START ======= \n" ;
            cout << "[SpillUnpack][UnpackBoards]| Board " << iBoard << "/" << nboards
		 << std::dec 
                 << "  Packed_ID " << bH.boardID
                 << "  type " 	<< bH.boardType
                 << "  id " 	<< bH.boardSingleId
                 << "  crate " 	<< bH.crateId
                 << "  size " 	<< bH.boardSize 
		 << endl ;
          }

        if (boards_.find (bH.boardID) == boards_.end ()) // not found the board creates a new one
          {
            AddBoard (bH) ;
          }
        //PG unpack the board boards_[bH.boardID]
        boards_[bH.boardID]->Unpack (*rawFile, event_,bH) ;

        nbrd++;
      } 
    else
      {
        cout << "[SpillUnpack][UnpackBoards]| ERROR :" 
             << " header for board " << iBoard << " not found\n" ;  
        return 1 ;     
      }  

    rawFile->read ((char*)&word, WORDSIZE) ;
    if (word == boardTrailerValue) 
      {
        if (DEBUG_UNPACKER) cout << "[SpillUnpack][UnpackBoards]| ========= BOARD END ======== " << endl;
      } 
    else
      {
        cout << "[SpillUnpack][UnpackBoards]| ERROR :" 
             << " trailer for board " << iBoard << " not found " << std::hex << word << std::dec << std::endl;
        return 1 ; 
      }  
  } // loop on boards to be read
  return 0 ;
}

void boardHeader::Update(){
		boardType	=SpillUnpack::GetBoardTypeId( boardID) ; 
		boardSingleId	=SpillUnpack::GetBoardId(boardID); 
		crateId		=SpillUnpack::GetBoardCrateId(boardID);  
};
