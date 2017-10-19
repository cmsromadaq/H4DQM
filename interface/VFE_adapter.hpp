#ifndef VFE_ADAPTER_H
#define VFE_ADAPTER_H

#include "interface/StandardIncludes.hpp"
#include "interface/Board.hpp"
#include "interface/Event.hpp"

class VFE_adapter: public Board
{

    public :

        VFE_adapter() {};

        int Unpack(dataType &stream) { return -1; }
        int Unpack(dataType &stream, Event * event, boardHeader&);

        int headNSamples(uint32_t header)  { return  header & 0x3FFF;    }
        int headFrequency(uint32_t header) { return 40 * (((header>>14) & 0x3) + 1);  }
        int headNDevices(uint32_t header)  { return (header>>16) & 0x7;  }
        int headFwVersion(uint32_t header) { return (header>>19) & 0x1F; }
};

#endif
