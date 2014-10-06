#ifndef DummyBoard_H
#define DummyBoard_H

#include "interface/Board.hpp"

class DummyBoard: public Board {

  int Unpack(dataType &stream);

};

#endif