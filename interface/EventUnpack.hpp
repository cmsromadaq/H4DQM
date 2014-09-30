#ifndef EVTUNPCK_H
#define EVTUNPCK_H

#include "interface/EventBuilder.hpp"
#include "interface/DataType.hpp"
#include "interface/StandardIncludes.hpp"
using namespace std;


class EventUnpack {
public:
  
  EventUnpack();
  ~EventUnpack();

  int Unpack(dataType &stream);

};
