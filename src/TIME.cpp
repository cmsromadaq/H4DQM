#include "interface/TIME.hpp"

#define DEBUG_UNPACKER 1

int TIME::Unpack (dataType &stream, Event * event) {

      WORD microseconds ;
      stream.read ((char*)&microseconds, WORDSIZE);
      // keep only the first six digits
      microseconds %= 1000000 ;
      WORD seconds ;
      stream.read ((char*)&seconds, WORDSIZE);
      event->evtTime = 1000000 * microseconds + seconds ;
      
      if (DEBUG_UNPACKER)
        {
          cout << "[TIME][Unpack]             |  reading of " << seconds << " s, " 
               << microseconds << " us \n" ; 
        }
      return 0 ; 

}
