#include <stdint.h>
#include <Arduino.h>
#include <cstdlib>
#include <functional>

class async{
    public:
        std::function<void(void)>threadCallback;
        void thread(void* arg){
            threadCallback();
            vTaskDelete(NULL);
        }
        async(const std::function<void(void)>Callback,unsigned short stackSize=10000,unsigned char priority=1){
            threadCallback=Callback;
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
