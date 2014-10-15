#ifndef CAEN_V1495PU_H
#define CAEN_V1495PU_H

#include "interface/Board.hpp"

class CAEN_V1495PU: public Board {

public: 
  CAEN_V1495PU (unsigned int size = 0) :   
      size_ (size / 4 - 4)
    {} ;

  int Unpack (dataType &stream) { return -1 ; } ; //PG FIXME to be removed
  int Unpack (dataType &stream, Event * event, boardHeader&) ;

private :

  unsigned int size_ ;

};

#endif
