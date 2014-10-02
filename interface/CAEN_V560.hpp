#ifndef CAEN_V560_H
#define CAEN_V560_H

#include "interface/Board.hpp"

class CAEN_V560: public Board {

  int Unpack(dataType &stream);

};

#endif