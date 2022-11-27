#include <Arduino.h>


#define async(asyncThread)  xTaskCreate((void (*)(void*))[&](void *arg){asyncThread;vTaskDelete(NULL);},"async-task",30000,NULL,1,NULL);
