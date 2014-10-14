#include "interface/Event.hpp"
#define DEBUG_UNPACKER 1

void Event::createOutBranches (TTree* tree,treeStructData& treeData)
{
  //Instantiate the tree branches
  tree->Branch("evtNumber",&treeData.evtNumber,"evtNumber/i");
  tree->Branch("evtTimeDist",&treeData.evtTimeDist,"evtTimeDist/i");
  tree->Branch("evtTimeStart",&treeData.evtTimeStart,"evtTimeStart/i");
  tree->Branch("evtTime",&treeData.evtTime,"evtTime/i");

  tree->Branch("triggerWord",&treeData.triggerWord,"triggerWord/i");

//  tree->Branch("triggerBits",&treeData.triggerBits,"triggerBits/i");

  tree->Branch("nAdcChannels",&treeData.nAdcChannels,"nAdcChannels/i");
  tree->Branch("adcBoard",treeData.adcBoard,"adcBoard[nAdcChannels]/i");
  tree->Branch("adcChannel",treeData.adcChannel,"adcChannel[nAdcChannels]/i");
  tree->Branch("adcData",treeData.adcData,"adcData[nAdcChannels]/i");

  tree->Branch("nTdcChannels",&treeData.nTdcChannels,"nTdcChannels/i");
  tree->Branch("tdcBoard",treeData.tdcBoard,"tdcBoard[nTdcChannels]/i");
  tree->Branch("tdcChannel",treeData.tdcChannel,"tdcChannel[nTdcChannels]/i");
  tree->Branch("tdcData",treeData.tdcData,"tdcData[nTdcChannels]/i");

  tree->Branch("nDigiSamples",&treeData.nDigiSamples,"nDigiSamples/i");
  tree->Branch("digiGroup",treeData.digiGroup,"digiGroup[nDigiSamples]/i");
  tree->Branch("digiChannel",treeData.digiChannel,"digiChannel[nDigiSamples]/i");
  tree->Branch("digiSampleIndex",treeData.digiSampleIndex,"digiSampleIndex[nDigiSamples]/i");
  tree->Branch("digiSampleValue",treeData.digiSampleValue,"digiSample[nDigiSamples]/F");

  tree->Branch("nScalerWords",&treeData.nScalerWords,"nScalerWords/i");
  tree->Branch("scalerWord",treeData.scalerWord,"scalerWord[nScalerWords]/i");

  tree->Branch("nPatterns",&treeData.nPatterns,"nPatterns/i");
  tree->Branch("pattern",treeData.pattern,"pattern[nPatterns]/i");

  tree->Branch("nPatterns_shodo",&treeData.nPatterns_shodo,"nPatterns_shodo/i");
  tree->Branch("pattern_shodo",treeData.pattern_shodo,"pattern_shodo[nPatterns_shodo]/i");


  return ;
} 


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void Event::fillTreeData (treeStructData & treeData)
{
  treeData.evtNumber = evtNumber ;
  //  if (DEBUG_UNPACKER) printf (" =  =  =  =  =  =  FILLING EVENT %d  =  =  =  =  = \n", treeData.evtNumber) ;
  treeData.evtTime = evtTime ;
  treeData.evtTimeStart = evtTimeStart ;
  treeData.evtTimeDist = evtTimeDist ;

  treeData.triggerWord = triggerWord ;
  cout << "[Event][fillTreeData]      | Trigger word: " << treeData.triggerWord << "\n" ;

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
      treeData.digiGroup[i] = digiValues[i].group ;
      treeData.digiChannel[i] = digiValues[i].channel ;
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
      treeData.scalerWord[i] = scalerWords[i] ;
    }

  treeData.nPatterns = patterns.size () ;
  if (DEBUG_UNPACKER)
    {
      cout << "[Event][fillTreeData]      | FILLING " << treeData.nPatterns << " Patterns\n" ;
    }

  for (unsigned int i = 0 ; i < fmin (MAX_PATTERNS, patterns.size()) ;++i)
    {
      treeData.pattern[i] = patterns[i] ;
    }

  treeData.nPatterns_shodo = patterns_shodo.size () ;
  if (DEBUG_UNPACKER)
    {
      cout << "[Event][fillTreeData]      | FILLING " << treeData.nPatterns_shodo << " Patterns\n" ;
    }

  for (unsigned int i = 0 ; i < fmin (MAX_PATTERNS_SHODO, patterns_shodo.size()) ;++i)
    {
      treeData.pattern_shodo[i] = patterns_shodo[i] ;
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
  evtNumber = -1 ;
  triggerWord = 0 ;
//  triggerBits.clear () ;
  adcValues.clear () ; 
  tdcValues.clear () ; 
  digiValues.clear () ; 
  scalerWords.clear () ; 
  patterns.clear () ; 
  patterns_shodo.clear () ; 
  evtTimeDist = -1 ;
  evtTimeStart = -1 ;
  evtTime = -1 ;
}
