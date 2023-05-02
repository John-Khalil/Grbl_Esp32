#include <stdint.h>
#include <functional>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include <type_traits>
// #include "stringFunctions.cpp"
#include "consoleLogger.h"

namespace utils{
class highLevelMemory
{
private:

    unsigned short stringCounter(unsigned char *counted){
        unsigned short counter=0;
        while(*counted){
            counter++;
            counted++;
        }
        return counter;
    }
    
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

    unsigned char *_CS(unsigned char *bigString,unsigned char *smallString){
        unsigned char *smallStringLocation=bigString+stringCounter(bigString);		// lucky for us c/c++ support pointer arthematic
        while(*smallString){
            *smallStringLocation=*smallString;
            smallString++;
            smallStringLocation++;
        }
        return bigString;
    }


    uint8_t *MAIN_MEMORY=nullptr;
    uint32_t MAIN_MEMORY_SIZE=0;
    struct highLevelMemoryElement{
        std::string variableName="";
        union{
            uint32_t virtualAddress=-1;
            uint16_t userDefinedAddress;
        }address;
        uint32_t size=0;
        uint32_t length=0;
        uint8_t *physicalAddress=nullptr;
        uint32_t bind=-1;
        uint8_t validToken=0;
        std::vector<std::function<void(unsigned char*)>>onchangeEventListeners;
        std::vector<std::function<void(void)>>readEventListeners;
    };
    std::vector<highLevelMemoryElement>allocationTable;



    uint32_t GLOBAL_INT_RETURN=-1;
    
public:
    uint8_t* NO_DATA=(uint8_t*)"NO_DATA";
    uint8_t* UNDEFINED=(uint8_t*)"undefined";
    const std::string UNDEFINED_STRING="undefined";

    uint32_t getVectorAddress(uint8_t *variableName){
        uint32_t loopCounter=allocationTable.size();
        while(loopCounter--){
            if(allocationTable[loopCounter].variableName==std::string((char*)variableName))
                return loopCounter;
        }
        return -1;
    }
    
    uint32_t getVectorAddress(uint32_t memoryAddress){
        if(memoryAddress>>16)
            return memoryAddress>>16;
        uint32_t loopCounter=allocationTable.size();
        while(loopCounter--){
            if(allocationTable[loopCounter].address.virtualAddress==memoryAddress)
                return loopCounter;
        }
        return -1;
    }

    uint32_t lastAvailabeAddress(void){
        uint32_t lastAddress=0;
        for(const auto &memoryElement : allocationTable)
            lastAddress+=memoryElement.length+1;    // +1 for the end string NULL
        return lastAddress;
    }


    highLevelMemory &shiftAddress(highLevelMemoryElement &memoryElement){
        uint32_t addressCounter=0;
        uint32_t loopCounter=(MAIN_MEMORY+lastAvailabeAddress())-(memoryElement.physicalAddress+memoryElement.length+1);
        while(loopCounter--)
            memoryElement.physicalAddress[addressCounter]=memoryElement.physicalAddress[(memoryElement.length+1)+(addressCounter++)];
        return *this;
    }

    highLevelMemory &shiftAllocationTable(highLevelMemoryElement &memoryElement){
        uint32_t addressCounter=(memoryElement.address.virtualAddress>>16);
        // uint32_t loopCounter=allocationTable.size()-addressCounter;
        while((++addressCounter)<allocationTable.size()){
            allocationTable[addressCounter].address.virtualAddress-=65536;  // instaed of (((x>>16)-1)<<16)|(x&65535)
            allocationTable[addressCounter].physicalAddress-=memoryElement.length+1;
        }
        return *this;
    }

    highLevelMemoryElement lastActiveElement;

    highLevelMemoryElement nullElement;

    highLevelMemory &get(uint8_t* key){
        for(auto &memoryElement : allocationTable)
            if(memoryElement.variableName==std::string((char*)key)){
                lastActiveElement=memoryElement;             
                return (*this); 
            }
        lastActiveElement=nullElement;
        return (*this);
    }

    highLevelMemory &get(uint32_t key){
        for(auto memoryElementNonRef : allocationTable)
            if((memoryElementNonRef=(key>>16)?allocationTable[key>>16]:memoryElementNonRef).address.userDefinedAddress==(key&0xFFFF)){    // switch context for the full virtual address
                lastActiveElement=memoryElementNonRef; 
                return (*this);             
            }   
        lastActiveElement=nullElement;
        return (*this);
    }

    highLevelMemory &onChange(const std::function<void(unsigned char *)>onchangeEventListener){
        if(lastActiveElement.physicalAddress!=nullptr)
            allocationTable[lastActiveElement.address.virtualAddress>>16].onchangeEventListeners.push_back(onchangeEventListener);
        return (*this);
    }

    highLevelMemory &onRead(const std::function<void(void)>readEventListener){
        if(lastActiveElement.physicalAddress!=nullptr)
            allocationTable[lastActiveElement.address.virtualAddress>>16].readEventListeners.push_back(readEventListener);
        return (*this);
    }

    highLevelMemory &bind(uint8_t* key){
        if(lastActiveElement.physicalAddress!=nullptr)
            for(auto &memoryElement : allocationTable)
                if(memoryElement.variableName==std::string((char*)key)){
                    allocationTable[lastActiveElement.address.virtualAddress>>16].bind=memoryElement.address.virtualAddress;              
                }
        return (*this);
    }

    highLevelMemory &bind(uint32_t key){
        if(lastActiveElement.physicalAddress!=nullptr)
            for(auto memoryElementNonRef : allocationTable)
                if((memoryElementNonRef=(key>>16)?allocationTable[key>>16]:memoryElementNonRef).address.userDefinedAddress==(key&0xFFFF)){    // switch context for the full virtual address
                    allocationTable[lastActiveElement.address.virtualAddress>>16].bind=memoryElementNonRef.address.virtualAddress;                     
                } 
        return (*this);
    }

    highLevelMemory &bind(void){
        if(lastActiveElement.physicalAddress!=nullptr)
            allocationTable[lastActiveElement.address.virtualAddress>>16].bind=-1;
        return (*this);
    }

    highLevelMemory &unBind(void){
        allocationTable[lastActiveElement.address.virtualAddress>>16].bind=-1;
        return (*this);
    }

    uint32_t getAddress(uint8_t *key){
        for(auto &memoryElement : allocationTable)
            if(memoryElement.variableName==std::string((char*)key))
                return memoryElement.address.virtualAddress;
        return -1;
    }

    uint32_t getAddress(uint32_t key){
        for(auto &memoryElement : allocationTable)
            if(memoryElement.address.userDefinedAddress==(key&0xFFFF))
                return memoryElement.address.virtualAddress;
        return -1;
    }

    // uint8_t validToken=0;

    highLevelMemory &WRITE(uint32_t key,uint8_t* data,uint8_t *keyString=nullptr,uint32_t dataLength=NULL){
        dataLength=((dataLength)?dataLength:stringCounter(data));
        highLevelMemoryElement newElement;
        uint16_t bindIndex=-1;
        for(auto memoryElementNonRef : allocationTable)
            if((memoryElementNonRef=(key>>16)?allocationTable[key>>16]:memoryElementNonRef).address.userDefinedAddress==(key&0xFFFF)){

                // bindIndex=(memoryElementNonRef.bind!=-1)?(memoryElementNonRef.address.virtualAddress>>16):bindIndex;                                     // keep index
                key=(memoryElementNonRef.bind!=-1)?(allocationTable[getAddress(memoryElementNonRef.bind)>>16].address.virtualAddress):key;                  //! THIS HAS TO COME FIRST -- it will be written back with the old address
                memoryElementNonRef=(memoryElementNonRef.bind!=-1)?allocationTable[getAddress(memoryElementNonRef.bind)>>16]:memoryElementNonRef;           // switch context for memory binding

                auto &memoryElement=allocationTable[memoryElementNonRef.address.virtualAddress>>16];                                                        //* get back to the org ref


                

                newElement=memoryElement;
                if(dataLength==memoryElement.length){
                    _CS(CLR(memoryElement.physicalAddress),data);
                }
                else{
                    shiftAddress(memoryElement).shiftAllocationTable(memoryElement).allocationTable.erase(allocationTable.begin() + (memoryElement.address.virtualAddress>>16));
                    break;      // adding it as a new element
                }
                goto functionReturn; //dry code                
            }
        
        if((dataLength+lastAvailabeAddress())<(MAIN_MEMORY_SIZE+1)){
            if(keyString!=nullptr)
                newElement.variableName=std::string((char*)keyString);
            newElement.length=dataLength;
            newElement.address.virtualAddress=(allocationTable.size()<<16)|(key&0xFFFF);        
            //! for the user defiend address if it was an int it will be represented from bit 14>0 if it was string it will be represented from bit 15>0 along with a counter stored in bits 14>0 (couter|0x8000)
            newElement.physicalAddress=MAIN_MEMORY+lastAvailabeAddress();
            allocationTable.push_back(newElement);
            _CS(CLR(newElement.physicalAddress,newElement.length+1),data);
        }

        functionReturn:
        lastActiveElement=newElement;
        if(!newElement.validToken)
        for(auto &onchangeCallback:allocationTable[(bindIndex==(uint16_t)-1)?(lastActiveElement.address.virtualAddress>>16):bindIndex].onchangeEventListeners)
            onchangeCallback(lastActiveElement.physicalAddress);

        return (*this);
    }

    uint8_t *READ(uint32_t key,uint8_t nonValidToken=0){
            for(auto memoryElementNonRef : allocationTable)
                if((memoryElementNonRef=(key>>16)?allocationTable[key>>16]:memoryElementNonRef).address.userDefinedAddress==(key&0xFFFF)){

                key=(memoryElementNonRef.bind!=-1)?(allocationTable[getAddress(memoryElementNonRef.bind)>>16].address.virtualAddress):key;                  //! THIS HAS TO COME FIRST -- changing the key for the next recursive call
                memoryElementNonRef=(memoryElementNonRef.bind!=-1)?allocationTable[getAddress(memoryElementNonRef.bind)>>16]:memoryElementNonRef;           // switch context for memory binding

                auto &memoryElement=allocationTable[memoryElementNonRef.address.virtualAddress>>16];                                                        //* get back to the org ref

                lastActiveElement=memoryElement;

                // static uint8_t validToken;
                if(!memoryElement.validToken){
                    memoryElement.validToken=1;
                    for(auto &readCallback:allocationTable[lastActiveElement.address.virtualAddress>>16].readEventListeners)
                        readCallback();
                    uint8_t *updatedAddress=READ((key&0xffff),1); //* the element may change if the read callback triggered a write for the same element, 
                    //* the key have been masked to insure we're not loking for and elemnt that doesnot exist anymore
                    // memoryElement.validToken=0;
                    return updatedAddress;
                }

                if(nonValidToken)
                    memoryElement.validToken=0;     //* this is to fix the recursive mess we have just created!!

                return memoryElement.physicalAddress;                
            }
    
        return UNDEFINED;
    }

    //^ follower overLoaded functions
    uint8_t *read(uint32_t key){
        return READ(key&=~0x8000);
    }

    highLevelMemory & write(uint32_t key,uint8_t* data,uint32_t dataLength=NULL){
        return WRITE((key&=~0x8000),data,nullptr,dataLength);
    }

    uint8_t *read(uint8_t *key){
        uint32_t keyAddress=getAddress(key);
        return ((keyAddress==(uint32_t)-1)?UNDEFINED:READ(keyAddress));
    }

    highLevelMemory & write(uint8_t *key,uint8_t* data,uint32_t dataLength=NULL){
        static uint32_t keyCounter;
        uint32_t keyAddress=getAddress(key);
        return WRITE(((keyAddress==(uint32_t)-1)?(++keyCounter|0x8000):keyAddress),data,key,dataLength);       // auto assign user defined address for a new added key , it should stay
    }


    uint8_t *read(void){
        return READ(lastActiveElement.address.virtualAddress);
    }

    highLevelMemory & write(uint8_t* data,uint32_t dataLength=NULL){
        return WRITE(lastActiveElement.address.virtualAddress,data,nullptr,dataLength);
    }


    //^ debug utils
    void printAllocationTable(void){
        for(auto memoryElementNonRef : allocationTable)
            console.log("\t\t\t--\t\t\t >> ",memoryElementNonRef.address.userDefinedAddress,"\t\t\t--\t\t\t",memoryElementNonRef.variableName.c_str(),"\t\t\t--\t\t\t",memoryElementNonRef.physicalAddress);
    }


    //^ overloaded operators

    #define ARRAY_N_ELEMENTS 10

    uint32_t LAST_ACTIVE_ELEMENTS[ARRAY_N_ELEMENTS]={};
    void pushElement(uint32_t newMemoryElement){
        uint8_t elementsCounter=ARRAY_N_ELEMENTS;
        while(--elementsCounter)
            LAST_ACTIVE_ELEMENTS[elementsCounter]=LAST_ACTIVE_ELEMENTS[elementsCounter-1];
        LAST_ACTIVE_ELEMENTS[0]=newMemoryElement;
        return;
    }

    highLevelMemoryElement sameClassObject=lastActiveElement;

    operator uint8_t*() {
        highLevelMemoryElement lastActiveElementBuffer=lastActiveElement;
        uint8_t *readBuffer=read();
        lastActiveElement=lastActiveElementBuffer;
		return readBuffer;
	}

    operator int8_t*() {
        highLevelMemoryElement lastActiveElementBuffer=lastActiveElement;
        uint8_t *readBuffer=read();
        lastActiveElement=lastActiveElementBuffer;
		return (int8_t*)readBuffer;
	}

    operator char*() {
        highLevelMemoryElement lastActiveElementBuffer=lastActiveElement;
        uint8_t *readBuffer=read();
        lastActiveElement=lastActiveElementBuffer;
		return (char*)readBuffer;
	}

    operator std::string(){
        highLevelMemoryElement lastActiveElementBuffer=lastActiveElement;
        uint8_t *readBuffer=read();
        lastActiveElement=lastActiveElementBuffer;
        return std::string((char*)readBuffer);
    }

    

    operator int(){
        return (int)GLOBAL_INT_RETURN;
    }



    

    highLevelMemory &operator [](uint8_t *key){
        if(getAddress((uint8_t*)key)==uint32_t(-1))
            write((uint8_t*)key,(uint8_t*)"");
        pushElement(getAddress(key)&0xffff);
        sameClassObject=lastActiveElement;
        return get(key);
    }

    highLevelMemory &operator [](int8_t *key){
        if(getAddress((uint8_t*)key)==uint32_t(-1))
            write((uint8_t*)key,(uint8_t*)"");
        pushElement(getAddress((uint8_t*)key)&0xffff);
        sameClassObject=lastActiveElement;
        return get((uint8_t *)key);
    }

    highLevelMemory &operator [](char *key){
        if(getAddress((uint8_t*)key)==uint32_t(-1))
            write((uint8_t*)key,(uint8_t*)"");
        pushElement(getAddress((uint8_t*)key)&0xffff);
        sameClassObject=lastActiveElement;
        return get((uint8_t *)key);
    }

    highLevelMemory &operator [](const char *key){
        if(getAddress((uint8_t*)key)==uint32_t(-1))
            write((uint8_t*)key,(uint8_t*)"");
        pushElement(getAddress((uint8_t*)key)&0xffff);
        sameClassObject=lastActiveElement;
        return get((uint8_t *)key);
    }

    highLevelMemory &operator [](std::string key){
        if(getAddress((uint8_t*)key.c_str())==uint32_t(-1))
            write((uint8_t*)key.c_str(),(uint8_t*)"");
        pushElement(getAddress((uint8_t*)key.c_str())&0xffff);
        sameClassObject=lastActiveElement;
        return get((uint8_t *)key.c_str());
    }

    highLevelMemory &operator [](uint32_t key){
        if(getAddress(key)==uint32_t(-1))
            write(key,(uint8_t*)"");
        pushElement(getAddress(key)&0xffff);
        sameClassObject=lastActiveElement;
        return get(key);
    }

    highLevelMemory &operator [](int32_t key){
        if(getAddress((uint32_t)key)==uint32_t(-1))
            write(key,(uint8_t*)"");
        pushElement(getAddress((uint32_t)key)&0xffff);
        sameClassObject=lastActiveElement;
        return get((uint32_t)key);
    }



    highLevelMemory &operator=(uint8_t *data){
        sameClassObject=lastActiveElement;
        return write(data);
    }

    highLevelMemory &operator=(int8_t *data){
        sameClassObject=lastActiveElement;
        return write((uint8_t *)data);
    }

    highLevelMemory &operator=(char *data){
        sameClassObject=lastActiveElement;
        return write((uint8_t *)data);
    }

    highLevelMemory &operator=(const char *data){
        sameClassObject=lastActiveElement;
        return write((uint8_t *)data);
    }

    highLevelMemory &operator=(std::string data){
        sameClassObject=lastActiveElement;
        return write((uint8_t *)data.c_str());
    }

    highLevelMemory &operator=(highLevelMemory &data){        
        if(&data==this){
            // console.log(" >> ",(uint16_t)lastActiveElement.address.userDefinedAddress," - ",(uint16_t)LAST_ACTIVE_ELEMENTS[0]," - ",(uint16_t)LAST_ACTIVE_ELEMENTS[1]);
            uint32_t LAST_ACTIVE_ELEMENTS_A=LAST_ACTIVE_ELEMENTS[1];//(LAST_ACTIVE_ELEMENTS[0]==lastActiveElement.address.userDefinedAddress)?LAST_ACTIVE_ELEMENTS[1]:LAST_ACTIVE_ELEMENTS[0];
            uint32_t LAST_ACTIVE_ELEMENTS_B=LAST_ACTIVE_ELEMENTS[0];//(LAST_ACTIVE_ELEMENTS[0]==lastActiveElement.address.userDefinedAddress)?LAST_ACTIVE_ELEMENTS[0]:LAST_ACTIVE_ELEMENTS[1];
            // console.log((uint8_t *)std::string((char*)read(LAST_ACTIVE_ELEMENTS_B)).c_str());

            return WRITE(LAST_ACTIVE_ELEMENTS_A,(uint8_t *)std::string((char*)READ(LAST_ACTIVE_ELEMENTS_B)).c_str());
        }
            // return write(LAST_ACTIVE_ELEMENTS[0],(uint8_t *)std::string((char*)read(LAST_ACTIVE_ELEMENTS[1])).c_str());
        else
            return write((uint8_t *)data);
    }



    highLevelMemory &operator!(){
        GLOBAL_INT_RETURN=(lastActiveElement.length==0);
        return (*this);
    }



    highLevelMemory &operator==(uint8_t *data){
        GLOBAL_INT_RETURN=(std::string((char*)lastActiveElement.physicalAddress)==std::string((char*)data));
        return (*this);
    }

    highLevelMemory &operator==(int8_t *data){
        GLOBAL_INT_RETURN=(std::string((char*)lastActiveElement.physicalAddress)==std::string((char*)data));
        return (*this);
    }

    highLevelMemory &operator==(char *data){
        GLOBAL_INT_RETURN=(std::string((char*)lastActiveElement.physicalAddress)==std::string((char*)data));
        return (*this);
    }

    highLevelMemory &operator==(const char *data){
        GLOBAL_INT_RETURN=(std::string((char*)lastActiveElement.physicalAddress)==std::string((char*)data));
        return (*this);
    }

    highLevelMemory &operator==(std::string data){
        GLOBAL_INT_RETURN=(std::string((char*)lastActiveElement.physicalAddress)==data);
        return (*this);
    }

    highLevelMemory &operator==(highLevelMemory &data){
        if(&data==this){
            // console.log(" >> ",(uint16_t)lastActiveElement.address.userDefinedAddress," - ",(uint16_t)LAST_ACTIVE_ELEMENTS[0]," - ",(uint16_t)LAST_ACTIVE_ELEMENTS[1]);
            GLOBAL_INT_RETURN=(std::string((char*)READ(LAST_ACTIVE_ELEMENTS[0]))==std::string((char*)READ(LAST_ACTIVE_ELEMENTS[1])));
        }
            // GLOBAL_INT_RETURN=(std::string((char*)read(LAST_ACTIVE_ELEMENTS[1]))==std::string((char*)data));
        else
            GLOBAL_INT_RETURN=(std::string((char*)lastActiveElement.physicalAddress)==std::string((char*)data));
        return (*this);
    }



    highLevelMemory &operator!=(uint8_t *data){
        GLOBAL_INT_RETURN=(std::string((char*)lastActiveElement.physicalAddress)!=std::string((char*)data));
        return (*this);
    }

    highLevelMemory &operator!=(int8_t *data){
        GLOBAL_INT_RETURN=(std::string((char*)lastActiveElement.physicalAddress)!=std::string((char*)data));
        return (*this);
    }

    highLevelMemory &operator!=(char *data){
        GLOBAL_INT_RETURN=(std::string((char*)lastActiveElement.physicalAddress)!=std::string((char*)data));
        return (*this);
    }

    highLevelMemory &operator!=(const char *data){
        GLOBAL_INT_RETURN=(std::string((char*)lastActiveElement.physicalAddress)!=std::string((char*)data));
        return (*this);
    }

    highLevelMemory &operator!=(std::string data){
        GLOBAL_INT_RETURN=(std::string((char*)lastActiveElement.physicalAddress)!=data);
        return (*this);
    }

    highLevelMemory &operator!=(highLevelMemory &data){
        if(&data==this)
            GLOBAL_INT_RETURN=(std::string((char*)READ(LAST_ACTIVE_ELEMENTS[1]))!=std::string((char*)data));
        else
            GLOBAL_INT_RETURN=(std::string((char*)lastActiveElement.physicalAddress)!=std::string((char*)data));
        return (*this);
    }



    highLevelMemory &operator|=(uint8_t *data){
        if(lastActiveElement.length)
            return get(lastActiveElement.address.virtualAddress);
        sameClassObject=lastActiveElement;
        return write(data);
    }

    highLevelMemory &operator|=(int8_t *data){
        if(lastActiveElement.length)
            return get(lastActiveElement.address.virtualAddress);
        sameClassObject=lastActiveElement;
        return write((uint8_t *)data);
    }

    highLevelMemory &operator|=(char *data){
        if(lastActiveElement.length)
            return get(lastActiveElement.address.virtualAddress);
        sameClassObject=lastActiveElement;
        return write((uint8_t *)data);
    }

    highLevelMemory &operator|=(const char *data){
        if(lastActiveElement.length)
            return get(lastActiveElement.address.virtualAddress);
        sameClassObject=lastActiveElement;
        return write((uint8_t *)data);
    }

    highLevelMemory &operator|=(std::string data){
        if(lastActiveElement.length)
            return get(lastActiveElement.address.virtualAddress);
        sameClassObject=lastActiveElement;
        return write((uint8_t *)data.c_str());
    }

    highLevelMemory &operator|=(highLevelMemory &data){        
        uint32_t elementAllocationTableAddress=getAddress(LAST_ACTIVE_ELEMENTS[1]);
        if(elementAllocationTableAddress!=(uint32_t)-1);
        if(allocationTable[elementAllocationTableAddress>>16].length)
            return get(elementAllocationTableAddress);

        if(&data==this){

            uint32_t LAST_ACTIVE_ELEMENTS_A=LAST_ACTIVE_ELEMENTS[1];//(LAST_ACTIVE_ELEMENTS[0]==lastActiveElement.address.userDefinedAddress)?LAST_ACTIVE_ELEMENTS[1]:LAST_ACTIVE_ELEMENTS[0];
            uint32_t LAST_ACTIVE_ELEMENTS_B=LAST_ACTIVE_ELEMENTS[0];//(LAST_ACTIVE_ELEMENTS[0]==lastActiveElement.address.userDefinedAddress)?LAST_ACTIVE_ELEMENTS[0]:LAST_ACTIVE_ELEMENTS[1];
            return WRITE(LAST_ACTIVE_ELEMENTS_A,(uint8_t *)std::string((char*)READ(LAST_ACTIVE_ELEMENTS_B)).c_str());
        }
        else
            return write((uint8_t *)data);
    }


    

    highLevelMemory &operator >>(const std::function<void(unsigned char *)>onchangeEventListener){
        if(lastActiveElement.physicalAddress!=nullptr)
            allocationTable[lastActiveElement.address.virtualAddress>>16].onchangeEventListeners.push_back(onchangeEventListener);
        return (*this);
    }

    highLevelMemory &operator <<(const std::function<void(void)>readEventListener){
        if(lastActiveElement.physicalAddress!=nullptr)
            allocationTable[lastActiveElement.address.virtualAddress>>16].readEventListeners.push_back(readEventListener);
        return (*this);
    }


    // highLevelMemory &write(uint8_t* key,uint8_t* data){
    //     highLevelMemoryElement newElement;
    //     uint16_t bindIndex=-1;
    //     for(auto &memoryElement : allocationTable)
    //         if(memoryElement.variableName==std::string((char*)key)){
    //             bindIndex=(memoryElement.bind!=-1)?(memoryElement.address.virtualAddress>>16):bindIndex;             // keep index
    //             memoryElement=(memoryElement.bind!=-1)?allocationTable[memoryElement.bind>>16]:memoryElement;        // switch context for memory binding

    //             newElement=memoryElement;
    //             if(stringCounter(data)==memoryElement.length){
    //                 _CS(CLR(memoryElement.physicalAddress,memoryElement.length+1),data);
    //             }
    //             else{
    //                 shiftAddress(memoryElement).shiftAllocationTable(memoryElement).allocationTable.erase(allocationTable.begin() + (memoryElement.address.virtualAddress>>16));
    //                 break;      // adding it as a new element
    //             }
    //             goto functionReturn; //dry code                
    //         }
        
    //     if((stringCounter(data)+lastAvailabeAddress())<(MAIN_MEMORY_SIZE+1)){
    //         newElement.variableName=std::string((char*)key);
    //         newElement.length=stringCounter(data);
    //         newElement.address.virtualAddress=(allocationTable.size()<<16);
    //         newElement.physicalAddress=MAIN_MEMORY+lastAvailabeAddress();
    //         allocationTable.push_back(newElement);
    //         _CS(CLR(newElement.physicalAddress,newElement.length+1),data);
    //     }

    //     functionReturn:
    //     lastActiveElement=newElement;
    //     if(!newElement.validToken)
    //     for(auto &onchangeCallback:allocationTable[(bindIndex==(uint16_t)-1)?(lastActiveElement.address.virtualAddress>>16):bindIndex].onchangeEventListeners)
    //         onchangeCallback(lastActiveElement.physicalAddress);

    //     return (*this);
    // }

    // uint8_t *read(uint8_t* key){
    //     for(auto &memoryElement : allocationTable)
    //         if(memoryElement.variableName==std::string((char*)key)){
    //             memoryElement=(memoryElement.bind!=-1)?allocationTable[memoryElement.bind>>16]:memoryElement;        // switch context for memory binding

    //             lastActiveElement=memoryElement;

    //             // static uint8_t validToken;
    //             if(!memoryElement.validToken){
    //                 memoryElement.validToken=1;
    //                 for(auto &readCallback:allocationTable[lastActiveElement.address.virtualAddress>>16].readEventListeners)
    //                     readCallback();
    //                 uint8_t *updatedAddress=read(key); // the element may change if the read callback triggered a write for the same element
    //                 memoryElement.validToken=0;
    //                 return updatedAddress;
    //             }

    //             return memoryElement.physicalAddress;                
    //         }
    
    //     return UNDEFINED;
    // }

    highLevelMemory(uint32_t memorySize){
        MAIN_MEMORY_SIZE=memorySize;
        MAIN_MEMORY=(uint8_t*)calloc(++memorySize,sizeof(uint8_t));
    }
    ~highLevelMemory(){
        free(MAIN_MEMORY);
    }
};

}
