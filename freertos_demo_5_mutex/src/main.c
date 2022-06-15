#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_log.h"
#include "sdkconfig.h"

#include "freertos/semphr.h"

xSemaphoreHandle mutexBus; // Burada semaphore flag'i oluşturuyorum.
                           // Buradaki xSemaphoreHandle bayrağı mutexBus'ı meşgul eder ve bu meşgülken
                           // hiç kimse onunla konuşamaz. 
void writeToBus(char *message)
{
    printf(message);
}

void task1(void *params)
{
    while(true)
    {
        printf("reading temperature \n");

        /*
            semaphore içermesi gereken bir fonksiyondur. Bu semaphore de daha önce oluşturulmuş
            olmalıdır. diğer zaman parameteresi de semaphore'nin kullanılabilir hale gelmesi için 
            beklenen süredir. Bunu gerçek zamana dönüştürmek için "portTICK_PERIOD_MS" makrosu 
            kullanılabilir. Semaphore'yi sorgulamak için sıfır blok zamanı kullanılabilir. "portMAX_DELAY"
            blok süresi, süresiz olarak engellemek için kullanılabilir.
            Biz de burada öyle yaptık.
        */
        if(xSemaphoreTake(mutexBus, 1000 / portTICK_PERIOD_MS))
        {
            writeToBus("temperature is 25 c\n");
            xSemaphoreGive(mutexBus); // Bu olmazsa okuyamayız
        }
        else
        {
            printf("writing temperature timed out \n");
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void task2(void *params)
{
    while(true)
    {
        printf("reading humidty\n");

        // 
        if(xSemaphoreTake(mutexBus, 1000 / portTICK_PERIOD_MS))
        {
            writeToBus("humidity is 50 \n");
            xSemaphoreGive(mutexBus); // Bu olmazsa okuyamayız
        }
        else
        {
            printf("writing humidty timed out \n");
        }

        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}



void app_main() 
{
    mutexBus = xSemaphoreCreateMutex(); // Burada mutexBus'ı mutex oluşturma fonksiyonuna eşitliyoruz.
    xTaskCreate(&task1, "temperaature reading", 2048, NULL, 2, NULL);
    xTaskCreate(&task2, "humidity reading", 2048, NULL, 2, NULL);

}

/*
    Hiçbir şekilde hum. ve temp. değerlerini okuyamıyoruz. Bunun sebebi mutex'in hiçbir şeye izin vermemesidir.
    xSemaphoreTake fonksiyonu buna izin vermiyor, nasıl aktive ederiz? 
    Bunun için tekrardan writeToBus fonksiyonlarına xSemaphoreGive yazarak aktive ediyoruz. 
*/