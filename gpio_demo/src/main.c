#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "sdkconfig.h"

#define PIN 23
static const char *TAG = "gpio_demo";

void app_main() 
{
    gpio_pad_select_gpio(PIN);                          // pin seçimi 
    gpio_set_direction(PIN, GPIO_MODE_DEF_OUTPUT);      // pinMode 
    int isOn = 0;
    while(true)
    {
        isOn =! isOn;
        gpio_set_level(PIN, isOn);                     // digitalWrite
        //ESP_LOGI(TAG, "PIN 23 is: %d", isOn);
        printf("Pin State %d \n", isOn);
        vTaskDelay(1000 / portTICK_PERIOD_MS); 

    }

}