#ifndef CAEN_V513_H
#define CAEN_V513_H

#include "interface/Board.hpp"

class CAEN_V513: public Board {

public:
  CAEN_V513 () {} ;

  int Unpack (dataType &stream) { return -1 ; } ; //PG FIXME to be removed
  int Unpack (dataType &stream, Event * event) ;

};

#endif