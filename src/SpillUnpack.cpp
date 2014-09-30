#include "interface/SpillUnpack.hpp"

SpillUnpack::SpillUnpack(){};

int SpillUnpack::Unpack(dataType &stream){};

void SpillUnpack::InitBoards(){
  //init differend kind of boards
  for (int i=0;i<nBoardTypes_;++i){
    switch(i){
    case 0:
      boards_[i] = new ;//TODO init different kind of boards. each one is a class inheriting from Board
      break;
    case 1:
      boards_[i]= new ;
      break;
    }
  }

}

