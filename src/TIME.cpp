#include "interface/TIME.hpp"
#include <assert.h>

#define DEBUG_UNPACKER 1

int TIME::Unpack (dataType &stream, Event * event, boardHeader &bH) 
{
  // The numbers are written in the board in little endian.
  uint64_t  myTime;
  stream.read ((char*)&myTime, 2 * WORDSIZE) ;
  myTime*=10;
  if ( bH.crateId==1)
 	 event->evtTime1 = myTime;
  else if ( bH.crateId==2)
 	 event->evtTime2 = myTime ;
  else if ( bH.crateId==3)
 	 event->evtTime2 = myTime ;
  else 
	{
	printf("Configured only 3 crates for time %u\n",bH.crateId);
	assert(0);
	}
 
   
  if (DEBUG_UNPACKER)
	{
	  cout << "[TIME][Unpack]             |  reading of " << myTime << " us \n" ; 
	}
  return 0 ; 

}
