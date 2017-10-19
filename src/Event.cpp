#include "interface/Event.hpp"
#include <assert.h>
#define DEBUG_UNPACKER 0

void Event::createOutBranches (TTree* tree,treeStructData& treeData)
{
  //Instantiate the tree branches
  tree->Branch("runNumber"	,&treeData.runNumber,"runNumber/i");
  tree->Branch("spillNumber"	,&treeData.spillNumber,"spillNumber/i");
  tree->Branch("evtNumber"	,&treeData.evtNumber,"evtNumber/i");
  tree->Branch("evtTimeDist"	,&treeData.evtTimeDist,	"evtTimeDist/i");
  tree->Branch("evtTimeStart"	,&treeData.evtTimeStart,"evtTimeStart/i");

  tree->Branch("nEvtTimes"	,&treeData.nEvtTimes,	"nEvtTimes/i"); // l-> ULong64_t
  tree->Branch("evtTime"	,treeData.evtTime,	"evtTime[nEvtTimes]/l"); // l-> ULong64_t
  tree->Branch("evtTimeBoard"	,treeData.evtTimeBoard,	"evtTimeBoard[nEvtTimes]/i"); // l-> ULong64_t

//  tree->Branch("triggerBits",&treeData.triggerBits,"triggerBits/i");

  tree->Branch("nAdcChannels"	,&treeData.nAdcChannels,"nAdcChannels/i");
  tree->Branch("adcBoard"	,treeData.adcBoard,"adcBoard[nAdcChannels]/i");
  tree->Branch("adcChannel"	,treeData.adcChannel,"adcChannel[nAdcChannels]/i");
  tree->Branch("adcData"	,treeData.adcData,"adcData[nAdcChannels]/i");

  tree->Branch("nTdcChannels"	,&treeData.nTdcChannels,"nTdcChannels/i");
  tree->Branch("tdcBoard"	,treeData.tdcBoard,"tdcBoard[nTdcChannels]/i");
  tree->Branch("tdcChannel"	,treeData.tdcChannel,"tdcChannel[nTdcChannels]/i");
  tree->Branch("tdcData"	,treeData.tdcData,"tdcData[nTdcChannels]/i");

  tree->Branch("nDigiSamples"	,&treeData.nDigiSamples,"nDigiSamples/i");
  tree->Branch("digiFrequency"	,treeData.digiFrequency,"digiFrequency[nDigiSamples]/i");
  tree->Branch("digiGroup"	,treeData.digiGroup,"digiGroup[nDigiSamples]/i");
  tree->Branch("digiChannel"	,treeData.digiChannel,"digiChannel[nDigiSamples]/i");
  tree->Branch("digiSampleIndex",treeData.digiSampleIndex,"digiSampleIndex[nDigiSamples]/i");
  tree->Branch("digiSampleValue",treeData.digiSampleValue,"digiSample[nDigiSamples]/s");
  tree->Branch("digiBoard"	,treeData.digiBoard,"digiBoard[nDigiSamples]/i");

  tree->Branch("nScalerWords"	,&treeData.nScalerWords,"nScalerWords/i");
  tree->Branch("scalerWord"	,treeData.scalerWord,	"scalerWord[nScalerWords]/i");
  tree->Branch("scalerBoard"	,treeData.scalerBoard,	"scalerBoard[nScalerWords]/i"); // size is Words

  tree->Branch("nPatterns"	,&treeData.nPatterns,"nPatterns/i");
  tree->Branch("pattern"	,treeData.pattern,"pattern[nPatterns]/i");
  tree->Branch("patternBoard"	,treeData.patternBoard,"patternBoard[nPatterns]/i");
  tree->Branch("patternChannel"	,treeData.patternChannel,"patternChannel[nPatterns]/i");

  tree->Branch("nTriggerWords"	,&treeData.nTriggerWords,"nTriggerWords/i");
  tree->Branch("triggerWords"	,treeData.triggerWords,"triggerWords[nTriggerWords]/i");
  tree->Branch("triggerWordsBoard",treeData.triggerWordsBoard,"triggerWordsBoard[nTriggerWords]/i");

  return ;
} 


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void Event::fillTreeData (treeStructData & treeData)
{
  treeData.runNumber = id.runNumber ;
  treeData.spillNumber = id.spillNumber ;
  treeData.evtNumber = id.evtNumber ;
  //  if (DEBUG_UNPACKER) printf (" =  =  =  =  =  =  FILLING EVENT %d  =  =  =  =  = \n", treeData.evtNumber) ;
  treeData.nEvtTimes = evtTimes.size () ;
  if (DEBUG_UNPACKER)
    {
      cout << "[Event][fillTreeData]      | FILLING " << treeData.nEvtTimes << " Times Values\n" ;
    }
  for (unsigned int i = 0 ;i<fmin (MAX_RO, evtTimes.size ()) ;++i)
    {
      treeData.evtTimeBoard[i] =  evtTimes[i].board;
      treeData.evtTime[i] =  evtTimes[i].time;
    }


  treeData.evtTimeStart = evtTimeStart ;
  treeData.evtTimeDist = evtTimeDist ;

  //treeData.triggerWord = triggerWord ;
  //cout << "[Event][fillTreeData]      | Trigger word: " << treeData.triggerWord << "\n" ;

//  treeData.triggerBits = 0 ;
//  for (unsigned int i = 0 ; i<fmin (32, triggerBits.size ()) ;++i)
//    treeData.triggerBits += triggerBits[i]>>i ;

  treeData.nAdcChannels = adcValues.size () ;
  if (DEBUG_UNPACKER)
    {
      cout << "[Event][fillTreeData]      | FILLING " << treeData.nAdcChannels << " ADC values\n" ;
    }
  for (unsigned int i = 0 ;i<fmin (MAX_ADC_CHANNELS, adcValues.size ()) ;++i)
    {
      treeData.adcBoard[i] = adcValues[i].board ;
      treeData.adcChannel[i] = adcValues[i].channel ;
      treeData.adcData[i] = adcValues[i].adcReadout ;
    }

  treeData.nTdcChannels = tdcValues.size () ;
  if (DEBUG_UNPACKER)
    {
      cout << "[Event][fillTreeData]      | FILLING " << treeData.nTdcChannels << " TDC values\n" ;
    }
  for (unsigned int i = 0 ;i<fmin (MAX_TDC_CHANNELS, tdcValues.size ()) ;++i)
    {
      treeData.tdcBoard[i] = tdcValues[i].board ;
      treeData.tdcChannel[i] = tdcValues[i].channel ;
      treeData.tdcData[i] = tdcValues[i].tdcReadout ;
    }

  treeData.nDigiSamples = digiValues.size () ;
  if (DEBUG_UNPACKER)
    {
      cout << "[Event][fillTreeData]      | FILLING " << treeData.nDigiSamples << " DIGI values\n" ;
    }

  for (unsigned int i = 0 ; i < fmin (MAX_DIGI_SAMPLES, digiValues.size ()) ;++i)
    {
      treeData.digiFrequency[i] = digiValues[i].frequency ;
      treeData.digiGroup[i] = digiValues[i].group ;
      treeData.digiChannel[i] = digiValues[i].channel ;
      treeData.digiBoard[i] = digiValues[i].board ;
      treeData.digiSampleIndex[i] = digiValues[i].sampleIndex ;
      treeData.digiSampleValue[i] = digiValues[i].sampleValue ;
    }

  treeData.nScalerWords = scalerWords.size () ;
  if (DEBUG_UNPACKER)
    {
      cout << "[Event][fillTreeData]      | FILLING " << treeData.nScalerWords << " Scaler words\n" ;
    }

  for (unsigned int i = 0 ; i < fmin (MAX_SCALER_WORDS, scalerWords.size()) ;++i)
    {
      treeData.scalerWord[i] = scalerWords[i].scalerValue ;
      treeData.scalerBoard[i] = scalerWords[i].board ;
    }

  treeData.nPatterns = patterns.size () ;
  if (DEBUG_UNPACKER)
    {
      cout << "[Event][fillTreeData]      | FILLING " << treeData.nPatterns << " Patterns\n" ;
    }

  for (unsigned int i = 0 ; i < fmin (MAX_PATTERNS, patterns.size()) ;++i)
    {
      treeData.pattern[i] 	 = patterns[i].patternValue ;
      treeData.patternBoard[i] 	 = patterns[i].board ;
      treeData.patternChannel[i] = patterns[i].channel ;
    }

  treeData.nTriggerWords = triggerWords.size () ;
  if (DEBUG_UNPACKER)
    {
      cout << "[Event][fillTreeData]      | FILLING " << treeData.nTriggerWords << " Trigger words\n" ;
      assert(scalerWords.size()<= MAX_TRIG_WORDS);
    }

  for (unsigned int i = 0 ; i < fmin (MAX_TRIG_WORDS, triggerWords.size()) ;++i)
    {
      treeData.triggerWords[i] 		= triggerWords[i].triggerWord ;
      treeData.triggerWordsBoard[i] 	= triggerWords[i].board ;
    }

  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void Event::Fill ()
{
  fillTreeData (thisTreeEvent_) ;
  outTree_->Fill () ;
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void Event::clear ()
{
  id.runNumber = 0 ; // this number is unsigned
  id.spillNumber = 0 ; // this number is unsigned
  id.evtNumber = 0 ; // this number is unsigned
  triggerWords.clear() ;
//  triggerBits.clear () ;
  adcValues.clear () ; 
  tdcValues.clear () ; 
  digiValues.clear () ; 
  scalerWords.clear () ; 
  patterns.clear () ; 
  evtTimeDist = 0 ; // this number is unsigned
  evtTimeStart = 0 ; // this number is unsigned
  evtTimes.clear() ; // these numbers are unsigned
}
