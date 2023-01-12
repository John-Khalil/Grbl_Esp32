#ifndef extendedPort_h
#define extendedPort_h

#include <stdint.h>
#include <cstdlib>
#include <functional>
#include <vector>


class extendedPort{
    private:
        std::function<void(unsigned char)>clkPin;
        std::function<void(unsigned char)>dataPin;
        std::function<void(unsigned char)>latchPin;
        std::function<void(float)>delay_us;
        std::vector<std::function<void(uint16_t,uint8_t,uint64_t)>>outputPassthrough;

        uint32_t clockSpeed=0;
        uint8_t portSize=0;
        uint64_t outputValue=0;
    public:
        

        void setup(
            const std::function<void(unsigned char)>&_clkPin,
            const std::function<void(unsigned char)>&_dataPin,
            const std::function<void(unsigned char)>&_latchPin,
            const std::function<void(float)>&_delay_us,
            uint32_t _clockSpeed=1000000,
            uint8_t _portSize=32,
            uint64_t _outputValue=0
        ){
            clkPin=_clkPin;
            dataPin=_dataPin;
            latchPin=_latchPin;
            delay_us=_delay_us;
            clockSpeed=_clockSpeed;
            portSize=_portSize;
            outputValue=_outputValue;
        }
};

extern extendedPort spiPort;

#endif