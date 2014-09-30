#ifndef SPILLUNPCK_H
#define SPILLUNPCK_H

#include <stdint.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//#include "interface/EventBuilder.hpp"
//#include "interface/DataType.hpp"
//#include "interface/StandardIncludes.hpp"

#include <TFile.h>
#include <TTree.h>

#define WORDSIZE 4
typedef unsigned long long dataTypeSize_t;
typedef uint32_t WORD;

#define MAX_ADC_CHANNELS 200
#define MAX_DIGI_SAMPLES 100000
#define MAX_TDC_CHANNELS 200

#define DEBUG_UNPACKER 1
#define DEBUG_VERBOSE_UNPACKER 0

using namespace std;

struct spillHeader
{
  WORD header;
  WORD runNumber;
  WORD spillNumber;
  WORD nEvents;
};


class SpillUnpack{

public:

  SpillUnpack();
  SpillUnpack(std::ifstream *in, TFile *out);
  ~SpillUnpack();

//  EventForTree evtTree_;
  TFile * outFile_;
  TTree* outTree_;
  UInt_t  boardId_;//probably useless
  UInt_t  boardType_;
  UInt_t  crateId_;
  Int_t  nBoardTypes_;
//  map<nBoardTypes_,Board *> boards_;
  void InitBoards();//create a map between boardType and board
  int Unpack();
  void CreateTree();
  void inline FillTree(){outTree_->Fill();};

private:

  std::ifstream *rawFile;
  TFile *outFile;  

};

#endif