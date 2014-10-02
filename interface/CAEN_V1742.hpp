#ifndef CAEN_V1742_H
#define CAEN_V1742_H

#include "interface/Board.hpp"

class CAEN_V1742: public Board {

  int Unpack(dataType &stream);

};

#endif
