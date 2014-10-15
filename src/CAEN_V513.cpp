#include "interface/CAEN_V513.hpp"

#define DEBUG_UNPACKER 0

int CAEN_V513::Unpack(dataType &stream, Event* event,boardHeader &bH) {

  WORD currWord = 0 ;

  stream.read ((char*)&currWord, WORDSIZE);
  if (DEBUG_UNPACKER)
      cout << "[CAEN_V513][Unpack]        | Read " << currWord << endl;
      
  triggerWordData myTrigger;
  myTrigger.triggerWord= currWord;
  myTrigger.board= bH.boardID;

  event->triggerWords.push_back(myTrigger);
  
  return 1;
}
