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
#include "src/custom/webService.h"

#include "src/custom/pointerTool.h"


// #include "src/custom/consoleLogger.h"
// #include "src/custom/consoleLogger.cpp"
// #include "src/custom/highLevelMemory.cpp"
// #include "src/custom/pointerTool.h"
// #include "src/custom/constDefinitions.h"
// #include "src/custom/eepromBasicConfig.h"
// #include "src/custom/networkUtils.h"
// #include "src/custom/platform.h"
// #include "src/custom/http.h"
// #include "src/custom/webService.h"
// #include"src/custom/fetch.h"

#define STATUS_LABEL  "statusLabel"
#define INPUT_VALUE   "INPUT_VALUE"
#define RETURN_DATA   "returnData"
#define OPERATOR      "operator"
#define ID            "ID"
#define ACK           "ack"

#define THREAD_ACK            "THREAD_ACK"
#define INPUT_VALUE           "INPUT_VALUE"
#define OUTPUT_ACK            "OUTPUT_ACK"
#define MOTIONCONTROLLER_ACK  "MOTIONCONTROLLER_ACK"



uint8_t *addToObject(uint8_t* userObjectStr,std::string newKey,std::string newValue){
  std::string userObject=(char*)userObjectStr;
  userObject[userObject.length()-1]=',';    // replace '}' with ',' 0x2c
  return (uint8_t*)(userObject+="\""+newKey+"\""+":"+"\""+newValue+"\"}").c_str();
}

#define $JSON(REQUESTED_JSON,JSON_STRING) std::string((char*)constJson((uint8_t*)REQUESTED_JSON,(uint8_t*)JSON_STRING))



//^ //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "src/custom/highLevelMemory.cpp"

utils::highLevelMemory MEMORY(4096);

#define INPUT_DEVICE        "inputDevice"
#define OUTPUT_DEVICE       "outputDevice"
#define MOTION_CONTROLLER   "motionController"
#define THREAD              "thread"

#define PIN_MODE          "pinMode"
#define INPUT_REGISTER_0  "inputRegister0"
#define INPUT_REGISTER_1  "inputRegister1"
#define OUTPUT_REGISTER_0 "outputRegister0"
#define OUTPUT_REGISTER_1 "outputRegister1"

#define ANALOG_INPUT      "analogInput"
#define DIGITAL_INPUT     "digitalInput"
#define DIGITAL_OUTPUT    "digitalOutput"
#define SERVO_CONTROL     "servoControl"
#define CLOCK_OUTPUT      "clockOutput"

#define EXTENDED_OUTPUT   "extendedOutput"

#define EXECUTABLE_OBJECT "executableObject"

void operatorCallbackSetup(void){
  // constJson();

  MEMORY["test0"]>>[&](uint8_t* data){
    MEMORY[EXECUTABLE_OBJECT]=addToObject((uint8_t*)MEMORY[EXECUTABLE_OBJECT],"testOutput","thats awsome");
  };
  MEMORY["test0"]<<[&](){
    MEMORY["test0"]="manga";
  };
  return;
}


//^ //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void setup() {
    Serial2.begin(500000);
    console.addConsole([&](unsigned char *cosnoleData,unsigned char autoNLCR){
      if(autoNLCR)
        Serial2.println((char*)cosnoleData);
      else
        Serial2.print((char*)cosnoleData);
    });

    console.log("code just started");
    operatorCallbackSetup();

    #define shiftRegisterClkPin   12
    #define shiftRegisterDataPin  27
    #define shiftRegisterLatchPin 14


    _PM(shiftRegisterClkPin,OUTPUT);
    _PM(shiftRegisterDataPin,OUTPUT);
    _PM(shiftRegisterLatchPin,OUTPUT);

    // spiPort.passThrough([&](uint16_t pinNumber,uint8_t pinState,uint64_t outputValue){
    //   console.log(">> ",pinNumber,">> ",pinState,">> ",(uint16_t)outputValue);
    // });

    // spiPort.setup([&](uint8_t clkPin){
    //   if(clkPin)
    //     outputRegisterLowSet|=(1<<shiftRegisterClkPin);
    //   else
    //     outputRegisterLowClear|=(1<<shiftRegisterClkPin);
    // },
    // [&](uint8_t dataPin){
    //   if(dataPin)
    //     outputRegisterLowSet|=(1<<shiftRegisterDataPin);
    //   else
    //     outputRegisterLowClear|=(1<<shiftRegisterDataPin);
    // },
    // [&](uint8_t latchPin){
    //   if(latchPin)
    //     outputRegisterLowSet|=(1<<shiftRegisterLatchPin);
    //   else
    //     outputRegisterLowClear|=(1<<shiftRegisterLatchPin);
    // },
    // [&](float microSec){
      
    // });

    spiPort.passThrough([&](uint16_t pinNumber,uint8_t pinState){
        uint8_t mainStepper=0;
        uint8_t followerStepper=1;
        spiPort.outputValue&=~(0x3f<<(6*followerStepper));
        // spiPort.outputValue|=(spiPort.outputValue<<((followerStepper-mainStepper)*6))&(0x3f<<(6*followerStepper));
        
        spiPort.outputValue|=(mainStepper>followerStepper)?(spiPort.outputValue&(0x3f<<(6*mainStepper)))>>((mainStepper-followerStepper)*6):(spiPort.outputValue&(0x3f<<(6*mainStepper)))<<((followerStepper-mainStepper)*6);
        // spiPort.outputValue&=~(1<<24);
    });

    spiPort.setup(
      [&](uint64_t outputValue,uint8_t portSize){
        uint8_t loopCounter=portSize;
        while(loopCounter--){
          // dataPin((outputValue>>loopCounter)&0x01);
          (((outputValue>>loopCounter)&0x01)?outputRegisterLowSet:outputRegisterLowClear)|=(1<<shiftRegisterDataPin);
          // if(((outputValue>>loopCounter)&0x01))
          //   outputRegisterLowSet|=(1<<shiftRegisterDataPin);
          // else
          //   outputRegisterLowClear|=(1<<shiftRegisterDataPin);

          outputRegisterLowSet|=(1<<shiftRegisterClkPin);
          outputRegisterLowClear|=(1<<shiftRegisterClkPin);
        }
      },
      [&](uint8_t latchPin){
        if(latchPin)
          outputRegisterLowSet|=(1<<shiftRegisterLatchPin);
        else
          outputRegisterLowClear|=(1<<shiftRegisterLatchPin);
      },
      [&](float microSec){
        
      }
    );
    spiPort|=((1<<3)|(1<<9)|(1<<15)|(1<<20)|(1<<26));   // setting the micro stepping to quarter step

    async({
      vTaskDelay(15000);
      web::service webServer(90,"/");
      webServer.onData([&](uint8_t *data){
        MEMORY["test"]="random value";
        MEMORY[EXECUTABLE_OBJECT]=addToObject(data,STATUS_LABEL,"ok");

        if($JSON(OPERATOR,MEMORY[EXECUTABLE_OBJECT])==INPUT_DEVICE){
          if($JSON(ACK,MEMORY[EXECUTABLE_OBJECT])=="undefined")
            MEMORY[EXECUTABLE_OBJECT]=addToObject((uint8_t*)MEMORY[EXECUTABLE_OBJECT],ACK,OUTPUT_ACK);
          
          MEMORY[$JSON(ID,MEMORY[EXECUTABLE_OBJECT])]|="UNDEFINED";
          MEMORY[EXECUTABLE_OBJECT]=addToObject(MEMORY[EXECUTABLE_OBJECT],INPUT_VALUE,(MEMORY[$JSON(ID,MEMORY[EXECUTABLE_OBJECT])]));
        }
        else if($JSON(OPERATOR,MEMORY[EXECUTABLE_OBJECT])==OUTPUT_DEVICE){
          if($JSON(ACK,MEMORY[EXECUTABLE_OBJECT])=="undefined")
            MEMORY[EXECUTABLE_OBJECT]=addToObject((uint8_t*)MEMORY[EXECUTABLE_OBJECT],ACK,OUTPUT_ACK);
          
          MEMORY[$JSON(ID,MEMORY[EXECUTABLE_OBJECT])]=MEMORY[EXECUTABLE_OBJECT];
        }


        webServer.send((uint8_t*)MEMORY[EXECUTABLE_OBJECT]);
        webServer.httpSetResponse((uint8_t*)MEMORY[EXECUTABLE_OBJECT]);
      });
      vTaskDelay(-1UL);
      // for(;;)vTaskDelay(1500000);
    });







    // spiPort|=((1<<4)|(1<<10)|(1<<16)|(1<<19)|(1<<25));      // setting the micro stepping to half step
    
    // spiPort.write((1<<3)|(1<<9)|(1<<15)|(1<<20)|(1<<24)|(1<<26)|(1<<4)|(1<<10)|(1<<16)|(1<<19)|(1<<25));   // setting the micro stepping to quarter step

    // async({
    //   uint8_t blinker=0;
    //   while(1){
    //     spiPort.write(spiPort.outputValue^255);
    //     spiPort.write(15,blinker^=255);
    //     vTaskDelay(500);
    //   }
    // });

    // async({
    //   uint8_t blinker=0;
    //   while(1){
    //     #define validationCount 2000
    //     uint16_t loopCounter=validationCount;
    //     uint16_t time=millis();
    //     while(loopCounter--){
    //       // spiPort.write(spiPort.outputValue^65535);
    //       spiPort.write(15,blinker^=255);
    //     }
    //     time=millis()-time;
    //     console.log("time >> ",(uint16_t)time);
    //     vTaskDelay(1500);
    //   }
      
    // });

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
