#include "interface/MAROC_ROC.hpp"
#include "interface/Event.hpp"

#define DEBUG_MAROC_UNPACKER 0

int MAROC_ROC::Unpack (dataType &stream, Event * event, boardHeader &bH) 
{
  WORD currWord = 0 ;
  adcData thisData;

  if (eventSize_ != MAROC_ROC_EVENT_WORDS)
    cout << "[MAROC_ROC][Unpack]        | GOT WRONG NUMBER OF EVENT WORDS " 
	 << eventSize_ << " EXPECTED " << MAROC_ROC_EVENT_WORDS << "\n" ;

  int xor_data=0XAAAA;

  for (unsigned int i=0;i<eventSize_;++i) {
    
    stream.read ((char*)&currWord, WORDSIZE);    

    if (DEBUG_MAROC_UNPACKER) 
      cout << "[MAROC_ROC][Unpack]     | Read Word: " << std::hex << currWord << std::dec << endl;

    if (i<MAROC_ROC_EVENT_HEADER_WORDS)
      {
	//HEADER DATA::doing nothing with this for the moment
      }
    else if (i<MAROC_ROC_EVENT_WORDS-MAROC_ROC_EVENT_TRAILER_WORDS)
      {
	//the actual adcData
	thisData.board      =  bH.boardID; 
	thisData.channel    =  i-MAROC_ROC_EVENT_HEADER_WORDS;
	thisData.adcReadout =  currWord;
	
	event->adcValues.push_back(thisData);
	xor_data=(xor_data^currWord)&0xFFFF;

	if (DEBUG_MAROC_UNPACKER) 
	  cout << "[MAROC_ROC][Unpack]     | Read pattern: " << thisData.adcReadout
	       << ", channel " << thisData.channel << ", board " << thisData.board << endl;
      }
    //for the last word
    else if (i==MAROC_ROC_EVENT_WORDS-1)
      {
	//Last word is ADC data XOR values, checking consistency
	if (currWord != (xor_data&0x1FFF) )
	  {
	    cout << "[MAROC_ROC]::[Unpack]     | Event XOR data not consistent " << std::hex << currWord << "," << (xor_data&0x1FFF) << std::dec << endl;
	  }
      }

  }

  return(1);
}
