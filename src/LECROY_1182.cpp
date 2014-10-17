#include "interface/LECROY_1182.hpp"

#define DEBUG_UNPACKER 0

int LECROY_1182::Unpack(dataType &stream, Event *event,boardHeader &bH) {

  WORD currWord = 0 ;
  adcData thisData;

  for (unsigned int i=0;i<size_;++i) {
    
    stream.read ((char*)&currWord, WORDSIZE);    

    thisData.board      =  bH.boardID; 
    thisData.channel    =  i;
    thisData.adcReadout =  currWord;

    event->adcValues.push_back(thisData);

    if (DEBUG_UNPACKER) 
      cout << "[LECROY_1182][Unpack]     | Read pattern: " << currWord
	   << ", channel " << thisData.channel << ", board " << thisData.board << endl;
    
  }

  return(1);

}
