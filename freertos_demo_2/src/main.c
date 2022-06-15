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
        printf("reading temperature [%d] \n", xTaskGetTickCount());
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void task2()
{
    while(true)
    {
        printf("reading humidity [%d] \n", xTaskGetTickCount());
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}


void app_main() 
{
    xTaskCreate(&task1, "temperature reading", 2048, NULL, 1, NULL); // düşük rakam verdim ilk buraya geliyor.
    xTaskCreate(&task2, "humidity reading", 2048, NULL, 2, NULL); // yüksek rakam verdim sonra buradan başlıyor
    
    //xTaskCreatePinnedToCore(&task2, "humidity reading", 2048, NULL, 1, NULL,1); // Bu fonksiyon ikinci öncelik sağlıyor, öyle tespit ettim.
}

// rakamlardan emin olmamakla beraber en yüksek 5 verilebiliyormuş
// 1024 verince kod göçüyor

/*
    reading temperature [0]
    reading humidity [0]
    reading humidity [100] 
    reading humidity [200] 
    reading humidity [300] 
    reading humidity [400] 
    reading humidity [500] 
    reading temperature [500]
    reading humidity [600] 
    reading humidity [700] 
    reading humidity [800] 
    reading humidity [900] 
    reading humidity [1000] 
    reading temperature [1000]
*/