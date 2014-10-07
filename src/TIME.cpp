#include "interface/TIME.hpp"

#define DEBUG_UNPACKER 1

int TIME::Unpack (dataType &stream, Event * event) 
{
  // The numbers are written in the board in little endian.

  WORD microseconds ;
  stream.read ((char*)&microseconds, WORDSIZE);
  // keep only the first six digits
  microseconds %= 1000000 ;
  WORD seconds ;
  stream.read ((char*)&seconds, WORDSIZE);
  event->evtTime = seconds << 32 ;
  event->evtTime += microseconds ;
  
  if (DEBUG_UNPACKER)
	{
	  cout << "[TIME][Unpack]             |  reading of " << seconds << " s, " 
		   << microseconds << " us \n" ; 
	}
  return 0 ; 

}
