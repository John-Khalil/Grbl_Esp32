#include <stdint.h>
#include <cstdlib>

volatile uint32_t *_outputRegisterLow=((volatile uint32_t*)0X3FF44004UL);
volatile uint32_t *_outputRegisterHigh=((volatile uint32_t*)0X3FF44010UL);

volatile uint32_t *_outputRegisterLowSet=((volatile uint32_t*)0X3FF44008UL);
volatile uint32_t *_outputRegisterHighSet=((volatile uint32_t*)0X3FF44014UL);

volatile uint32_t *_outputRegisterLowClear=((volatile uint32_t*)0X3FF4400CUL);
volatile uint32_t *_outputRegisterHighClear=((volatile uint32_t*)0X3FF44018UL);




#define outputRegisterLow (*_outputRegisterLow)
#define outputRegisterHigh (*_outputRegisterHigh)

#define outputRegisterLowSet (*_outputRegisterLowSet)
#define outputRegisterHighSet (*_outputRegisterHighSet)

#define outputRegisterLowClear (*_outputRegisterLowClear)
#define outputRegisterHighClear (*_outputRegisterHighClear)


#define inputRegisterLow (*((volatile uint32_t*)0x3FF4403CUL))
#define inputRegisterHigh (*((volatile uint32_t*)0x3FF44040UL))



volatile uint32_t *_outputEnableRegisterLow=((volatile uint32_t*)0X3FF44020UL);
volatile uint32_t *_outputEnableRegisterHigh=((volatile uint32_t*)0X3FF44024UL);

volatile uint32_t *_outputEnableRegisterLowSet=((volatile uint32_t*)0X3FF44028UL);
volatile uint32_t *_outputEnableRegisterHighSet=((volatile uint32_t*)0X3FF4402CUL);

volatile uint32_t *_outputEnableRegisterLowClear=((volatile uint32_t*)0X3FF44030UL);
volatile uint32_t *_outputEnableRegisterHighClear=((volatile uint32_t*)0X3FF44034UL);




#define outputEnableRegisterLow (*_outputEnableRegisterLow)
#define outputEnableRegisterHigh (*_outputEnableRegisterHigh)

#define outputEnableRegisterLowSet (*_outputEnableRegisterLowSet)
#define outputEnableRegisterHighSet (*_outputEnableRegisterHighSet)

#define outputEnableRegisterLowClear (*_outputEnableRegisterLowClear)
#define outputEnableRegisterHighClear (*_outputEnableRegisterHighClear)









#define _DW digitalWrite
#define _DR digitalRead
#define _PM pinMode