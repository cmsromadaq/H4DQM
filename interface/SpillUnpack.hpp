#ifndef SPILLUNPCK_H
#define SPILLUNPCK_H

//#include "interface/EventBuilder.hpp"
//#include "interface/DataType.hpp"

#include "interface/StandardIncludes.hpp"
#include "interface/TIME.hpp"
#include "interface/CAEN_VX718.hpp"
#include "interface/CAEN_V513.hpp"
#include "interface/CAEN_V262.hpp"
#include "interface/CAEN_V1742.hpp"
#include "interface/CAEN_V792.hpp"
#include "interface/CAEN_V1290.hpp"
#include "interface/CAEN_V1495PU.hpp"
#include "interface/CAEN_V560.hpp"
#include "interface/CAEN_V1495.hpp"
#include "interface/DummyBoard.hpp"
#include "interface/Event.hpp"

#include <TFile.h>
#include <TTree.h>

#define DEBUG_UNPACKER 1
#define DEBUG_VERBOSE_UNPACKER 1

struct spillHeader
{
  WORD runNumber;
  WORD spillNumber;
  WORD spillSize;
  WORD nEvents;
};

struct eventHeader
{
  WORD eventNumber;
  WORD eventSize;
  WORD nBoards;
};

struct boardHeader
{
  WORD boardID;
  WORD boardSize;
};


class SpillUnpack{

public:

  SpillUnpack();
  SpillUnpack(std::ifstream *in, TFile *out, TTree * outTree);
  ~SpillUnpack();

  // add a board if not existing
  int  AddBoard (boardHeader bH) ; 
  int  Unpack(int events);
  int UnpackEvents( WORD nevents );
  WORD GetBoardTypeId (WORD);
  int UnpackBoards( WORD nboards );
  void CreateTree();
  void inline FillTree(){outTree_->Fill();};

private:

  map<WORD, Board *> boards_;

  std::ifstream *rawFile;
  Event * event_ ;
  TFile * outFile_;
  TTree * outTree_;
  UInt_t  boardId_;//probably useless
  UInt_t  boardType_;
  UInt_t  crateId_;
//  Int_t  nBoardTypes_;

};

#endif
