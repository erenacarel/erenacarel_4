#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "freertos/event_groups.h"

#include "esp_timer.h"

/*
    Timer oluşturmak için kullanılması gereken fonksiyon esp_timer_create() fonksiyonu
    kullanılır. Timer silmek için esp_timer_delete() kullanılır.  
*/

/*
    esp_timer_start_once() timer'i tek seferlik modda başlatmak için çağırılır ve içerisinde
    geri aramanın(callback) çağırılacağı zaman aralığı seçilir. Callback çağırıldığında timer
    durdurulmuş olarak kabul edilir. 

    esp_timer_start_once() 20us'tan küçük bir zaman aşımı değeriyle çağırılırsa, callback
    yalnızca yaklaşık 20us sonra gönderilir. 
*/

void timer_callback(void *args)
{
    static bool on = false;
    on = !on;
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    printf("%d\n", on);
    gpio_set_level(GPIO_NUM_26, on);
}

void app_main() 
{

    gpio_pad_select_gpio(GPIO_NUM_26);
    gpio_set_direction(GPIO_NUM_26, GPIO_MODE_OUTPUT);


    const esp_timer_create_args_t esp_timer_create_args ={
        .callback = timer_callback,
        .name = "My timer",
        
    };

    esp_timer_handle_t esp_timer_handle;
    esp_timer_create(&esp_timer_create_args, &esp_timer_handle);
    //esp_timer_start_once(esp_timer_handle, 50);     // bir kere timer kullanılıyor.
    esp_timer_start_periodic(esp_timer_handle, 1000); // düzenli olarak artmasını sağlıyor.

    int x = 0;
    while(true)
    {
        esp_timer_dump(stdout); // Zamanlayıcıların listesini bir akışa boşaltıyor.
        vTaskDelay(pdMS_TO_TICKS(1000));
        // 5 kere aşağıya kaydırıyoruz. 0 ile 5 arası.
        if(x++ == 6)
        {
            esp_timer_stop(esp_timer_handle);
            esp_timer_delete(esp_timer_handle);
        }

    }

}