#ifndef CAEN_V792_H
#define CAEN_V792_H

#include "interface/Board.hpp"

class CAEN_V792: public Board {

  int Unpack(dataType &stream);

};

#endif