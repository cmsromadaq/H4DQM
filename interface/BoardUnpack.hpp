#ifndef BRDUNPCK_H
#define BRDUNPCK_H

#include "interface/EventBuilder.hpp"
#include "interface/DataType.hpp"
#include "interface/StandardIncludes.hpp"
using namespace std;


class BoardUnpack {
public:
  
  BoardUnpack();
  ~BoardUnpack();

  int Unpack(dataType &stream);

};
