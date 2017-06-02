/*  Data format
 *  -----------
 *   First 3 words: header, containing the timestamp
 *     information (i.e. the number of the 160 MHz clocks
 *     from the StartDAQ() command):
 *     timestamp = (t5<<56) + (t4<<42) + (t3<<28) + (t2<<14) + t1;
 *   Subsequent words: _nsamples for each of the 5 VFE channels
 *   Then concatenate header and samples for each VFE adapter
 *   read by the board.
 *
 *           32 ... 16 ...  0 -> bits
 *   words
 *           VFE_adapter_1
 *     1     [10...0][  t1  ] \
 *     2     [  t3  ][  t2  ]  |-> header
 *     3     [  t5  ][  t4  ] /
 *     4     [  --  ][ch0_s0] \
 *     5     [ch2_s0][ch1_s0]  |
 *     6     [ch4_s0][ch3_s0]  |
 *     7     [  --  ][ch0_s1]  |-> samples
 *     8     [ch2_s1][ch1_s1]  |
 *     9     [ch4_s1][ch3_s1]  |
 *    ...       ...    ...    ...
 *           VFE_adapter_N
 *    M+1    [10...0][  t1  ] \
 *    M+2    [  t3  ][  t2  ]  |-> header
 *    ...       ...    ...    ...
 */

#include "interface/VFE_adapter.hpp"
#include <bitset>

int VFE_adapter::Unpack (dataType &stream, Event * event, boardHeader &bH)
{
    uint32_t header;
    stream.read ((char*)&header, sizeof(uint32_t));
    //unsigned int nSamples = (bH.boardSize - 4) /3 ; //move to hpp like dig1742Words_
    unsigned int nSamples = headNSamples(header);
    unsigned int nDevices = headNDevices(header);
    unsigned int freq = headFrequency(header);
    unsigned int timestamp[3];
    stream.read ((char*)timestamp, 3 * sizeof(uint32_t));
    unsigned long int t1 =  timestamp[0]     &0xFFFF;
    unsigned long int t2 =  timestamp[1]     &0xFFFF;
    unsigned long int t3 = (timestamp[1]>>16)&0xFFFF;
    unsigned long int t4 =  timestamp[2]     &0xFFFF;
    unsigned long int t5 = (timestamp[2]>>16)&0x00FF;
    unsigned long int ts = (t5<<56) + (t4<<42) + (t3<<28) + (t2<<14) + t1;
    unsigned short int ch_sample[5]; // 5 channels per VFE
    size_t offset = event->digiValues.size();
    event->digiValues.resize(offset + nSamples * 5);
    unsigned int samples[3]; // first 16 bits are trashed, see the event description above
    for (int idev = 0; idev < nDevices; ++idev) {
        for (int iSample = 0; iSample < nSamples; ++iSample) {
            stream.read ((char*)samples, 3 * sizeof(uint32_t));
            ch_sample[0] = samples[0]     &0xFFFF;
            ch_sample[1] = samples[1]     &0xFFFF;
            ch_sample[2] =(samples[1]>>16)&0xFFFF;
            ch_sample[3] = samples[2]     &0xFFFF;
            ch_sample[4] =(samples[2]>>16)&0xFFFF;
            //fprintf(stderr, "*** sample: %5d    blocks: %8.8x %8.8x %8.8x\n", iSample, samples[0], samples[1], samples[2]);
            //fprintf(stderr, "--> sample: %5d  channels: %8d %8d %8d %8d %8d\n", iSample, ch_sample[0], ch_sample[1], ch_sample[2], ch_sample[3], ch_sample[4]);

            for (int ich = 0; ich < 5; ++ich) {
                size_t pos = offset + (idev * 5 + ich) * nSamples + iSample;
                event->digiValues[pos].board = bH.boardSingleId; // FSM number
                event->digiValues[pos].group = idev; // adapter number
                event->digiValues[pos].channel = ich;
                event->digiValues[pos].frequency = freq;
                event->digiValues[pos].sampleIndex = iSample;
                event->digiValues[pos].sampleValue = ch_sample[ich];

                //std::cout << "VFE_adapter unpack       | sample " << iSample << " channel: "<< ich << " value: " << event->digiValues[pos].sampleValue << "\n" ;

            }
        }
    }
    
    return 0 ;
}
