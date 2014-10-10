#include "interface/CAEN_V1495PU.hpp"

#define DEBUG_UNPACKER 1

int CAEN_V1495PU::Unpack(dataType &stream, Event *event) {

  WORD currWord = 0 ;
  for (unsigned int i=0;i<size_;++i) {

    stream.read ((char*)&currWord, WORDSIZE);
    
    if ((i==0) && (currWord!=0x50000004))
      cout << "[CAEN_V1495PU][Unpack]     | Header corrupted, read " << currWord << endl;
    if (i>1 && i<size_-1) {
      event->patterns.push_back((unsigned int)currWord);
      if (DEBUG_UNPACKER) 
        cout << "[CAEN_V1495PU][Unpack]     | Read pattern: " << currWord << "  size: " << event->patterns.size() << endl;
    }
  }

  if (currWord!=0x30000007)
    cout << "[CAEN_V1495PU][Unpack]     | Footer corrupted, read " << currWord << endl;

  return(1);

}
