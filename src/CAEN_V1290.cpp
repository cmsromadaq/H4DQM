#include "interface/CAEN_V1290.hpp"

#define DEBUG_UNPACKER 0
#define DEBUG_VERBOSE_UNPACKER 0

using namespace std;

int CAEN_V1290::Unpack (dataType &stream, Event * event,boardHeader &bH)
{
  for (unsigned int i = 0; i < tdc1290Words_; ++i)
	{
	  unsigned int tdcRawData;
	  stream.read ((char*)&tdcRawData, sizeof (tdcRawData));
	  //	  if (DEBUG_VERBOSE_UNPACKER) printf ("TDC WORD %d: %X\n",i,tdcRawData);
	  if (tdcRawData>>28 == 10 ) //TDC BOE
	    {
	      unsigned int tdcEvent=tdcRawData & 0xFFFFFFF;
	      if (DEBUG_UNPACKER) 
            cout << "[CAEN_V12490][Unpack]       | " 
                 << "TDC 1190 BOE: event " << tdcEvent+1 << "\n";
	      if (tdcEvent+1 != event->evtNumber)
            cout << "[CAEN_V12490][Unpack]       | " 
                 << "WARNING MISMATCH IN EVT NUMBER TDCEVT " 
                 <<  tdcEvent+1 << " EVT " << event->evtNumber << "\n";
	    }
	  else if (tdcRawData>>28 == 8) //TDC EOE
	    {
	    }
	  else if (tdcRawData>>28 == 0) //TDC DATUM
	    {
	      tdcData thisData;
	      thisData.board=bH.boardID;
	      thisData.channel = (tdcRawData>>21) & 0x1f;
	      thisData.tdcReadout = tdcRawData & 0x1fffff;
	      if (DEBUG_UNPACKER) 
            {
              cout << "[CAEN_V12490][Unpack]       | "
                   << "tdc 1190 board " << thisData.board
                   << "channel " << thisData.channel
                   << " tdcReadout " << thisData.tdcReadout << "\n";
            }
	      event->tdcValues.push_back (thisData);
	    }
	}
  return 0 ;
}

