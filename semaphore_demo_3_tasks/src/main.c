#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

SemaphoreHandle_t xSemaphore = NULL;

void Task1()
{
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    for(size_t i = 0; i < 3; i++)
    {
        printf("Task1 %d: %lld\n", i, esp_timer_get_time()/1000);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    printf("\n");
    xSemaphoreGive(xSemaphore);
    vTaskDelete(NULL);
}


void Task2()
{
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    for(size_t i = 0; i < 3; i++)
    {
        printf("Task2 %d: %lld\n", i, esp_timer_get_time()/1000);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    printf("\n");
    xSemaphoreGive(xSemaphore);
    vTaskDelete(NULL);
}


void Task3()
{
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    for(size_t i = 0; i < 3; i++)
    {
        printf("Task3 %d: %lld\n", i, esp_timer_get_time()/1000);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    printf("\n");
    xSemaphoreGive(xSemaphore); // Bu komutla semaforları alıyor
    vTaskDelete(NULL);
}

/*
    vTaskDelete(NULL) komutunu kullanmayınca sadece ilk Task'ı yapmakta ve ardından 
    kendini resetlemekte 
*/


void app_main() 
{
    printf("\n");
    xSemaphore = xSemaphoreCreateMutex();

    xTaskCreate(Task1, "Task1", 2048, NULL, 1, NULL);
    xTaskCreate(Task2, "Task2", 2048, NULL, 1, NULL);
    xTaskCreate(Task3, "Task3", 2048, NULL, 1, NULL);

}

/*
    Task1 0: 19
    Task1 1: 1013
    Task1 2: 2013

    Task2 0: 3013
    Task2 1: 4013
    Task2 2: 5013

    Task3 0: 6013
    Task3 1: 7013
    Task3 2: 8013
*/


