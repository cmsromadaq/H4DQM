#ifndef CAEN_V513_H
#define CAEN_V513_H

#include "interface/Board.hpp"

class CAEN_V513: public Board {

  int Unpack(dataType &stream);

};

#endif