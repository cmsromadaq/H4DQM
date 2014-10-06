#ifndef TIME_H
#define TIME_H

#include "interface/Board.hpp"

class TIME: public Board {

  int Unpack(dataType &stream) { return -1; };
  int Unpack (dataType &stream, Event * event) ;

};

#endif