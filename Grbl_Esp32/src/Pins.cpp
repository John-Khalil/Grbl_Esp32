#include <Arduino.h>

#include "Grbl.h"
#include "I2SOut.h"


#include "custom/consoleLogger.h"
#include <functional>
#include "custom/async.cpp"


String pinName(uint8_t pin) {
    if (pin == UNDEFINED_PIN) {
        return "None";
    }
    if (pin < I2S_OUT_PIN_BASE) {
        return String("GPIO(") + pin + ")";
    } else {
        return String("I2SO(") + (pin - I2S_OUT_PIN_BASE) + ")";
    }
}

// Even if USE_I2S_OUT is not defined, it is necessary to
// override the following functions, instead of allowing
// the weak aliases in the library to apply, because of
// the UNDEFINED_PIN check.  That UNDEFINED_PIN behavior
// cleans up other code by eliminating ifdefs and checks.
void IRAM_ATTR digitalWrite(uint8_t pin, uint8_t val) {
    static unsigned long time;

    static unsigned long callCount;

    // async({
    //     // time=micros()-time;
    //     console.log(pin,"-",micros());

    // });
    
    vTaskDelay(100);        //! manually throttling the main thread this really expalins the weird behaviour 

    // task 1
    async({                 //! this thread would luch normally after manually introducing the bottle-neck cause it gives it enough time to terminate
        time++;
        // console.log(micros());
    });

    // task 2
    async(                  //! this would lunch only once, showing the same thread will only lunch after the old instance is terminated 
        console.log("feedBack");
        vTaskDelay(5000);
        console.log("final value ->> ",time);
    );
    //^ both task 1 & 2 should be lunched the exact numebr of time as the function gets called (some one trying to change any output pin state)

    console.log(time,"-",micros()," -- ",callCount++);
    /*
        ^ solution
            * we should keep the output cached in a list
            * then lunch a thread that would output to pins
            * this thread wont terminate unless it go through every state in the list cause lunching the thread takes time

    */

    if (pin == UNDEFINED_PIN) {
        return;
    }
    if (pin < I2S_OUT_PIN_BASE) {
        __digitalWrite(pin, val);
        return;
    }
#ifdef USE_I2S_OUT
    i2s_out_write(pin - I2S_OUT_PIN_BASE, val);
#endif
}

void IRAM_ATTR pinMode(uint8_t pin, uint8_t mode) {
    // console.log("Pin >> ",(unsigned long)pin," -- ",(unsigned long)mode);
    if (pin == UNDEFINED_PIN) {
        return;
    }
    if (pin < I2S_OUT_PIN_BASE) {
        __pinMode(pin, mode);
    }
    // I2S out pins cannot be configured, hence there
    // is nothing to do here for them.
}

int IRAM_ATTR digitalRead(uint8_t pin) {
    if (pin == UNDEFINED_PIN) {
        return 0;
    }
    if (pin < I2S_OUT_PIN_BASE) {
        return __digitalRead(pin);
    }
#ifdef USE_I2S_OUT
    return i2s_out_read(pin - I2S_OUT_PIN_BASE);
#else
    return 0;
#endif
}
