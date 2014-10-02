#ifndef CAEN_V262_H
#define CAEN_V262_H

#include "interface/Board.hpp"

class CAEN_V262: public Board {

  int Unpack(dataType &stream);

};

#endif