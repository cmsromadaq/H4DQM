#ifndef Event_hpp
#define Event_hpp


// struct treeStructData
// {
//   // unsigned int runNumber; to be added soon
//   unsigned int evtNumber;
//   unsigned int evtTimeDist;
//   unsigned int evtTimeStart;
//   unsigned int evtTime;
// 
//   unsigned int boardTriggerBit;
// 
//   unsigned int triggerWord;
// 
//   unsigned int nAdcChannels;
//   unsigned int adcBoard[MAX_ADC_CHANNELS];
//   unsigned int adcChannel[MAX_ADC_CHANNELS];
//   unsigned int adcData[MAX_ADC_CHANNELS];
// 
//   unsigned int nDigiSamples;
//   unsigned int digiGroup[MAX_DIGI_SAMPLES];
//   unsigned int digiChannel[MAX_DIGI_SAMPLES];
//   unsigned int digiSampleIndex[MAX_DIGI_SAMPLES];
//          float digiSampleValue[MAX_DIGI_SAMPLES];
// 
//   unsigned int nTdcChannels;
//   unsigned int tdcBoard[MAX_TDC_CHANNELS];
//   unsigned int tdcChannel[MAX_TDC_CHANNELS];
//   unsigned int tdcData[MAX_TDC_CHANNELS];
// };

struct adcData
{
  unsigned int board;
  unsigned int channel;
  unsigned int adcReadout;
};

struct tdcData
{
  unsigned int board;
  unsigned int channel;
  unsigned int tdcReadout;
};

struct digiData
{
  unsigned int group;
  unsigned int channel;
  unsigned int sampleIndex;
         float sampleValue;
};

struct Event
{
  unsigned int evtNumber;
  unsigned int boardTriggerBit;
  std::vector<bool> triggerWord;
  std::vector<adcData> adcValues; 
  std::vector<tdcData> tdcValues; 
  std::vector<digiData> digiValues; 
  unsigned int evtTimeDist;
  unsigned int evtTimeStart;
  unsigned int evtTime;

  int clear ()
    {
      evtNumber = -1 ;
      boardTriggerBit = 0 ;
      triggerWord.clear () ;
      adcValues.clear () ; 
      tdcValues.clear () ; 
      digiValues.clear () ; 
      evtTimeDist = -1 ;
      evtTimeStart = -1 ;
      evtTime = -1 ;
    }


};

#endif
