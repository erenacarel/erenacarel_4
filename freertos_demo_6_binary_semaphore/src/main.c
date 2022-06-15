#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_log.h"
#include "sdkconfig.h"

xSemaphoreHandle binSemaphore; // bir önceki örnekte mutexBus kullandıydık.

void listenForHTTP(void *params)
{
    while(true)
    {
        printf("received http message \n");
        xSemaphoreGive(binSemaphore); // Bunu yazacağız ki mesajları okuyalım.
        printf("processed http message \n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);

    }
}

void task1(void *params)
{
    while(true)
    {
        xSemaphoreTake(binSemaphore, portMAX_DELAY); // Burası uykuya geçirme yani etkisizleştirme yeridir, çünkü portMAX_DELAY kullandık.
                                                     // Herhangi bir CPU tutmuyor, hiçbir şey yapmıyor. 
                                                     // Bu fonksiyon gerçekten o semaforu verdiğinde FreeRTOS bayrağı kaldırır ve semaforu uyandırır.
                                                     // Böylece aşağıdaki yazı yazdırılır ve while döngüsünde kod işler. Tekrar aynı yere gelir 
                                                     // ve tekrar uyur.
        printf("doing something with http\n");

    }
}


void app_main() 
{
    binSemaphore = xSemaphoreCreateBinary(); // Burada binSemaphore'yi semaphore oluşturma fonksiyonuna eşitliyoruz.
    //xTaskCreate(&listenForHTTP, "get http", 2048, NULL, 2, NULL);
    //xTaskCreate(&task1, "do something with http", 2048, NULL, 1, NULL);
    /*
        received http message
        processed http message 
        doing something with http
    */

    //xTaskCreate(&listenForHTTP, "get http", 2048, NULL, 1, NULL);
    //xTaskCreate(&task1, "do something with http", 2048, NULL, 2, NULL);
    
    /*
        received http message
        doing something with http
        processed http message
    */

    //xTaskCreate(&listenForHTTP, "get http", 2048, NULL, 1, NULL);
    //xTaskCreate(&task1, "do something with http", 2048, NULL, 1, NULL);
    
    /*
        received http message
        doing something with http
        processed http message
    */


}