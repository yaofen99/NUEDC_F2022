#include <unistd.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/PWM.h>
#include <ti/drivers/GPIO.h>

/* Driver configuration */
#include "ti_drivers_config.h"
#include "motor.h"

#include "includes.h"


int encoder1_value = 0;
int encoder2_value = 0;
int encoder3_value = 0;
int encoder4_value = 0;

void ENCODER_1A_CB(uint_least8_t index)
{
    /* Clear the GPIO interrupt and toggle an LED */
    GPIO_toggle(CONFIG_LED_0_GPIO);
    if(0 == GPIO_read(ENCODER_1B))
        encoder1_value++;
    else
        encoder1_value--;
}

void ENCODER_1B_CB(uint_least8_t index)
{
    /* Clear the GPIO interrupt and toggle an LED */
    GPIO_toggle(CONFIG_LED_1_GPIO);
    if(0 == GPIO_read(ENCODER_1A))
        encoder1_value--;
    else
        encoder1_value++;
}

void ENCODER_2A_CB(uint_least8_t index)
{
    /* Clear the GPIO interrupt and toggle an LED */
    GPIO_toggle(CONFIG_LED_0_GPIO);
    if(0 == GPIO_read(ENCODER_2B))
        encoder2_value++;
    else
        encoder2_value--;
}

void ENCODER_2B_CB(uint_least8_t index)
{
    /* Clear the GPIO interrupt and toggle an LED */
    GPIO_toggle(CONFIG_LED_1_GPIO);
    if(0 == GPIO_read(ENCODER_2A))
        encoder2_value--;
    else
        encoder2_value++;
}
void ENCODER_3A_CB(uint_least8_t index)
{
    /* Clear the GPIO interrupt and toggle an LED */
    GPIO_toggle(CONFIG_LED_0_GPIO);
    if(0 == GPIO_read(ENCODER_3B))
        encoder3_value++;
    else
        encoder3_value--;
}

void ENCODER_3B_CB(uint_least8_t index)
{
    /* Clear the GPIO interrupt and toggle an LED */
    GPIO_toggle(CONFIG_LED_1_GPIO);
    if(0 == GPIO_read(ENCODER_3A))
        encoder3_value--;
    else
        encoder3_value++;
}
void ENCODER_4A_CB(uint_least8_t index)
{
    /* Clear the GPIO interrupt and toggle an LED */
    GPIO_toggle(CONFIG_LED_0_GPIO);
    if(0 == GPIO_read(ENCODER_4B))
        encoder4_value++;
    else
        encoder4_value--;
}

void ENCODER_4B_CB(uint_least8_t index)
{
    /* Clear the GPIO interrupt and toggle an LED */
    GPIO_toggle(CONFIG_LED_1_GPIO);
    if(0 == GPIO_read(ENCODER_4A))
        encoder4_value--;
    else
        encoder4_value++;
}


int get_encoder1(void)
{
  return -encoder1_value;
}

int get_encoder2(void)
{
  return encoder2_value;
}

int get_encoder3(void)
{
  return -encoder4_value;
}

int get_encoder4(void)
{
  return encoder3_value;
}


void *speed_measure_Thread(void *arg0)
{
    static int last_encoder[4];
    static UInt32 last_time;


    GPIO_init();

    /*encoder1*/
    GPIO_setConfig(ENCODER_1A, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    GPIO_setConfig(ENCODER_1B, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    /* install Button callback */
    GPIO_setCallback(ENCODER_1A, ENCODER_1A_CB);
    GPIO_setCallback(ENCODER_1B, ENCODER_1B_CB);
    /* Enable interrupts */
    GPIO_enableInt(ENCODER_1A);
    GPIO_enableInt(ENCODER_1B);

    /*encoder2*/
    GPIO_setConfig(ENCODER_2A, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    GPIO_setConfig(ENCODER_2B, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    /* install Button callback */
    GPIO_setCallback(ENCODER_2A, ENCODER_2A_CB);
    GPIO_setCallback(ENCODER_2B, ENCODER_2B_CB);
    /* Enable interrupts */
    GPIO_enableInt(ENCODER_2A);
    GPIO_enableInt(ENCODER_2B);

    /*encoder3*/
    GPIO_setConfig(ENCODER_3A, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    GPIO_setConfig(ENCODER_3B, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    /* install Button callback */
    GPIO_setCallback(ENCODER_3A, ENCODER_3A_CB);
    GPIO_setCallback(ENCODER_3B, ENCODER_3B_CB);
    /* Enable interrupts */
    GPIO_enableInt(ENCODER_3A);
    GPIO_enableInt(ENCODER_3B);

    /*encoder4*/
    GPIO_setConfig(ENCODER_4A, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    GPIO_setConfig(ENCODER_4B, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    /* install Button callback */
    GPIO_setCallback(ENCODER_4A, ENCODER_4A_CB);
    GPIO_setCallback(ENCODER_4B, ENCODER_4B_CB);
    /* Enable interrupts */
    GPIO_enableInt(ENCODER_4A);
    GPIO_enableInt(ENCODER_4B);



    while (1){
    int encoder[4];
    uint32_t time;
    time = Clock_getTicks();

    encoder[0] = get_encoder1();
    encoder[1] = get_encoder2();
    encoder[2] = get_encoder3();
    encoder[3] = get_encoder4();


    int i = 0;
    for (; i<4 ;i++){
        speed_wheel[i] = (encoder[i] - last_encoder[i])*100/(float)(time - last_time);
    }

//    DEBUG_printf("%d,%d,%d,%f\n",encoder[0],last_encoder[0],encoder[0]-last_encoder[0],speed_wheel[0]);
//    DEBUG_printf("%f,%f,%f,%f \n",speed_wheel[0],speed_wheel[1],speed_wheel[2],speed_wheel[3]);

//    DEBUG_printf("%f,%f,%f",motor_speed_pid[0].Kp,motor_speed_pid[0].Ki,motor_speed_pid[0].Kd);


    last_encoder[0] = encoder[0];
    last_encoder[1] = encoder[1];
    last_encoder[2] = encoder[2];
    last_encoder[3] = encoder[3];
    last_time = time;

    usleep(100);
    }


}
