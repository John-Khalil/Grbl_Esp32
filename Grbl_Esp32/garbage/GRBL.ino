#include <stdint.h>
#include <cstdlib>
#include <functional>
#include <vector>

class consoleLogger{
    public:

        std::vector<std::function<void(unsigned char*,unsigned char)>>consoleFeedback;
        /*
            *the main idea here is to have a list of call back function that have the real data logger
            *all what is required to setup the predefined instance is to add the callback function like serial print, network logger
            ~both functional and vector shoudl be included from the standard  library 

        */

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
                *(--signedStr)=0x2D;        // '-' sign
            return signedStr;
        }

        unsigned char *mainLogger(unsigned char *consoleData){
            unsigned short consoleFeedBackCounter=consoleFeedback.size();
            while(consoleFeedBackCounter--)
                consoleFeedback[consoleFeedBackCounter](consoleData,autoNLCR);      // passing the data to callback function in the list
            return consoleData;
        }


        void addConsole(const std::function<void(unsigned char*,unsigned char)>newCallBack){
            consoleFeedback.push_back(newCallBack);                                             // adding to the call back list
            return;
        }
       
        void disableNL(void){
            autoNLCR=0;
        }
        void enableNL(void){
            autoNLCR=1;
        }

        // creating overloaded functions to handle any data type
        //? check this blog post for more details <https://www.geeksforgeeks.org/function-overloading-c/#:~:text=Function%20overloading%20is%20a%20feature,it%20is%20called%20Function%20Overloading.>

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

        // creating a template that can have unlimited number of args to be printed
        //? check this blog post for more details <https://www.geeksforgeeks.org/variadic-function-templates-c/#:~:text=Variadic%20templates%20are%20class%20or,help%20to%20overcome%20this%20issue.>

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


extern consoleLogger console;          //^ predefined class instance that could be used globally 
