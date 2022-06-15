#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_log.h"
#include "sdkconfig.h"

/*
    Queue oluşturma da semaphore mantğına benzemektedir. Avantajı bununla data
    gönderilebilmektedir.
*/

xQueueHandle queue;

void listenForHTTP(void *params)
{
    int count = 0;

    while(true)
    {
        count++;
        printf("received http message 1\n");
        long ok = xQueueSend(queue, &count, 1000 / portTICK_PERIOD_MS);
        if(ok)
        {
            // Demek ki kuyruğa bir şeyler gönderilmiş.
            // bir süre sonra fail'e dönüyor. 
            printf("added message to queue 2\n");
        }
        else
        {
            printf("failed to add message to queue 3\n");
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);

    }
}

void task1(void *params)
{
    while(true)
    {
        int rxInt;
        if(xQueueReceive(queue, &rxInt, 1000 / portMAX_DELAY))
        {
            printf("doing something with http 4|| %d\n", rxInt);
        }
        
    }
}


void app_main(void) 
{
    queue = xQueueCreate(3, sizeof(int));
    xTaskCreate(listenForHTTP, "get http", 2048, NULL, 1, NULL);
    xTaskCreate(task1, "doing something with http", 2048, NULL, 2, NULL);

    // Eşit priority'ler kullanınca watchdog timer ile resetlenme oldu
    // listenForHTTP'ye yüksek öncelik verince watchdog ile resetleme oldu.
    // task 1'e yüksek priority verince
    /*
        received http message 
        added message to queue
        doing something with http 722
        received http message 
        added message to queue
        doing something with http 723
    */



}