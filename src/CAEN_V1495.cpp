#include "interface/CAEN_V1495.hpp"

#define DEBUG_UNPACKER 1

#define SMALL_HODO_X_FIBERS_BITMASK 0x0000FF00
#define SMALL_HODO_Y_FIBERS_BITMASK 0x000000FF

int CAEN_V1495::Unpack(dataType &stream, Event* event) {

  WORD currWord = 0 ;

  stream.read ((char*)&currWord, WORDSIZE);
   
  if (DEBUG_UNPACKER)
    cout << "[CAEN_V1495][Unpack]        | Read " << currWord << endl;
 
  WORD small_hodo_x_word = (currWord & SMALL_HODO_X_FIBERS_BITMASK)>>8;
  WORD small_hodo_y_word = (currWord & SMALL_HODO_Y_FIBERS_BITMASK)>>8;

  // DO SOMETHING
  event->patterns_shodo.push_back( small_hodo_x_word );     
  event->patterns_shodo.push_back( small_hodo_y_word );     
  
  return 1;
}
