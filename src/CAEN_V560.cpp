#include "interface/CAEN_V560.hpp"

#define DEBUG_UNPACKER 1

using namespace std ;

int CAEN_V560::Unpack(dataType &stream, Event* event) {

  WORD currWord = 0 ;
//  WORD boardTrailerValue = *((uint32_t*)"BRDT");
//  int nWords = 0 ;

//  while (currWord != boardTrailerValue) {
  for (unsigned int i=0;i<scaler560size_;++i) {

    stream.read ((char*)&currWord, WORDSIZE);
    if (DEBUG_UNPACKER) 
      cout << "[CAEN_V560][Unpack]        | Read " << currWord << endl;

  }

}
