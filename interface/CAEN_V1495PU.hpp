#ifndef CAEN_V1495PU_H
#define CAEN_V1495PU_H

#include "interface/Board.hpp"

class CAEN_V1495PU: public Board {

  int Unpack(dataType &stream);

};

#endif
