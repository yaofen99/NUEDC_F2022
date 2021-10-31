/*
 *  ======== adcsinglechannel.c ========
 */
#include <stdint.h>
#include <stddef.h>

/* POSIX Header files */
#include <pthread.h>

/* Driver Header files */
#include <ti/drivers/ADC.h>
#include <ti/drivers/GPIO.h>
#include <ti/display/Display.h>

/* Driver configuration */
#include "ti_drivers_config.h"
#include "includes.h"

/* ADC sample count */

#define THREADSTACKSIZE   (768)


/*=======================TSL1401  ��ض���  ========================*/
#define TSL1401_SI(x)       GPIO_write(TSL1401_SI_GPIO, x);
#define TSL1401_CLK(x)     GPIO_write(TSL1401_CLK_GPIO, x);
#define PRINT_AD        1       //���ô��ڴ�ӡ�������ͣ�0:��ӡ��ֵ�����ݣ�1����ӡADֵ
#define THRESHOLD       10     //���ö�ֵ����ֵ
#define WINDOW_WIDTH    128     //���ô��ڴ�ӡ�����ظ��������128����С0

//ȫ�ֱ�������
uint8_t gPixel[128] = {0};
ADC_Handle   adc;
/*
 * ��ʱ��΢��
 */
void delay()
{

}

void TSL1401_GetLine(ADC_Handle  adc, uint8_t *pixel)
{
  uint8_t i;
  int_fast16_t res;
  uint16_t ad_value;
  //��ʼSI
  TSL1401_SI(0) ;
  TSL1401_CLK(0);
  delay();
  TSL1401_SI(1);
  delay();
  TSL1401_CLK(1);
  delay();
  TSL1401_SI(0);
  delay();
  //�ɼ���1����
  res = ADC_convert(adc, &ad_value);
  *pixel = ad_value ; pixel++;
  TSL1401_CLK(0);

  //�ɼ���2~128����
  for(i=1; i<128; i++)
  {
    delay();
    TSL1401_CLK(1);
    delay();
    res = ADC_convert(adc, &ad_value);
    *pixel = ad_value ; pixel++;
    TSL1401_CLK(0);
  }
  //���͵�129��clk
   delay();
   TSL1401_CLK(1);
   delay();
   TSL1401_CLK(0);
   delay();
}
/*
 *  ======== threadFxn0 ========
 *  Open an ADC instance and get a sampling result from a one-shot conversion.
 */
void *TSL1401_adc_Thread(void *arg0)
{
    ADC_Params   params;
    int i;
    uint8_t temp;

    /* Call driver init functions */
    ADC_init();
    GPIO_init();

    ADC_Params_init(&params);
    adc = ADC_open(CONFIG_TSL1401_ADC, &params);

    if (adc == NULL) {
        while (1);
    }
    else
    {
    }
    while(1)
    {
        /* Blocking mode conversion */
        TSL1401_GetLine(adc, gPixel);
        usleep(50);
    }
    ADC_close(adc);

}
