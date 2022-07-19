// ESP IDF EXAMPLE
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_system.h"

TaskHandle_t myTask1Handle = NULL;
TaskHandle_t myTask2Handle = NULL;
QueueHandle_t queue1;

void task1(void *arg)
{
    char txbuff[50];
    queue1 = xQueueCreate(5, sizeof(txbuff)); // Kuyruk oluşturulduğunda heap içerisinde yani RAM'de alan ayrılır.
                                              // İlk parametre kuruğa max kaç eleman gireceği belirtilir.
                                              // İkinci parametre kuyrukta depolanan verinin kaç bayt olacağı ile ilgilidir.
    if(queue1 == 0)
    {
        printf("fail %p \n", queue1);
    }

    sprintf(txbuff, "hello world 1");
    xQueueSend(queue1, &txbuff, 1000 / portTICK_PERIOD_MS); // fonksiyon void* tipinde değer istiyor, bu verinin boyutu kuyruk oluşturulurken 
                                               // tanımlanmıştır. Diğer şey bekleme parametresidir. Blok süresi tick periyoduna 
                                               // göre belirlenir. portMAXDELAY girilebilir.
                                               // Lakin adres değerinin bilgisini görmesini sağlamak da aynı sonucu verir

    // xQueueSendToback() ile xQueueSend() kuyruğun sonuna yazar, aynı işlevdedir.
    // xQueuesendToFront() kuyruğun başına yazar.

    sprintf(txbuff, "hello word 2");
    xQueueSend(queue1, &txbuff, 1000 / portTICK_PERIOD_MS );

    sprintf(txbuff, "hello world 3");
    xQueueSendToFront(queue1, &txbuff, 1000 / portTICK_PERIOD_MS);

    while(1)
    {
        printf("data waiting to be read: %d available spaces: %d \n", uxQueueMessagesWaiting(queue1), uxQueueSpacesAvailable(queue1));
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

}

// Kuyruktan veri okumak için xQueueReceive() kullanılır. Veriyi okuduktan sonra 
// okuduğu veriyi kuyruktan siler. xQueuePeek() kuyruktaki verileri silinmeden okur.


void task2(void *arg)
{
    char rxbuff[50];
    while(1)
    {
        if(xQueueReceive(queue1, &(rxbuff), (TickType_t)5 )) // rxbuff'ın adres değerinin bilgisi
        {
            printf("got data from queue ! = %s \n", rxbuff);
            vTaskDelay(1000 / portTICK_PERIOD_MS);

        }


    }



}


void app_main(void)
{

xTaskCreate(task1, "task1", 4096, NULL, (void*)10, NULL); // void siz de olur sadece 10 da çalışıyor diğerlerinde sonsuza akıyıor ????

  /*
        xTaskCreate(task1,     // task fonksiyonumuz 
                "task1",  // fonksiyon ismi
                4096,     // task'ın stack'de kaplayacağı yer
                NULL,      // task'ın parametresi
                0,        // task'ın önceliği, küçük rakam büyük öncelik
                NULL);      // oluşturulan görevin izlenmesini korumak için task handle 
  */
        xTaskCreatePinnedToCore(task2, "task2", 4096, NULL, 1, NULL, 1);
        //vTaskStartScheduler();
  

}

/*
    data waiting to be read: 3 available spaces: 2 
    got data from queue ! = hello world 3
    data waiting to be read: 2 available spaces: 3 
    got data from queue ! = hello world 1 
    data waiting to be read: 1 available spaces: 4 
    got data from queue ! = hello word 2 
    data waiting to be read: 0 available spaces: 5 
    data waiting to be read: 0 available spaces: 5 
    data waiting to be read: 0 available spaces: 5
*/
