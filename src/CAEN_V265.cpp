#include "interface/CAEN_V265.hpp"

#define DEBUG_UNPACKER 0

int CAEN_V265::Unpack(dataType &stream, Event *event,boardHeader &bH) {

  WORD currWord = 0 ;
  adcData thisData;

  for (unsigned int i=0;i<size_;++i) {
    
    stream.read ((char*)&currWord, WORDSIZE);    

    thisData.board      =  bH.boardID; 
    thisData.channel    =  (currWord & 0xE000) >> 13;
    thisData.adcReadout =  currWord & 0xFFF;

    event->adcValues.push_back(thisData);

    if (DEBUG_UNPACKER) 
      cout << "[CAEN_V265][Unpack]     | Read pattern: " << thisData.adcReadout
	   << ", channel " << thisData.channel << ", board " << thisData.board << endl;
    
  }

  return(1);

}
