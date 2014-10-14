#ifndef CAEN_V1495_H
#define CAEN_V1495_H

#include "interface/Board.hpp"

class CAEN_V1495: public Board {

public:
  CAEN_V1495 () {} ;

  int Unpack (dataType &stream) { return -1 ; } ; //PG FIXME to be removed
  int Unpack (dataType &stream, Event * event) ;

};

#endif
