#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

#define TAG "NVS"

void app_main(void)
{
    // Initialize the NVS partition
    nvs_flash_init();

    // Wait for memeory initialization
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    // Define memeory handle
    nvs_handle_t my_handle;

    // Open storage with a given namespace
    // storage namespace - "storage"
    nvs_open("store", NVS_READWRITE, &my_handle);

    // Read data
    // key - "data"
    // value - "read_data"
    int32_t read_data = 100000;
    nvs_get_i32(my_handle, "data", &read_data);
    printf("Read data: %d \n", read_data);

    // Set data structure for update during the commit 
    // key - "data"
    // value - "write_data = read_data++"
    // int32_t read_data++;
    int32_t write_data = read_data++;
    nvs_set_i32(my_handle, "data", write_data);

    // write any pending changes to non-volatile storage
    printf("Write_data: %d \n", read_data);
    nvs_commit(my_handle);

    // Close the storage handle
    nvs_close(my_handle);

}

/*
    ESP flash'ı yerine bellek bölümünü başlatıyoruz. Bu şey başlatılıncaya kadar 
    1 saniye bekleme yapıyoruz. NVS içerisinde "storage" denen alanımızı açarız.
    NVS_READWRITE mode'sini seçiyoruz. my_handle'mizin adresini referans olarak
    gösteriyoruz. Hafızadaki değişimleri nvs_commit ile gösteriyoruz. Ve hafızayı
    kapatıyoruz.

*/


