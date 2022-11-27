#include <stdint.h>
#include <Arduino.h>
#include <cstdlib>
#include <functional>
#include "consoleLogger.h"

class async{
    public:
        std::function<void(void)>threadCallback=[&](){
            console.log("this should not appear");
        };
        void thread(void* arg){
            // async::threadCallback();
            console.log("test from inside class");
            vTaskDelete(NULL);
        }
        async(std::function<void(void)>Callback,unsigned short stackSize=10000,unsigned char priority=3){
            // threadCallback=Callback;
            // threadCallback();
            xTaskCreate(
                (void (*)(void*))&async::thread,    // Function that should be called
                "async-class-task",                 // Name of the task (for debugging)       //! we should get instance name or identifier 
                stackSize,                          // Stack size (bytes)
                NULL,                               // Parameter to pass
                priority,                           // Task priority
                NULL                                // Task handle
            );
        }

        ~async(){
            
        }
};
