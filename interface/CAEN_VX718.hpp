#ifndef CAEN_VX718_H
#define CAEN_VX718_H

#include "interface/Board.hpp"

class CAEN_VX718: public Board {

  int Unpack(dataType &stream);

};

#endif
