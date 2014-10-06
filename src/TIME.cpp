#include "interface/TIME.hpp"

int TIME::Unpack (dataType &stream, Event * event) {
      WORD currWord = 0 ;
      WORD boardTrailerValue = *((uint32_t*)"BRDT");
      int nWords = 0 ;
      while (currWord != boardTrailerValue)
	{
          stream.read ((char*)&currWord, WORDSIZE);
          ++nWords ;
          cout << "[TIME][Unpack]             | Reaqding timestamp word " << nWords << " : " << currWord << endl;
          
        }
      return 1 ; 

}
