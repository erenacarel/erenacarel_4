#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_system.h"

// #include "driver/gpio.h"
// #include "esp_system.h"
// #include "esp_spi_flash.h"
#include "esp_log.h"
#include "sdkconfig.h"

#include "freertos/semphr.h"

#define BUTTON_PIN 12
#define LED_PIN    27

/*
   Semaphore global bir değişkeni kilitliyor.
   Başka bir peripheraldan o değişkene müdahale eklenmesini önlüyor.
   Veya birden fazla process'in eş zamanlı çalışması halinde birbirleri
   için risk teşkil ettikleri kritik zamanlarda birbirlerini beklemesini
   sağlayan mekanizmadır. Semaforlar üzerinde kilitleme ve kilidi kaldırma 
   yerine semafor değerini artırma ve azaltma şeklinde işlemler yapılabilir.

   Semaforların çalışması sırasında birden fazla iş tek bir iş gibi 
   algılanmalı ve araya başka işin girmesine izin verilmemelidir.
*/

SemaphoreHandle_t xSemaphore = NULL;

TaskHandle_t myTask1 = NULL;
TaskHandle_t myTask2 = NULL;

void IRAM_ATTR button_isr_handler(void *arg)
{
    xSemaphoreGive(xSemaphore);
    vTaskDelay(pdMS_TO_TICKS(5000));
}

void task1(void *arg)
{
    while(1)
    {
        printf("sent message! [%d] \n", xTaskGetTickCount());
        xSemaphoreGive(xSemaphore);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}


void task2(void *arg)
{
    while(1)
    {
        if(xSemaphoreTake(xSemaphore, portMAX_DELAY))
        {
            printf("got message! [%d] \n", xTaskGetTickCount());
        }
    }
}


void app_main() 
{
    gpio_pad_select_gpio(BUTTON_PIN);
    gpio_pad_select_gpio(LED_PIN);

    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(LED_PIN, GPIO_MODE_DEF_OUTPUT);

    gpio_set_intr_type(BUTTON_PIN, GPIO_INTR_NEGEDGE);

    gpio_install_isr_service(0);

    gpio_isr_handler_add(BUTTON_PIN, button_isr_handler, NULL);




    xSemaphore = xSemaphoreCreateBinary();
    xTaskCreate(task1, "task1", 4096, NULL, 10, &myTask1);
    xTaskCreatePinnedToCore(task2, "task2", 4096, NULL, 10, &myTask2, 1);

}