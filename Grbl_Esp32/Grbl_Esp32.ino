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
#include "src/custom/async.cpp"

void setup() {
    Serial2.begin(115200);
    console.addConsole([&](unsigned char *cosnoleData,unsigned char autoNLCR){
      if(autoNLCR)
        Serial2.println((char*)cosnoleData);
      else
        Serial2.print((char*)cosnoleData);
    });

    console.log("code just started");

    async task1([&](void){
      vTaskDelay(1000);
      console.log("task 1 log");
    });

    
    async task2([&](void){
      vTaskDelay(500);
      console.log("task 2 log");
    });
    vTaskDelay(10000);

    grbl_init();
}

void loop() {
    run_once();
}
