#ifndef CAEN_V792_H
#define CAEN_V792_H

#include "interface/Board.hpp"

class CAEN_V792: public Board {

public:

  CAEN_V792 (unsigned int adc792Channels = 0) :   
      adc792Channels_ (adc792Channels / 4 - 4)
    {} ;

  int Unpack (dataType &stream) { return -1 ; } ; //PG FIXME to be removed
  int Unpack (dataType &stream, Event * event) ;

private:

  unsigned int adc792Channels_;  

};

#endif