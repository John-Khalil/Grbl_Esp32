#include <stdint.h>
#include <cstdlib>
#include <functional>
#include <vector>

class consoleLogger{

    std::vector<std::function<void(unsigned char*,unsigned char)>>consoleFeedback;

    // void (*_clkPin)(unsigned char);
    // void (*_dataPin)(unsigned char);
    // void (*_Delay_us)(unsigned long);
    // unsigned char (*_syncPin)(void);

    std::function<void(unsigned char)>_clkPin;
    std::function<void(unsigned char)>_dataPin;
    std::function<unsigned char(void)>_syncPin;
    std::function<void(unsigned long)>_Delay_us;

    std::function<void(unsigned char)>_clkPinChangeDirection;
    std::function<void(unsigned char)>_dataPinChangeDirection;
    std::function<unsigned char(void)>_clkPinRead;
    std::function<unsigned char(void)>_dataPinRead;

    std::function<void(unsigned char*,unsigned char)>_consoleFeedback;


    float _clkSpeed=0;

    unsigned char autoNLCR=1;


    unsigned short CLR_LENGTH=0;									//this value will be reseted to zero after clearing the string/uint_8 pointer
    unsigned char * CLR(unsigned char *deletedString,unsigned short _CLR_LENGTH=0){
        CLR_LENGTH=(CLR_LENGTH)?CLR_LENGTH:_CLR_LENGTH;
        unsigned char *returnedString=deletedString;
        while(*deletedString||(CLR_LENGTH-=(CLR_LENGTH!=0))){
            *deletedString=0;
            deletedString++;	
        }
        return returnedString;
    }





    unsigned char GLOBAL_64_BIT_INT_TO_STRING[21]={};
    unsigned char* inttostring(uint64_t num){
        CLR(GLOBAL_64_BIT_INT_TO_STRING,21);
        unsigned char finalPointerIndex=20;
        uint64_t modOperator=1;
        uint64_t conversionAccumulator=0;

        while((conversionAccumulator=(num%(modOperator*=10)))!=num)
            GLOBAL_64_BIT_INT_TO_STRING[--finalPointerIndex]=(conversionAccumulator/(modOperator/10))+0x30;
        GLOBAL_64_BIT_INT_TO_STRING[--finalPointerIndex]=(conversionAccumulator/(modOperator/10))+0x30;		// for the very last digit

        return GLOBAL_64_BIT_INT_TO_STRING+finalPointerIndex;		// lucky of us the c++ support pointer arthematic
    }


    unsigned char* longToString(int64_t num){
        unsigned char *signedStr=inttostring((num<0)?(num*-1):num);
        if(num<0)
            *(--signedStr)=0x2D;
        return signedStr;
    }

    unsigned char *mainLogger(unsigned char *consoleData){
        unsigned short consoleFeedBackCounter=consoleFeedback.size();
        while(consoleFeedBackCounter--)
            consoleFeedback[consoleFeedBackCounter](consoleData,autoNLCR);
        return consoleData;
    }








    public:

        void addConsole(const std::function<void(unsigned char*,unsigned char)>newCallBack){
            consoleFeedback.push_back(newCallBack);
            return;
        }
       
        void disableNL(void){
            autoNLCR=0;
        }
        void enableNL(void){
            autoNLCR=1;
        }

        unsigned char *log(unsigned char *consoleData){
            return mainLogger(consoleData);
        }
        unsigned char *log(char *consoleData){
            return mainLogger((unsigned char*)consoleData);
        }
        unsigned char *log(const char *consoleData){
            return mainLogger((unsigned char*)consoleData);
        }
        unsigned char *log(unsigned long consoleData){
            return mainLogger(inttostring(consoleData));
        }

        unsigned char *log(unsigned short consoleData){
            return mainLogger(inttostring(consoleData));
        }

        unsigned char *log(unsigned char consoleData){
            return mainLogger(inttostring(consoleData));
        }

        unsigned char *log(long consoleData){
            return mainLogger(longToString(consoleData));
        }

        unsigned char *log(short consoleData){
            return mainLogger(longToString(consoleData));
        }

        unsigned char *log(char consoleData){
            return mainLogger(longToString(consoleData));
        }

        unsigned char *log(double consoleData){
            #define extraDigits 5
            const float decimalPlace=1e5f;
            unsigned char *biggerNumber=longToString(consoleData*decimalPlace);
            unsigned char biggerNumberCharCount=0;
            while(biggerNumber[biggerNumberCharCount++]);
            unsigned char decimalPointIndex=(--biggerNumberCharCount)-extraDigits;
            unsigned char endsWithZero=1;
            while((biggerNumberCharCount--)-decimalPointIndex){
                biggerNumber[biggerNumberCharCount+1]=biggerNumber[biggerNumberCharCount];
                if(endsWithZero){
                    endsWithZero=!(biggerNumber[biggerNumberCharCount]-0x30);
                    biggerNumber[biggerNumberCharCount+1]*=(biggerNumber[biggerNumberCharCount+1]!=0x30);
                }
            }    
            biggerNumber[decimalPointIndex]=0x2E;                                                   //finally adding the decimal point
            if(!biggerNumber[decimalPointIndex+1])
                biggerNumber[decimalPointIndex+1]=0x30;
            return mainLogger(biggerNumber);
             
        }



        void log(void){

        }

        template<typename T,typename... Types>
        void log(T arg1,Types... arg2){
            disableNL();
            log(arg1);
            autoNLCR=(sizeof...(Types)==1);
            log(arg2...);
        }




		
};

consoleLogger console;