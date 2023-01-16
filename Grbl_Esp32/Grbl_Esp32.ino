/*
  Grbl_ESP32.ino - Header for system level commands and real-time processes
  Part of Grbl
  Copyright (c) 2014-2016 Sungeun K. Jeon for Gnea Research LLC

	2018 -	Bart Dring This file was modified for use on the ESP32
					CPU. Do not use this with Grbl for atMega328P

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "src/Grbl.h"
#include "src/custom/consoleLogger.h"
#include <functional>
#include "src/custom/async.cpp"
#include "src/custom/extendedPort.h"
#include "src/custom/esp32basics.h"
#include "src/Pins.h"


void setup() {
    Serial2.begin(500000);
    console.addConsole([&](unsigned char *cosnoleData,unsigned char autoNLCR){
      if(autoNLCR)
        Serial2.println((char*)cosnoleData);
      else
        Serial2.print((char*)cosnoleData);
    });

    console.log("code just started");

    #define shiftRegisterClkPin 27
    #define shiftRegisterDataPin 14
    #define shiftRegisterLatchPin 12


    _PM(shiftRegisterClkPin,OUTPUT);
    _PM(shiftRegisterDataPin,OUTPUT);
    _PM(shiftRegisterLatchPin,OUTPUT);

    spiPort.setup([&](uint8_t clkPin){
      if(clkPin)
        outputRegisterLowSet|=(1<<shiftRegisterClkPin);
      else
        outputRegisterLowClear|=(1<<shiftRegisterClkPin);
    },
    [&](uint8_t dataPin){
      if(dataPin)
        outputRegisterLowSet|=(1<<shiftRegisterDataPin);
      else
        outputRegisterLowClear|=(1<<shiftRegisterDataPin);
    },
    [&](uint8_t latchPin){
      if(latchPin)
        outputRegisterLowSet|=(1<<shiftRegisterLatchPin);
      else
        outputRegisterLowClear|=(1<<shiftRegisterLatchPin);
    },
    [&](float microSec){
      
    });

    // async({
    //   uint8_t blinker=0;
    //   while(1){
    //     spiPort.write(spiPort.outputValue^255);
    //     spiPort.write(15,blinker^=255);
    //     vTaskDelay(500);
    //   }
    // });

    async({
      uint8_t blinker=0;
      while(1){
        #define validationCount 2000
        uint16_t loopCounter=validationCount;
        uint16_t time=millis();
        while(loopCounter--){
          // spiPort.write(spiPort.outputValue^65535);
          spiPort.write(15,blinker^=255);
        }
        time=millis()-time;
        console.log("time >> ",(uint16_t)time);
        vTaskDelay(500);
      }
      
    });

    // async({
    //   while(1){
    //     vTaskDelay(1500);
    //     console.log("task 1 log");
    //   }
      
    // });

    // async(
    //   vTaskDelay(1000);
    //   console.log("task 2 log");
    // );

    

    grbl_init();

    // console.log("setup function exit");
}

void loop() {
    run_once();
}
