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


#define ledyesil 26
#define ledsari 27

const int buton1 = 12;
const int buton2 = 14;

bool butondurum1 = false;
bool butondurum2 = false;

// Interrupta bir şey gönderirken Queue(kuyruk) kullanılır.
QueueHandle_t interruptQueue1;
QueueHandle_t interruptQueue2;

static const char *TAG = "isr_demo_3";

// Butona basılıp basılmadığını kontrol eden kesme(interrupt) fonksiyonu
// Başına static ekleyerek başka .c dosyasında kullanılmasını önlüyorum.
static void IRAM_ATTR buton1control(void *args)
{
    int pinNumber = (int)args;
    xQueueSendFromISR(interruptQueue1, &pinNumber, NULL);

}  


static void IRAM_ATTR buton2control(void *args)
{
    int pinNumber = (int)args;
    xQueueSendFromISR(interruptQueue2, &pinNumber, NULL);
}

void buton1Pushed(void *params)
{
    int pinNumber = 0;
    ESP_LOGI(TAG, "buton1Pushed\n");
   
    while(true)
    {
        if(xQueueReceive(interruptQueue1, &pinNumber, portMAX_DELAY))
        {
            gpio_set_level(GPIO_NUM_27, 0);
       
                gpio_set_level(GPIO_NUM_26, 1);
                ESP_LOGI(TAG, "Yesil led yandi\n");
                butondurum1 = !butondurum1;
            

        }
        //gpio_isr_handler_remove(pinNumber1);
    }
    //gpio_isr_handler_add(pinNumber1, buton1control, (void*)pinNumber1);
}

void buton2Pushed(void *params)
{
    int pinNumber = 0;
    ESP_LOGI(TAG, "buton2Pushed\n");
    
    while(true)
    {
        if(xQueueReceive(interruptQueue2, &pinNumber, portMAX_DELAY))
        {
            gpio_set_level(GPIO_NUM_26, 0);
           
                gpio_set_level(GPIO_NUM_27, 1);
                ESP_LOGI(TAG, "Sari led yandi\n");
                butondurum2 = !butondurum2;
            

        }
        //gpio_isr_handler_remove(pinNumber2);
    }
    //gpio_isr_handler_add(pinNumber2, buton2control, (void*)pinNumber2);
}

void app_main() 
{
    ESP_LOGI(TAG, "start\n");
    gpio_pad_select_gpio(buton1);
    gpio_set_direction(buton1, GPIO_MODE_INPUT);
    gpio_pullup_dis(buton1);
    gpio_pulldown_en(buton1);

    gpio_pad_select_gpio(buton2);
    gpio_set_direction(buton2, GPIO_MODE_INPUT);
    gpio_pullup_dis(buton2);
    gpio_pulldown_en(buton2);

    gpio_pad_select_gpio(ledyesil);
    gpio_set_direction(ledyesil, GPIO_MODE_OUTPUT);

    gpio_pad_select_gpio(ledsari);
    gpio_set_direction(ledsari, GPIO_MODE_OUTPUT);

    gpio_set_intr_type(buton1, GPIO_INTR_ANYEDGE);
    interruptQueue1 = xQueueCreate(10, sizeof(int)); // Kuyruk oluşturmak lazım yoksa kod sonsuza kadar akar
    xTaskCreate(buton1Pushed, "buton1Pushed", 4096, NULL, 1, NULL);

    gpio_set_intr_type(buton2, GPIO_INTR_ANYEDGE);
    interruptQueue2 = xQueueCreate(10, sizeof(int));
    xTaskCreatePinnedToCore(buton2Pushed, "buton2Pushed", 4096, NULL, 1, NULL,1);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(buton1, buton1control, (void*)buton1); // attachInterrupt
    gpio_isr_handler_add(buton2, buton2control, (void*)buton2); // attachInterrupt

}