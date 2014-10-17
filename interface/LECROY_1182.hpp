#ifndef LECROY_1182_H
#define LECROY_1182_H

#include "interface/Board.hpp"

class LECROY_1182: public Board {

public: 
  LECROY_1182 (unsigned int size = 0) :   
    size_ ( (size-16)/sizeof(WORD) )
  {} ;

  int Unpack(dataType &stream,  Event*, boardHeader&);
  int Unpack (dataType &stream) { return -1 ; } ; //PG FIXME to be removed                                                          

private:
  unsigned int size_;
};

#endif
