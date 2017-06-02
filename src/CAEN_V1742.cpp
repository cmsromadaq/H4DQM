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
  //              ...   = [          .....             ]


#include "interface/CAEN_V1742.hpp"
#include <bitset>

#define DEBUG_UNPACKER 0
#define DEBUG_VERBOSE_UNPACKER 0

using namespace std ;


int CAEN_V1742::Unpack (dataType &stream, Event * event, boardHeader &bH) 
{
  isDigiSample_ = 0 ;

  //PG loop over board words (the external header was already read in SpillUnpacker
  for (unsigned int i = 0 ; i < dig1742Words_ ; ++i)
    {
    //cout<< "[CAEN_V1742]::[Unpack]:: fr="<<i<<"/"<<dig1742Words_ 
    //    << "  BoardHeader "<<std::hex<<bH.boardID<<std::dec
    //    << "  Ch "<< channelId_
    //    <<endl;
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
          if (DEBUG_UNPACKER)
            {
              cout << "[CAEN_V1742][Unpack]       | number of words " << nWords << "\n" ;
            }
          if (nWords != dig1742Words_)
                {
              cout << "[CAEN_V1742][Unpack]       | ERROR: HEY MISMATCH IN DIGI 1742 #WORDS: " 
                       << 4 * nWords << " != " << (dig1742Words_ - 16) << "\n" ;
                }
        }
      else if (i==2)
        { 
          dig1742channels_ = digRawData_;
        }
      else if (i==3)
        { 
          unsigned int digiEvt = digRawData_;
          if (DEBUG_UNPACKER)
            {
              cout << "[CAEN_V1742][Unpack]       | DIGI 1742 BOE: event " << digiEvt+1 << "\n" ;
            }
          if (digiEvt+1 != event->id.evtNumber)
              cout << "[CAEN_V1742][Unpack]       | "
                   << "WARNING MISMATCH IN EVT NUMBER DIGIEVT " <<  digiEvt+1 
                   << " EVT " << event->id.evtNumber << endl;
            
        }
      else if (i>4)
        {
          if (!isDigiSample_)
            {
              if (0 == nSamplesToReadout_)
                {
                  //This is the ChHeader[0]
                  short dt_type = digRawData_>>28 & 0xF ; //ChHeader is 1000 (0x8)
    
		  frequency_ = digRawData_>>26 & 0x3 ;
		  // 00 = 5GS/s
		  // 01 = 2.5GS/s
		  // 10 = 1GS/s
		  // 11 = not used
                  if (DEBUG_UNPACKER) 
                    {
                      cout << "[CAEN_V1742][Unpack]       | data taking frequency : " 
                           << frequency_ << " : " << bitset<2> (frequency_) << "\n" ;
                           
                    }
                  if (dt_type != 0x8)
                    {
                      cout << "[CAEN_V1742][Unpack]       | ERROR: DIGI 1742 BLOCK SEEMS CORRUPTED\n" ;
                      cout << "[CAEN_V1742][Unpack]       | ERROR: CH header : " << bitset<32> (digRawData_) << "\n" ;
                      cout << "[CAEN_V1742][Unpack]       | ERROR:             10987654321098765432109876543210\n" ;
                      cout << "[CAEN_V1742][Unpack]       | ERROR: quitting unpacking\n" ;                  
                      break ;
                    }
                  unsigned int nChWords = digRawData_ & 0x3FFFFFF ; 
//                  unsigned int nChWords2 = digRawData_ & 0xFFF ; 
                  //PG FIXME check whether the second is better, when the trigger digitisation is on
                  if (DEBUG_UNPACKER) 
                    {
                      cout << "[CAEN_V1742][Unpack]       | NEW CHANNEL size : " 
                           << nChWords << "\n" ;
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
                      cout << "[CAEN_V1742][Unpack]       | group " 
                           << groupId_ << " channel " << channelId_ << "\n" ;
                    }
                  //Next sample will be a sample and should be read as a float
                  isDigiSample_ = 1 ;
                }
                }
              else
                {
                  //This is a sample! 
                  digiData aDigiSample ;
                  aDigiSample.board = bH.boardID ;
                  aDigiSample.channel = channelId_ ;
                  aDigiSample.group = groupId_ ;
                  aDigiSample.frequency = frequency_ ;
                  aDigiSample.sampleIndex = nSamplesRead_ ;
                  aDigiSample.sampleValue = digRawSample_ & 0xFFFF;
                  if (DEBUG_VERBOSE_UNPACKER) 
                    {
                      cout << "[CAEN_V1742][Unpack]       | sample " 
                           << aDigiSample.sampleIndex 
                           << " : " << aDigiSample.sampleValue << "\n" ;
                    }
                  event->digiValues.push_back (aDigiSample) ;

                  if (1 == nSamplesToReadout_)
                    {
                      //Next sample will be a ChHeader and should be read as as a uint
                      isDigiSample_ = 0 ;
                      if (nSamplesRead_ + 3 != nChannelWords_)
                        {
                          cout << "[CAEN_V1742][Unpack]       | ERROR: DIGI 1742 BLOCK SEEMS CORRUPTED:"
                               << " NOT ALL THE SAMPLES WERE READOUT FOR CHANNEL " 
                               << channelId_ << " IN GROUP " << groupId_ << "\n"
                               << "[CAEN_V1742][Unpack]       |       expected " << nChannelWords_ - 3
                               << ", read " << nSamplesRead_ << "\n" ;
                        }
                    }
                  --nSamplesToReadout_ ;
                  ++nSamplesRead_ ;
                }
        } // else (i > 4)
    } //PG loop over board words

  //PG FIXME maybe the reading of the end group time data is missing,
  //PG FIXME which means that I need to decide when a group is finished being read
  
  return 0 ;

} ;
