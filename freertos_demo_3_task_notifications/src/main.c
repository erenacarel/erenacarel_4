#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_log.h"
#include "sdkconfig.h"

static TaskHandle_t receiverHandler = NULL;

void sender(void *params)
{
    while(true)
    {
        xTaskNotifyGive(receiverHandler); // Tetikleme olduğunda burası çalışsın.
                                          // Göndericinin bir şeyin olması gerektiğini alıcıya bildirebileceği bir fonksiyondur.
        xTaskNotifyGive(receiverHandler);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void receiver(void *params)
{
    while(true)
    {
        int count = ulTaskNotifyTake(pdFALSE, portMAX_DELAY); // ikinci parametre timer
        /*
            Burada delay yazmaya gerek yok. Bu fonksiyon zaten bunu sağlamaktadır.
            Yukarıdaki fonksiyondan cevap gelene kadar çalışmaz.
        */
        printf("received notification %d times\n", count);
    }
}

void app_main() 
{
    xTaskCreate(receiver, "sender", 2048, NULL, 2, &receiverHandler); // bu alıcıya "receiveHandler"in adresini yazacağız.
                                                                      // yani bu görev bir kez oluşturlduktan sonra artık o görevim için bir handle'a sahip olduğum anlamına gelir.
    xTaskCreate(sender, "receiver", 2048, NULL, 2, NULL);
}