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

#define PIN_SWC  15
#define PIN_LED  19  

// Kuyruklar genelde FIFO buffer kullanır. Yani kuyruğa ilk yazılan veri ilk önce okunur.
// Bir veri yazılırken sondan eklenir kuyruğun başından okunur 
// İnterrupt taska bir şey gönderirken Queue kullanacağız.

/*
   Semaphore global bir değişkeni kilitliyor.
   Başka bir peripheraldan o değişkene müdahale eklenmesini önlüyor.
*/
xQueueHandle interruptQueue;



/*
   Bu fonksiyon C derleyicisine, gerçekten belirli kod parçasını çalıştırırken, onu standart 
   RAM'den çalıştırmayacağımızı, onu DRAM'den veya dynamic RAM'den çalıştıracağımızı söyleyen
   özel bir derleyici yönergesidir. Bu fonksiyon interruptlar için özeldir.
*/

// static fonksiyon diyerek başka .c dosyasında kullanılmasını önlüyorum.
static void IRAM_ATTR gpio_isr_handler(void *args)
{
   int pinNumber = (int)args;
   xQueueSendFromISR(interruptQueue, &pinNumber, NULL); // Interruptan Q'ya gönder.
}

void buttonPushedTask(void *params)
{
   int pinNumber, count = 0;
   printf("sa"); // ???
   while(true)
   {
      // interrupt tetiklendiğinde buraya gelinecek.
      if(xQueueReceive(interruptQueue, &pinNumber, portMAX_DELAY))
      {

         printf("GPIO %d was pressed %d times. The state is %d\n", pinNumber, count++, gpio_get_level(PIN_SWC));
         gpio_set_level(PIN_LED, gpio_get_level(PIN_SWC));

         // re-enable interrupt
         gpio_isr_handler_add(pinNumber, gpio_isr_handler, (void*)pinNumber);


      }
   }
}

void app_main() 
{
    printf("HEllo interrupt \n");
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    
    gpio_pad_select_gpio(PIN_LED);                      // pin ayarlaması
    gpio_set_direction(PIN_LED, GPIO_MODE_OUTPUT);      // pinMode

    gpio_pad_select_gpio(PIN_SWC);
    gpio_set_direction(PIN_SWC, GPIO_MODE_INPUT);
    gpio_pulldown_en(PIN_SWC);
    gpio_pullup_dis(PIN_SWC);

    // Sürekli while döngüsüyle uğraşmamak için interrupt kullanacağız.
    gpio_set_intr_type(PIN_SWC, GPIO_INTR_POSEDGE);  // Burada pek çok seçenek var

    interruptQueue = xQueueCreate(10, sizeof(int));
    // Bir kuyruk yaratmak için iki paramnetre vardır. İlki kuyruğun boyu, ikincisi size
    xTaskCreate(buttonPushedTask, "buttonPushedTask", 2048, NULL, 1, NULL); // 1024 ???


    gpio_install_isr_service(0);   // interrupt güncelleme
    gpio_isr_handler_add(PIN_SWC, gpio_isr_handler, (void*) PIN_SWC); // İlk yere ISR routinenin tetiklenmesini istediğmiz pini yazıyoruz.
                                                                    // İkinci yere bir callback function yazıyoruz.
                                                                    // Üçüncü yerde bu rutin handler'in içine gerçek pin numarasını parse edeceğim(ayrıştıracağım)   




}