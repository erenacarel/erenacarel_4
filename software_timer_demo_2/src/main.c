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
    const esp_timer_create_args_t esp_timer = {
        .callback = myTimerCallBack2,
        .name = "my_timer"
    };

    esp_timer_handle_t esp_timer_handle;
    esp_timer_create(&esp_timer, &esp_timer_handle);
    esp_timer_start_periodic(esp_timer_handle, 1000);

    printf("Timer start \n");
    

}