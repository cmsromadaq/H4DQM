#ifndef CAEN_V265_H
#define CAEN_V265_H

#include "interface/Board.hpp"

class CAEN_V265: public Board {

public: 
  CAEN_V265 (unsigned int size = 0) :   
    size_ ( (size-16)/sizeof(WORD) )
  {} ;

  int Unpack(dataType &stream,  Event*, boardHeader&);
  int Unpack (dataType &stream) { return -1 ; } ; //PG FIXME to be removed                                                          

private:
  unsigned int size_;
};

#endif
