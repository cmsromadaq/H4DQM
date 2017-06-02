#ifndef CAEN_V1742_H
#define CAEN_V1742_H

#include "interface/StandardIncludes.hpp"
#include "interface/Board.hpp"
#include "interface/Event.hpp"

class CAEN_V1742: public Board {

public :

  CAEN_V1742 (unsigned int dig1742Words = 0) :   
      dig1742channels_ (0),
      dig1742Words_ (dig1742Words / 4 - 4),
      isDigiSample_ (0),
      nChannelWords_ (0),
      nSamplesToReadout_ (0),
      nSamplesRead_ (0),
      channelId_ (-1),
      groupId_ (-1)
    {} ;

  int Unpack (dataType &stream) { return -1 ; } ; //PG FIXME to be removed
  int Unpack_test (dataType &stream, Event * event) ;
  int Unpack (dataType &stream, Event * event, boardHeader&) ;

private :

  //PG FIXME deprecated, I want it in the ctor only
  int setNumOfWords (unsigned int dig1742Words) 
    {
      dig1742Words_ = dig1742Words ;
      return 0 ;
    }
  
  unsigned int dig1742channels_ ;
  unsigned int dig1742Words_ ;
  bool isDigiSample_ ;
  unsigned int digRawData_ ;
  unsigned int digRawSample_ ;

  int nChannelWords_ ;
  int nSamplesToReadout_ ;
  int nSamplesRead_ ;
  int channelId_ ;
  int groupId_ ;
  WORD frequency_ ;


};

#endif
