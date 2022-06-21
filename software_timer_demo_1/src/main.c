#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

#include "esp_timer.h" // esp_timer_create() gibi fonksiyonlar


void myTimerCallBack(TimerHandle_t xTimer)
{
    printf("MyTimer CallBack\n");

}

void myTimerCallBack2(TimerHandle_t xTimer)
{
    printf("MyTimer CallBack 2\n");

}

void app_main() 
{
    TimerHandle_t myTimer1 = NULL;
    TimerHandle_t myTimer2 = NULL;

    myTimer1 = xTimerCreate("myTimer1", pdMS_TO_TICKS(1000), pdTRUE, (void*)1, myTimerCallBack);
    myTimer2 = xTimerCreate("myTimer2", pdMS_TO_TICKS(2000), pdTRUE, (void*)1, myTimerCallBack2);

    printf("Timer start \n");
    xTimerStart(myTimer1, 0);
    xTimerStart(myTimer2, 0);





}