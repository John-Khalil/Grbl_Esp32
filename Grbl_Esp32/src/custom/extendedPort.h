#ifndef extendedPort_h
#define extendedPort_h

#include <stdint.h>
#include <cstdlib>
#include <functional>
#include <vector>
#include "consoleLogger.h"


class extendedPort{
    private:
        /* callback functions */                            //! this needs to be initialized first
        //~ callback function that control each pin
        std::function<void(unsigned char)>clkPin;
        std::function<void(unsigned char)>dataPin;
        std::function<void(unsigned char)>latchPin;
        //~ callback for the system delay this could vary depending on the implementation from one system to another
        std::function<void(float)>delay_us;
        //~ external spi , that could be useful for optimizations or using HARDWARE SPI to make it even faster
        std::function<void(uint64_t,uint8_t)>externalSpiPort=nullptr;
        //~ callback list for the output passthrough , this could be useful for debugging or reading or modifying the output value
        std::vector<std::function<void(uint16_t,uint8_t)>>outputPassthrough;

        

        float delayTime=0;
        float halfDelayTime=0;

        uint16_t pinNumber=-1;
        uint8_t pinState=-1;
    public:
        //^ those members need to be public to be easily manipulated
        uint32_t clockSpeed=0;
        uint8_t portSize=0;
        uint64_t outputValue=0;

        extendedPort &setClockSpeed(uint32_t _clockSpeed){
            clockSpeed=_clockSpeed;
            delayTime=1000000.0/(float)clockSpeed;
            halfDelayTime=delayTime/2.0;
            return *this;
        }

        /*
            ~ thare are 3 ways to manipulate the shift register (74HC595) introduced in the following overloadded instances 
            ? Datasheet   https://www.ti.com/lit/ds/symlink/sn74hc595.pdf?ts=1674911460797&ref_url=https%253A%252F%252Fwww.google.com%252F
            * write the output values directly from the register (outputValue)
            * update the register first then write the output
            * assign each pin value 
        */


        extendedPort &write(void){
            
            for(auto &passThroughCallback:outputPassthrough)
                passThroughCallback(pinNumber,pinState);
                
            if(externalSpiPort!=nullptr)
                externalSpiPort(outputValue,portSize);
            else{
                uint8_t loopCounter=portSize;
                while(loopCounter--){
                    dataPin((outputValue>>loopCounter)&0x01);
                    delay_us(delayTime);
                    clkPin(1);
                    delay_us(delayTime);
                    clkPin(0);
                    // delay_us(halfDelayTime);
                }
            }
        
            latchPin(1);
            delay_us(delayTime);
            latchPin(0);

            pinNumber=-1;
            pinState=-1;
            return *this;
        }

        extendedPort &write(uint64_t _outputValue){
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

        //~ push a new passthorugh callback

        extendedPort &passThrough(const std::function<void(uint16_t,uint8_t)>passThroughCallback){
            outputPassthrough.push_back(passThroughCallback);
            return *this;
        }

        /*
            ~ setup methodes , either in normal software SPI mode or external SPI callback function
            ^NOTE: the external SPI callback function would OVER-RIDE
        */

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

        void setup(
            const std::function<void(uint64_t,uint8_t)>&_externalSpiPort,
            const std::function<void(unsigned char)>&_latchPin,
            const std::function<void(float)>&_delay_us,
            uint32_t _clockSpeed=1000000,
            uint8_t _portSize=32,
            uint64_t _outputValue=0
        ){
            externalSpiPort=_externalSpiPort;
            latchPin=_latchPin;
            delay_us=_delay_us;

            portSize=_portSize;
            outputValue=_outputValue;

            clockSpeed=_clockSpeed;
            delayTime=1000000.0/(float)clockSpeed;
            halfDelayTime=delayTime/2.0;

            write();
        }

        //^ overloaded operators

        operator uint64_t(){
            return outputValue;
        }

        operator int64_t(){
            return (int64_t)outputValue;
        }

        extendedPort &operator=(uint64_t _outputValue){
            outputValue=_outputValue;
            return write();
        }

        extendedPort &operator|=(uint64_t _outputValue){
            outputValue|=_outputValue;
            return write();
        }

        extendedPort &operator&=(uint64_t _outputValue){
            outputValue&=_outputValue;
            return write();
        }

};

extern extendedPort spiPort;

#endif