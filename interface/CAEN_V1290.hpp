#ifndef CAEN_V1290_H
#define CAEN_V1290_H

#include "interface/Board.hpp"

class CAEN_V1290: public Board {

  int Unpack(dataType &stream);

};

#endif
