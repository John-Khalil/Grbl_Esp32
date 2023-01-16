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

        

        float delayTime=0;
        float halfDelayTime=0;

        uint16_t pinNumber=-1;
        uint8_t pinState=-1;
    public:
        uint32_t clockSpeed=0;
        uint8_t portSize=0;
        uint32_t outputValue=0;

        extendedPort &setClockSpeed(uint32_t _clockSpeed){
            clockSpeed=_clockSpeed;
            delayTime=1000000.0/(float)clockSpeed;
            halfDelayTime=delayTime/2.0;
            return *this;
        }




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
            latchPin(0);

            unsigned char passThroughCounter=outputPassthrough.size();
            while(passThroughCounter--)
                outputPassthrough[passThroughCounter](pinNumber,pinState,outputValue);

            pinNumber=-1;
            pinState=-1;
            return *this;
        }

        extendedPort &write(uint32_t _outputValue){
            outputValue=_outputValue;
            return write();
        }

        extendedPort &write(uint16_t _pinNumber,uint8_t _pinState){
            pinNumber=_pinNumber;
            pinState=_pinState;
            if(pinState)
                outputValue|=(1<<_pinNumber);
            else
                outputValue&=~(1<<_pinNumber);
            return write();
        }

        extendedPort &passThrough(const std::function<void(uint16_t,uint8_t,uint64_t)>passThroughCallback){
            outputPassthrough.push_back(passThroughCallback);
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

            portSize=_portSize;
            outputValue=_outputValue;

            clockSpeed=_clockSpeed;
            delayTime=1000000.0/(float)clockSpeed;
            halfDelayTime=delayTime/2.0;

            write();
        }
};

extern extendedPort spiPort;

#endif