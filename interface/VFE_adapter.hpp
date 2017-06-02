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

        uint16_t headNSamples(uint32_t header)  { return  header & 0x3FFF;    }
        uint16_t headFrequency(uint32_t header) { return 40 * (((header>>14) & 0x3) + 1);  }
        uint16_t headNDevices(uint32_t header)  { return (header>>16) & 0x7;  }
        uint16_t headFwVersion(uint32_t header) { return (header>>19) & 0x1F; }
};

#endif
