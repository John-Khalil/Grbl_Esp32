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

        float delayTime=0;
        float halfDelayTime=0;
    public:
        extendedPort &write(void){
            uint8_t loopCounter=portSize;
            while(loopCounter--){
                dataPin((outputValue>>loopCounter)&0x01);
                delay_us(halfDelayTime);
                clkPin(1);
                delay_us(delayTime);
                clkPin(0);
                delay_us(halfDelayTime);
            }
            latchPin(1);
            delay_us(delayTime);

            return *this;
        }

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
            delayTime=1000000.0/(float)clockSpeed;
            halfDelayTime=delayTime/2.0;

            write();
        }
};

extern extendedPort spiPort;

#endif