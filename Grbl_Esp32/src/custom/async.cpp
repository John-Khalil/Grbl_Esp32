#include <Arduino.h>


#define async(asyncThread)  xTaskCreate((void (*)(void*))[&](void *arg){asyncThread;vTaskDelete(NULL);},"async-task",500,NULL,0,NULL);
