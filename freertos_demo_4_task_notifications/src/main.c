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
        // xTaskNotify(receiverHandler, (1<<0), eSetValueWithOverwrite); // eSetValueWithOverwrite --> Görev bildirim değerini(notification value), önceki değer görev tarafından henüz okunmamış olsa bile belirli bir değere ayarlar 
        // vTaskDelay(5000 / portTICK_PERIOD_MS);
        // xTaskNotify(receiverHandler, (1<<1), eSetValueWithOverwrite);
        // vTaskDelay(5000 / portTICK_PERIOD_MS);  // mevcut değeri yazdırıyor. 2'1 = 2 
        // xTaskNotify(receiverHandler, (1<<2), eSetValueWithOverwrite);
        // vTaskDelay(5000 / portTICK_PERIOD_MS); // ve hep başa sardırıyopr.

        xTaskNotify(receiverHandler, (1<<0), eSetBits); // Bu da bir önceki değeri saklayıp toplama yapıyor.
        //vTaskDelay(5000 / portTICK_PERIOD_MS); // Buradaki delay'i kaldırmak üsteki fonksiyonla alttaki fonksiyonun değerlerinin toplanmasını sağlıyor.
        xTaskNotify(receiverHandler, (1<<1), eSetBits);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        xTaskNotify(receiverHandler, (1<<2), eSetBits);
        vTaskDelay(5000 / portTICK_PERIOD_MS);


    }

}

void receiver(void *params)
{
    while(true)
    {
        uint state;
        // Nasıl ateşlendiği öğreneceğiz peki
        // xTaskNotifyWait(0, 0, &state, portMAX_DELAY); 
        xTaskNotifyWait(0xffffffff, 0, &state, portMAX_DELAY);

        /*
            Bütün bitlerin girişini 1'liyor. eSetValueWithOverwrite fonksiyonu ile ilgili bir
            sorun yok. Zaten son değeri güncelleyip başa sardırıyor.
            Aşağıdaki eSetBits fonksiyonu son değerde kalıyordu. Onu güncellemek için 0xffffffff 
            kullandık.
        */


        /*
            Burada delay yazmaya gerek yok. Bu fonksiyon zaten bunu sağlamaktadır.
            Yukarıdaki fonksiyondan cevap gelene kadar çalışmaz.
        */
        printf("received state %d times\n", state);
    }
}

void app_main() 
{
    xTaskCreate(receiver, "sender", 2048, NULL, 2, &receiverHandler); // bu alıcıya "receiveHandler"in adresini yazacağız.
                                                                      // yani bu görev bir kez oluşturlduktan sonra artık o görevim için bir handle'a sahip olduğum anlamına gelir.
    xTaskCreate(sender, "receiver", 2048, NULL, 2, NULL);
}