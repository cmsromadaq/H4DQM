#include "interface/TIME.hpp"

<<<<<<< HEAD
#define DEBUG_UNPACKER 1

int TIME::Unpack (dataType &stream, Event * event) 
{
  // The numbers are written in the board in little endian.
  stream.read ((char*)&event->evtTime, 2 * WORDSIZE) ;
  event->evtTime *= 10 ;
  
  if (DEBUG_UNPACKER)
	{
	  cout << "[TIME][Unpack]             |  reading of " << event->evtTime << " us \n" ; 
	}
  return 0 ; 

}
