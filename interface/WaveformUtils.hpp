#ifndef WAVEFORM_UTILS_H
#define WAVEFORM_UTILS_H

#include "interface/Waveform.hpp"
#include "TProfile.h"


namespace WaveformUtils
{
  Waveform generateWaveform(TProfile* ref_profile,float sampleSize,int nSamples, float rms_amplitude_noise);
};

#endif
