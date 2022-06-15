#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_log.h"
#include "sdkconfig.h"


void task1()
{
    while(true)
    {
        printf("reading temperature \n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void task2()
{
    while(true)
    {
        printf("reading humidity \n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}


void app_main() 
{
    xTaskCreate(&task1, "tmperature reading", 2048, NULL, 2, NULL);
    xTaskCreate(&task2, "humidity reading", 2048, NULL, 2, NULL);
        
}