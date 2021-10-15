/*
 * beep.c
 *
 *  Created on: 2021��10��10��
 *      Author: sundm
 */

#include <unistd.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/PWM.h>

/* Driver configuration */
#include "ti_drivers_config.h"

/*
 *  ======== mainThread ========
 *  Task beepplayer the PWM duty for the on board beep.
 */

PWM_Handle pwm7 = NULL; //beep

int beep_set(uint16_t freq, uint8_t volume)
{
    uint32_t period, pulse;
    /* ��Ƶ��ת��Ϊ���� ���ڵ�λ:ns Ƶ�ʵ�λ:HZ */
    period = 1000000 / (uint32_t)freq;  //unit:us 1/HZ*10^6 = us

    /* ����������С����ռ�ձ� �������͵�ƽ���� */
    pulse = period - period / 100 * volume;

    PWM_setPeriod(pwm7,period);
    PWM_setDuty(pwm7,pulse);

    return 0;
}

int beep_init(void)
{
    /* Period and duty in microseconds */
    uint16_t   pwmPeriod = 500;

    PWM_Params params;

    /* Call driver init functions. */
    PWM_init();

    PWM_Params_init(&params);
    params.dutyUnits = PWM_DUTY_US;
    params.dutyValue = 100;
    params.periodUnits = PWM_PERIOD_US;
    params.periodValue = pwmPeriod;
    pwm7 = PWM_open(CONFIG_PWM_7, &params);
    if (pwm7 == NULL) {
        /* CONFIG_PWM_0 did not open */
        while (1);
    }
    PWM_start(pwm7);
}

int beep_on(void)
{
    PWM_start(pwm7); //ʹ�ܷ�������Ӧ�� PWM ͨ��

    return 0;
}

int beep_off(void)
{
    PWM_stop(pwm7); //ʧ�ܷ�������Ӧ�� PWM ͨ��

    return 0;
}

void *beep_pwm_Thread(void *arg0)
{
    uint32_t i;
    uint16_t music[]={256, 288, 320, 341, 384, 426, 480, 512};
    beep_init();

    beep_set(0,0);

    /* Loop forever incrementing the PWM duty */
    while (1) {
        for(i=0; i<8;i++)
        {
           // beep_set(music[i],90);
            sleep(1);
        }
    }
}
