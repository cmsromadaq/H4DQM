#include "interface/CAEN_V1495PU.hpp"

#define DEBUG_UNPACKER 0

int CAEN_V1495PU::Unpack(dataType &stream, Event *event,boardHeader &bH) {

  WORD currWord = 0 ;
  patternData myPattern;
  for (unsigned int i=0;i<size_;++i) {

    stream.read ((char*)&currWord, WORDSIZE);
    
    if ((i==0) && (currWord!=0x50000004)) //
      cout << "[CAEN_V1495PU][Unpack]     | Header corrupted, read " << currWord << endl;
    if (i>1 && i<size_-1) {

      //event->patterns.push_back((unsigned int)currWord);
      myPattern.board=bH.boardID;
      myPattern.channel=i-1; // TODO: check offset by one
      myPattern.patternValue=currWord;
      event->patterns.push_back( myPattern );

      if (DEBUG_UNPACKER) 
        cout << "[CAEN_V1495PU][Unpack]     | Read pattern: " << currWord << "  size: " << event->patterns.size() << endl;
    }
  }

  if (currWord!=0x30000007)
    cout << "[CAEN_V1495PU][Unpack]     | Footer corrupted, read " << currWord << endl;

  return(1);

}
