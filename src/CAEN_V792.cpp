#include "interface/CAEN_V792.hpp"
#include "interface/Event.hpp"

#define DEBUG_VERBOSE_UNPACKER 0

int CAEN_V792::Unpack (dataType &stream, Event * event) 
{
  WORD currWord = 0 ;

//  WORD boardTrailerValue = *((uint32_t*)"BRDT");
//  while (currWord != boardTrailerValue) {
  for (int i=0;i<adc792Channels_;++i) {

    stream.read ((char*)&currWord, WORDSIZE);
    short dt_type = currWord>>24 & 0x7; //dt_type 0 is datum,2 is BOE,4 is EOE

    if (dt_type==0) {
      //FOR ADC792 DATA
      adcData thisData;
	      
      //ADC792 -> adcBoard 1
      thisData.board=1; 
      thisData.channel = currWord>>16 & 0x1F;
      thisData.adcReadout = currWord & 0xFFF;
      if ((currWord>>12) & 0x1) //overflow
	thisData.adcReadout=4095;
      if (DEBUG_VERBOSE_UNPACKER) 
        cout << "[CAEN_V792][Unpack]        | Board " << thisData.board
             << "  Channel " << thisData.channel
             << "  adcReadout " << thisData.adcReadout << endl; 
      event->adcValues.push_back(thisData);
    } 
 
    if (dt_type==4) {
      WORD adcEvt=currWord & 0xFFFFFF;
      if (DEBUG_VERBOSE_UNPACKER) cout << "[CAEN_V792][Unpack]        |  EOE. Event: " << adcEvt+1 << endl; 

//      if (adcEvt+1 != event->evtNumber)
//	cout << "[CAEN_V792][Unpack]        | HEY MISMATCH IN EVT NUMBER ADCEVT " << adcEvt+1 << " EVT " << event->evtNumber << endl;
    }

  }
}
