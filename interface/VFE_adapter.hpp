#ifndef VFE_ADAPTER_H
#define VFE_ADAPTER_H

#include "interface/StandardIncludes.hpp"
#include "interface/Board.hpp"
#include "interface/Event.hpp"

class VFE_adapter: public Board {

public :

VFE_adapter (){} ;

int Unpack (dataType &stream) { return -1 ; } ; //PG FIXME to be removed
int Unpack (dataType &stream, Event * event, boardHeader&) ;

private :

int nSamples_;

};

#endif
