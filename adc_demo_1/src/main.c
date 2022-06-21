#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
//#include "driver/dac.h"

/*
    Potansiyometre kullanarak yaptığım bir ADC örneğidir.
    potansiyometrenin bir bacağı 3.3V diğeri gnd, diğeri de
    ESP32'nin VP pinine bağlıdır. Yani ADC1_CH0, CH0, VP pinine
    karşılık gelmektedir. 
*/

#define NO_OF_SAMPLE 64

/*
    Vref giriş voltajını ölçmek için ESP32 ADC'leri tarafından dahili olarak kullanılan referans voltajlarıdır. 
    ESP32 ADC'leri 0V ile referans voltajı arasındaki volajı ölçebilir. Vref'den daha büyük voltajları dönüştürmek
    için giriş voltajları ADC'lere girilmeden önce azaltılabilir. Kullanılabilir 4 tane zayıflama seçeneği vardır.(attenuation)
    zayıflama ne kadar yüksekse ölçülebilir giriş voltajı o kadar yüksek olabilir.
*/

static const adc_channel_t channel = ADC_CHANNEL_0;
static const adc_bits_width_t width = ADC_WIDTH_BIT_12; // 2^12 = 4096-1 = 4095
static const adc_atten_t atten = ADC_ATTEN_DB_11; // en uygunu bu oldu, ADC_ATTEN_MAX ters okudu.

void app_main() 
{
    // Configure ADC
    adc1_config_width(width);
    adc1_config_channel_atten(channel, atten);

    /* 
        Mesela 12 tane 1'i yan yana koyduk. Bu 4096 etmektedir.
        Varsayılan zayıflama 0 DB'dir. Daha yüksek zayıflama ile daha yüksek voltajları da okuyabiliriz.
    */

   while(1)
   {
    uint32_t adc_reading = 0;

    for(int i = 0; i < NO_OF_SAMPLE; i++)
    {
        adc_reading += adc1_get_raw(channel); 
    }

    adc_reading /= NO_OF_SAMPLE;

    // convert adc_reading to voltage in V
    float voltage = adc_reading * 3.3 / 4095.0; 
    
    printf("Raw: %d\t  Voltage: %f\n", adc_reading, voltage);
    

   }

}
