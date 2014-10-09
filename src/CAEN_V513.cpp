#include "interface/CAEN_V513.hpp"

#define DEBUG_UNPACKER 0

int CAEN_V513::Unpack(dataType &stream, Event* event) {

  WORD currWord = 0 ;

  stream.read ((char*)&currWord, WORDSIZE);
  if (DEBUG_UNPACKER)
      cout << "[CAEN_V513][Unpack]        | Read " << currWord << endl;
      
  event->triggerWord=(unsigned int)currWord;
  
  return 1;
}
