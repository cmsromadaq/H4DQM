#ifndef CAEN_V560_H
#define CAEN_V560_H

#include "interface/Board.hpp"

class CAEN_V560: public Board {

public:
  CAEN_V560 (unsigned int scaler560size = 0) :   
      scaler560size_ (scaler560size / 4 - 4)
    {} ;

  int Unpack (dataType &stream) { return -1 ; } ; //PG FIXME to be removed
  int Unpack (dataType &stream, Event * event) ;

private:

  unsigned int scaler560size_;  

};

#endif