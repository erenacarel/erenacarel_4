#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"

void app_main() 
{
    ledc_timer_config_t timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_10_BIT, // 10 tane 1, 11 1111 1111 = 1024
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 4000,
        .clk_cfg = LEDC_AUTO_CLK

    };

    // timer structure'sini yapılandırıyoruz. config ediyoruz.
    ledc_timer_config(&timer);

    ledc_channel_config_t eren = {
        .gpio_num = GPIO_NUM_5,
        .channel = LEDC_CHANNEL_0,
        .duty = 0, //.duty = 1023
        .hpoint = 0,
        .intr_type = LEDC_INTR_DISABLE,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_sel = LEDC_TIMER_0
        

    };

    //ledc_channel_config(&channel);
    ledc_channel_config(&eren);
    
    ledc_fade_func_install(0); 
    
    // for (int i = 0; i < 1024; i++)
    // {
    //ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, i);
    //ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 256);
    //ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 512);
    //ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 700);
    //ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    //vTaskDelay(10 / portTICK_PERIOD_MS);
    //}



}