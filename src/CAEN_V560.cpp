#include "interface/CAEN_V560.hpp"

int CAEN_V560::Unpack(dataType &stream, Event* event) {

  WORD currWord = 0 ;
//  WORD boardTrailerValue = *((uint32_t*)"BRDT");
//  int nWords = 0 ;

//  while (currWord != boardTrailerValue) {
  for (int i=0;i<scaler560size_;++i) {

    stream.read ((char*)&currWord, WORDSIZE);
    if (DEBUG_VERBOSE_UNPACKER) 
      cout << "[CAEN_V560][Unpack]        | Read " << currWord << endl;

  }

}
