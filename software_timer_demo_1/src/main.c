#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "esp_timer.h" // esp_timer_create() gibi fonksiyonlar
#include "sdkconfig.h"

// xTimer zamanlayıcı tanımlayıcısı
void myTimerCallBack1(TimerHandle_t xTimer)
{
    printf("MyTimer CallBack 1\n");

}

void myTimerCallBack2(TimerHandle_t xTimer)
{
    printf("MyTimer CallBack 2\n");

    const char *strTimerName;
    strTimerName = pcTimerGetName(xTimer);
    printf("Timer Name = %s  ,", strTimerName);

    int id = 0;
    id = (int) pvTimerGetTimerID(xTimer); // int'e cast ediyoruz.
    printf(" Timer ID = %d \n", id);


}

void app_main() 
{
    TimerHandle_t myTimer1 = NULL;
    TimerHandle_t myTimer2 = NULL;

    myTimer1 = xTimerCreate("myTimer1", pdMS_TO_TICKS(1000), pdTRUE, (void*)1, myTimerCallBack1);
    myTimer2 = xTimerCreate("myTimer2", pdMS_TO_TICKS(2000), pdTRUE, (void*)2, myTimerCallBack2);

    printf("Timer start \n");
    xTimerStart(myTimer1, 0); 
    xTimerStart(myTimer2, 0); // ikinci parametre xTickToWait() parametresidir. xTimerStop() çağırıldığında
                              // kuyruğun zaten dolu olması durumunda durdurma komutunun timer komut kuyruğuna
                              // başarılı bir şekilde gönderilmesini beklemek için çağıran görevin engellenmiş
                              // durumda tutulması gereken süreyi tik olarak belirtir. Zamanlayıcı başaltılmadan 
                              // önce xTimerStop() çağırılsa xTickToWait() yoksayılır.

    
    

    /*
        pdTRUE diye yazdığımız şey uxAutoReload parametresidir. Bunu pdTRUE olarak ayarlarsak, timer'ımız
        ikinci yerdeki xTimerPeriodInTicks parametresinde ayarlanan frekansla tekrar tekrar sona erer.  Eğer 
        pdFALSE olarak ayarlanırsa, timer tek seferlik(one-shot) bir timer olur ve uyku durumuna(dormant state)
        girer.

        (void*)1 diye yazdığımız dördüncü parametre timer'a atanan bir tip tanımlayıcıdır. Tipik olarak bu, aynı
        callback fonksiyonu birden fazla timer'a atandığında süresi dolan callback fonksiyonunu tanımlamak için 
        kullanılır.

        (void*)2 yazımı önceliği artıryor, (void*)1 yazımı daha düşük öncelik.

        myTimer1 = xTimerCreate("myTimer1", pdMS_TO_TICKS(1000), pdTRUE, (void*)1, myTimerCallBack1);
        myTimer2 = xTimerCreate("myTimer2", pdMS_TO_TICKS(2000), pdTRUE, (void*)2, myTimerCallBack2);

        Timer start
        MyTimer CallBack 1
        MyTimer CallBack 2
        Timer Name = myTimer2  , Timer ID = 2
        MyTimer CallBack 1
        MyTimer CallBack 1
        MyTimer CallBack 2
        Timer Name = myTimer2  , Timer ID = 2
        MyTimer CallBack 1
        MyTimer CallBack 1
        MyTimer CallBack 2
        Timer Name = myTimer2  , Timer ID = 2
        MyTimer CallBack 1
        MyTimer CallBack 1
        MyTimer CallBack 2

        myTimer1 = xTimerCreate("myTimer1", pdMS_TO_TICKS(1000), pdTRUE, (void*)2, myTimerCallBack1);
        myTimer2 = xTimerCreate("myTimer2", pdMS_TO_TICKS(2000), pdTRUE, (void*)1, myTimerCallBack2);

        Timer start
        MyTimer CallBack 1
        MyTimer CallBack 2
        Timer Name = myTimer2  , Timer ID = 1
        MyTimer CallBack 1
        MyTimer CallBack 1
        MyTimer CallBack 2
        Timer Name = myTimer2  , Timer ID = 1
        MyTimer CallBack 1
        MyTimer CallBack 1
        MyTimer CallBack 2
        Timer Name = myTimer2  , Timer ID = 1
        MyTimer CallBack 1
        MyTimer CallBack 1
        MyTimer CallBack 2



        myTimer1 = xTimerCreate("myTimer1", pdMS_TO_TICKS(1000), pdTRUE, (void*)1, myTimerCallBack1);
        myTimer2 = xTimerCreate("myTimer2", pdMS_TO_TICKS(2000), pdTRUE, (void*)1, myTimerCallBack2);

        Timer start
        MyTimer CallBack 1
        MyTimer CallBack 2
        Timer Name = myTimer2  , Timer ID = 1
        MyTimer CallBack 1
        MyTimer CallBack 1
        MyTimer CallBack 2
        Timer Name = myTimer2  , Timer ID = 1
        MyTimer CallBack 1
        MyTimer CallBack 1
        MyTimer CallBack 2




    */



}