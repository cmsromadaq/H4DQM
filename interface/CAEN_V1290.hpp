#ifndef CAEN_V1290_H
#define CAEN_V1290_H

#include "interface/Board.hpp"

class CAEN_V1290: public Board 
{
public: 

  CAEN_V1290 (WORD tdc1290Words) : tdc1290Words_ (tdc1290Words / 4 - 4) {} ;
  int Unpack (dataType &stream) { return -1 ; } ; //PG FIXME to be removed
  int Unpack (dataType &stream, Event * event, boardHeader &bH) ;

private:
  WORD tdc1290Words_ ;

};

#endif
