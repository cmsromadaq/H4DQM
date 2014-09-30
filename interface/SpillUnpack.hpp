#ifndef SPILLUNPCK_H
#define SPILLUNPCK_H

#include "interface/EventBuilder.hpp"
#include "interface/DataType.hpp"
#include "interface/StandardIncludes.hpp"
#include "interface/Board.hpp"
using namespace std;


class SpillUnpack{

public:

  SpillUnpack();
  ~SpillUnpack();

  dataType stream;


  EventForTree evtTree_;
  TFile * outFile_;
  TTree* outTree_;
  UInt_t  boardId_;//probably useless
  UInt_t  boardType_;
  UInt_t  crateId_;
  Int_t  nBoardTypes_;
  map<nBoardTypes_,Board *> boards_;
  void InitBoards();//create a map between boardType and board
  int Unpack(dataType &stream);
  void CreateTree();
  void inline FillTree(){outTree_->Fill();};

};
