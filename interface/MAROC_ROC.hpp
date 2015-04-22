#ifndef MAROC_ROC_H
#define MAROC_ROC_H

#include "interface/Board.hpp"

#define MAROC_ROC_EVENT_WORDS 73
#define MAROC_ROC_EVENT_HEADER_WORDS 6
#define MAROC_ROC_EVENT_TRAILER_WORDS 1

class MAROC_ROC: public Board {

public:

  MAROC_ROC (unsigned int eventSize) :   
    eventSize_ (eventSize / 4 - 4)
    {} ;

  int Unpack (dataType &stream) { return -1 ; } ; //PG FIXME to be removed
  int Unpack (dataType &stream, Event * event, boardHeader &) ;

private:
  unsigned int eventSize_;  
};

#endif
