#include "interface/CAEN_V560.hpp"

#define DEBUG_UNPACKER 0

using namespace std ;

int CAEN_V560::Unpack(dataType &stream, Event* event) {

  WORD currWord = 0 ;
  for (unsigned int i=0;i<scaler560size_;++i) {

    stream.read ((char*)&currWord, WORDSIZE);
    if (DEBUG_UNPACKER) 
      cout << "[CAEN_V560][Unpack]        | Read " << currWord << endl;
      
    event->scalerWords.push_back((unsigned int)currWord);
  }

  return(1);
}
