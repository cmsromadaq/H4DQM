  //          ====================================================
  //          |           V1742 Raw Event Data Format            |
  //          ====================================================

  //                       31  -  28 27  -  16 15   -   0
  //            Word[0] = [ 1010  ] [Event Tot #Words  ] //Event Header (5 words)
  //            Word[1] = [     Board Id    ] [ Pattern]  
  //            Word[2] = [      #channels readout     ]
  //            Word[3] = [        Event counter       ]
  //            Word[4] = [      Trigger Time Tag      ]
  //            Word[5] = [ 1000  ] [    Ch0   #Words  ] // Ch0 Data (2 + #samples words)
  //            Word[6] = [    Ch0  #Gr    ] [ Ch0 #Ch ] 
  //            Word[7] = [ Ch0 Corr. samples  (float) ]
  //                ..  = [ Ch0 Corr. samples  (float) ]
  // Word[5+Ch0 #Words] = [ 1000  ] [    Ch1   #Words  ] // Ch1 Data (2 + #samples words)
  // Word[6+Ch0 #Words] = [    Ch1  #Gr    ] [ Ch1 #Ch ]
  // Word[7+Ch0 #Words] = [ Ch1 Corr. samples  (float) ]
  //               ...   = [          .....             ]


#define DEBUG_UNPACKER 1
#define DEBUG_VERBOSE_UNPACKER 1

#include "interface/CAEN_V1742.hpp"

using namespace std ;

int CAEN_V1742::Unpack (dataType &stream, Event * event) 
{
  isDigiSample_ = 0 ;


  //PG loop over board words (the header was already read in SpillUnpacker
  for (int i = 0 ; i < dig1742Words_ ; ++i)
    {
      // read the word from the stream
      // ---- ---- ---- ---- ---- ---- ---- ----
    
      if (!isDigiSample_) stream.read ((char*)&digRawData_, sizeof (digRawData_)) ;
      else                stream.read ((char*)&digRawSample_, sizeof (digRawSample_)) ;

      if (i==0)
	{
	  short dt_type = digRawData_>>28 & 0xF; //BOE is 1010 (0xA)
	  if (dt_type != 0xA)
	    {
	      cout << "[CAEN_V1742][Unpack]       | ERROR: DIGI 1742 BLOCK SEEMS CORRUPTED\n" ;
	    }
	  unsigned int nWords = digRawData_ & 0xFFFFFFF; 
	  if (4 * nWords != dig1742Words_ - 16)
            {
	      cout << "[CAEN_V1742][Unpack]       | ERROR: HEY MISMATCH IN DIGI 1742 #WORDS: " 
                   << 4 * nWords << " != " << (dig1742Words_ - 16) << "\n" ;
            }
	}
      else if (i==2)
	{ 
	  dig1742channels_=digRawData_;
	}
      else if (i==3)
	{ 
	  unsigned int digiEvt = digRawData_;
	  if (DEBUG_UNPACKER)
	    {
	      cout << "[CAEN_V1742][Unpack]       | DIGI 1742 BOE: event " << digiEvt+1 << "\n" ;
	    }
	  //	  if (digiEvt+1 != thisEvent.evtNumber)
	  //	    std::cout << "HEY MISMATCH IN EVT NUMBER DIGIEVT " <<  digiEvt+1 << " EVT " << thisEvent.evtNumber << std::endl;
	        
	}
      else if (i>4)
        {
	  
	  // following words: ADC content
	  // ---- ---- ---- ---- ---- ---- ---- ----
	  
	  if (!isDigiSample_)
	    {
	      if (0 == nSamplesToReadout_)
		{
		  //This is the ChHeader[0]
		  short dt_type = digRawData_>>28 & 0xF ; //ChHeader is 1000 (0x8)
		  if (dt_type != 0x8)
		    {
		      cout << "[CAEN_V1742][Unpack]       | ERROR: DIGI 1742 BLOCK SEEMS CORRUPTED\n" ;
		      //PG FIXME add the board number here
		    }
		  unsigned int nChWords = digRawData_ & 0xFFFFFFF ; 
		  if (DEBUG_UNPACKER) 
		    {
		      cout << "[CAEN_V1742][Unpack]       | NEW CHANNEL size : " << nChWords << "\n" ;
		    }
		  nSamplesToReadout_ = nChWords ;
		  nChannelWords_ = nChWords ;
		  nSamplesRead_ = 0 ;
		  --nSamplesToReadout_ ;
		}
	      else
		{
		  //This is the ChHeader[1]
		  int gr=digRawData_>>16 & 0xFFFF ; 
		  int ch=digRawData_ & 0xFFFF ; 
		  channelId_ = ch ;
		  groupId_ = gr ;
		  --nSamplesToReadout_ ;
		  if (DEBUG_UNPACKER) 
		    {
		      cout << "[CAEN_V1742][Unpack]       | group " << groupId_ << " channel " << channelId_ << "\n" ;
		    }
		  //Next sample will be a sample and should be read as a float
		  isDigiSample_ = 1 ;
		}
	    }
	  else
	    {
	      //This is a sample! 
	      digiData aDigiSample ;
	      aDigiSample.channel = channelId_ ;
	      aDigiSample.group = groupId_ ;
	      aDigiSample.sampleIndex = nSamplesRead_ ;
	      aDigiSample.sampleValue = digRawSample_ ;
	      if (DEBUG_VERBOSE_UNPACKER) 
                {
	          cout << "[CAEN_V1742][Unpack]       | sample " << aDigiSample.sampleIndex 
                       << " : " << aDigiSample.sampleValue << "\n" ;
                }
	      //	      event->digiValues.push_back (aDigiSample) ;
	      
	      if (1 == nSamplesToReadout_)
		{
		  //Next sample will be a ChHeader and should be read as as a uint
		  isDigiSample_ = 0 ;
		  if (nSamplesRead_ + 3 != nChannelWords_)
		    {
		      cout << "[CAEN_V1742][Unpack]       | ERROR: DIGI 1742 BLOCK SEEMS CORRUPTED:"
			   << " NOT ALL THE SAMPLES WERE READOUT FOR CHANNEL " 
			   << channelId_ << " IN GROUP " << groupId_ << "\n" ;
		      
		    }
		}
	      --nSamplesToReadout_ ;
	      ++nSamplesRead_ ;
	    }
	} // else (i > 4)
    } //PG loop over board words
  
  return 0 ;

} ;
