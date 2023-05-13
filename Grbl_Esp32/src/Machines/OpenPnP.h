#pragma once
// clang-format off

/*
    6_pack_external_XYZ.h

    Covers all V1 versions V1p0, V1p1, etc

    Part of Grbl_ESP32
    Pin assignments for the ESP32 I2S 6-axis board

    2021    - Bart Dring
    
    Grbl_ESP32 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Grbl is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Grbl_ESP32.  If not, see <http://www.gnu.org/licenses/>.


    6 Pack Jumpers for External Drivers
    The only jumper you set is the Vcc on 5V
    Stallguard jumpers must not be connected
    MS/SPI Do not need to be installed. It is OK to put them oonm the MS side
    TMC5160 Is does not matter if this is installed or not on any side.


*/
#define MACHINE_NAME            "6 Pack External XYZ"


#define N_AXIS 6

// === Special Features

// I2S (steppers & other output-only pins)
#define USE_I2S_OUT
#define USE_I2S_STEPS
//#define DEFAULT_STEPPER ST_I2S_STATIC

#define I2S_OUT_BCK             GPIO_NUM_4//GPIO_NUM_22
#define I2S_OUT_WS              GPIO_NUM_4//GPIO_NUM_4
#define I2S_OUT_DATA            GPIO_NUM_4//GPIO_NUM_21

// #define CUSTOM_CODE_FILENAME    "../Custom/CoreXY.cpp"

// Motor Socket #1
#define X_STEP_PIN              I2SO(1)
#define X_DIRECTION_PIN         I2SO(0)
#define X_DISABLE_PIN           I2SO(5)
// #define X_STEPPER_MS3           I2SO(2)

// Motor Socket #1
// #define X2_STEP_PIN              I2SO(7)
// #define X2_DIRECTION_PIN         I2SO(6)
// #define X2_DISABLE_PIN           I2SO(11)

// #define X_DISABLE_PIN         27  
// #define X_DIRECTION_PIN        14 
// #define X_STEP_PIN              12

// Motor Socket #3
#define Y_STEP_PIN              I2SO(13)
#define Y_DIRECTION_PIN         I2SO(12)
#define Y_DISABLE_PIN           I2SO(17)
// #define Y_STEPPER_MS3           I2SO(14)

// Motor Socket #4
#define Z_STEP_PIN              I2SO(22)
#define Z_DIRECTION_PIN         I2SO(23)
#define Z_DISABLE_PIN           I2SO(18)
// #define Z_STEPPER_MS3           I2SO(21)


// Motor Socket #5
#define C_STEP_PIN              I2SO(28)
#define C_DIRECTION_PIN         I2SO(29)
#define C_DISABLE_PIN           I2SO(24)



// // Motor Socket #4
// #define A_SERVO_PIN 		      GPIO_NUM_26
// #define DEFAULT_A_MAX_TRAVEL          20   // or change it live with $Z/MaxTravel=5.0
// #define DEFAULT_A_HOMING_MPOS         0   // $Z/Homing/MPos=5.0

/*
// Motor Socket #4
#define A_DIRECTION_PIN         I2SO(12)
#define A_STEP_PIN              I2SO(13)
#define A_DISABLE_PIN           I2SO(15)

// Motor Socket #5
#define B_DISABLE_PIN           I2SO(16)
#define B_DIRECTION_PIN         I2SO(17)
#define B_STEP_PIN              I2SO(18)

// Motor Socket #5
#define C_DIRECTION_PIN         I2SO(20)
#define C_STEP_PIN              I2SO(21)
#define C_DISABLE_PIN           I2SO(23)
*/


/*  //^ future tests

#define USE_SERVO_AXES >> config.h
#define SERVO_Y_PIN GPIO_NUM_14
#define SERVO_Y_CHANNEL_NUM 6



#define N_AXIS 3

// === Special Features

// I2S (steppers & other output-only pins)
#define USE_I2S_OUT
#define USE_I2S_STEPS
//#define DEFAULT_STEPPER ST_I2S_STATIC

#define USE_STEPSTICK   // makes sure MS1,2,3 !reset and !sleep are set

#define I2S_OUT_BCK      GPIO_NUM_22
#define I2S_OUT_WS       GPIO_NUM_17
#define I2S_OUT_DATA     GPIO_NUM_21

// Motor Socket #1
#define X_DISABLE_PIN           I2SO(0)
#define X_DIRECTION_PIN         I2SO(1)
#define X_STEP_PIN              I2SO(2)
#define X_STEPPER_MS3           I2SO(3)

// Motor Socket #2
#define Y_DIRECTION_PIN         I2SO(4)
#define Y_STEP_PIN              I2SO(5)
#define Y_STEPPER_MS3           I2SO(6) 
#define Y_DISABLE_PIN           I2SO(7)

// Motor Socket #3
#define Y2_DISABLE_PIN           I2SO(8)
#define Y2_DIRECTION_PIN         I2SO(9)
#define Y2_STEP_PIN              I2SO(10)
#define Y2_STEPPER_MS3           I2SO(11)

// Motor Socket #4
#define Z_DIRECTION_PIN         I2SO(12)
#define Z_STEP_PIN              I2SO(13)
#define Z_STEPPER_MS3           I2SO(14)
#define Z_DISABLE_PIN           I2SO(15)

// Motor Socket #5
#define Z2_DISABLE_PIN           I2SO(16)
#define Z2_DIRECTION_PIN         I2SO(17)
#define Z2_STEP_PIN              I2SO(18)
#define Z2_STEPPER_MS3           I2SO(19)\






// Motor Socket #1
#define X_DISABLE_PIN           I2SO(0)
#define X_DIRECTION_PIN         I2SO(1)
#define X_STEP_PIN              I2SO(2)
#define X_STEPPER_MS3           I2SO(3)

// Motor Socket #2
#define Y_DIRECTION_PIN         I2SO(4)
#define Y_STEP_PIN              I2SO(5)
#define Y_STEPPER_MS3           I2SO(6)
#define Y_DISABLE_PIN           I2SO(7)

// Motor Socket #3
#define Z_DISABLE_PIN           I2SO(8)
#define Z_DIRECTION_PIN         I2SO(9)
#define Z_STEP_PIN              I2SO(10)
#define Z_STEPPER_MS3           I2SO(11)

// Motor Socket #4
#define A_DIRECTION_PIN         I2SO(12)
#define A_STEP_PIN              I2SO(13)
#define A_STEPPER_MS3           I2SO(14) 
#define A_DISABLE_PIN           I2SO(15)

// Motor Socket #5
#define B_DISABLE_PIN           I2SO(16)
#define B_DIRECTION_PIN         I2SO(17)
#define B_STEP_PIN              I2SO(18)
#define B_STEPPER_MS3           I2SO(19) 

// Motor Socket #5
#define C_DIRECTION_PIN         I2SO(20)
#define C_STEP_PIN              I2SO(21)
#define C_STEPPER_MS3           I2SO(22)
#define C_DISABLE_PIN           I2SO(23)




// 4x Input Module in Socket #1
// https://github.com/bdring/6-Pack_CNC_Controller/wiki/4x-Switch-Input-module
#define X_LIMIT_PIN             GPIO_NUM_33
#define Y_LIMIT_PIN             GPIO_NUM_32
#define Y2_LIMIT_PIN            GPIO_NUM_35
#define Z_LIMIT_PIN             GPIO_NUM_34

// 4x Input Module in Socket #2
// https://github.com/bdring/6-Pack_CNC_Controller/wiki/4x-Switch-Input-module
#define Z2_LIMIT_PIN            GPIO_NUM_2


// === Default settings
#define DEFAULT_STEP_PULSE_MICROSECONDS I2S_OUT_USEC_PER_PULSE


#define STEPPERS_DISABLE_PIN GPIO_NUM_13 //^ we have to test


//* ------------                                                 spindle

enum class SpindleType : int8_t {
    NONE = 0,
    PWM,
    RELAY,
    LASER,
    DAC,
    HUANYANG,
    BESC,
    _10V,
    H2A,
    YL620,
    L510
};

#define SPINDLE_TYPE            SpindleType::RELAY
#define SPINDLE_TYPE            SpindleType::PWM

#define SPINDLE_OUTPUT_PIN          GPIO_NUM_16
#define SPINDLE_ENABLE_PIN          GPIO_NUM_32
#define SPINDLE_DIR_PIN         GPIO_NUM_16

#define PROBE_PIN               GPIO_NUM_34



*/

//* ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



//^ ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/*
    Socket I/O reference
    The list of modules is here...
    https://github.com/bdring/6-Pack_CNC_Controller/wiki/CNC-I-O-Module-List
    Click on each module to get example for using the modules in the sockets


Socket #1
#1 GPIO_NUM_33 
#2 GPIO_NUM_32
#3 GPIO_NUM_35 (input only)
#4 GPIO_NUM_34 (input only)

Socket #2
#1 GPIO_NUM_2
#2 GPIO_NUM_25
#3 GPIO_NUM_39 (input only)
#4 GPIO_NUM_36 (input only)

Socket #3
#1 GPIO_NUM_26
#2 GPIO_NUM_4
#3 GPIO_NUM_16
#4 GPIO_NUM_27

Socket #4
#1 GPIO_NUM_14
#2 GPIO_NUM_13
#3 GPIO_NUM_15
#4 GPIO_NUM_12

Socket #5
#1 I2SO(24)  (output only)
#2 I2SO(25)  (output only)
#3 I2SO26)  (output only)

*/


// 4x Input Module in Socket #1
// https://github.com/bdring/6-Pack_CNC_Controller/wiki/4x-Switch-Input-module
#define X_LIMIT_PIN                     GPIO_NUM_33
// #define X2_LIMIT_PIN                    GPIO_NUM_26
#define Y_LIMIT_PIN                     GPIO_NUM_32
#define Z_LIMIT_PIN                     GPIO_NUM_25



// ================= Setting Defaults ==========================

// see wiki https://github.com/bdring/Grbl_Esp32/wiki/External-Stepper-Drivers
#define DEFAULT_STEP_ENABLE_DELAY        3 // how long after enable do we wait for 
#define DEFAULT_STEP_PULSE_MICROSECONDS  0 // length of step pulse. Must be greater than I2S_OUT_USEC_PER_PULSE (4) with I2S
#define STEP_PULSE_DELAY                 6 // gap between enable and dir changes before step

#define SPINDLE_TYPE            SpindleType::PWM

#define SPINDLE_OUTPUT_PIN          GPIO_NUM_26
// #define SPINDLE_ENABLE_PIN          GPIO_NUM_13

#define DEFAULT_STEPPING_INVERT_MASK     (bit(X_AXIS) | bit(Y_AXIS) | bit(Z_AXIS))
#define DEFAULT_DIRECTION_INVERT_MASK    (bit(X_AXIS) | bit(Y_AXIS) | bit(Z_AXIS))
#define DEFAULT_INVERT_ST_ENABLE         false



