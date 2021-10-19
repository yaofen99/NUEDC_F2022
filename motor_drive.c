/*
    Function of this Thread is to take in the number of speed and apply it
    Transform the speed to pwm and apply
    With the some lower laiers like PID and PWM
 */

#include <unistd.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/PWM.h>

/* Driver configuration */
#include "ti_drivers_config.h"

//#include "PID.h"
/*
 *  ======== mainThread ========
 *  Task periodically increments the PWM duty for the on board LED.
 */

//this thread is used for maintain the inner loop (mainly pid calculation of )
void *motor_pwm_Thread(void *arg0)
{
    /* Period and duty in microseconds */
    uint16_t   pwmPeriod = 3000;
    uint16_t   duty = 0;
    int16_t   duty_motor = 0, dir_flag = 0;
    int16_t   dutyInc = 100;

    /* Sleep time in microseconds */
    uint32_t   time = 50;
    PWM_Handle pwm1 = NULL;
    PWM_Handle pwm2 = NULL;

    PWM_Params params;

    /* Call driver init functions. */
    PWM_init();

    PWM_Params_init(&params);
    params.dutyUnits = PWM_DUTY_US;
    params.dutyValue = 0;
    params.periodUnits = PWM_PERIOD_US;
    params.periodValue = pwmPeriod;
    pwm1 = PWM_open(CONFIG_PWM_0, &params);
    if (pwm1 == NULL) {
        /* CONFIG_PWM_0 did not open */
        while (1);
    }

    PWM_start(pwm1);

    pwm2 = PWM_open(CONFIG_PWM_1, &params);
    if (pwm2 == NULL) {
        /* CONFIG_PWM_0 did not open */
        while (1);
    }

    PWM_start(pwm2);

    motor_init();

    /* Loop forever incrementing the PWM duty */
    while (1) {
//        PWM_setDuty(pwm1, duty);
//        PWM_setDuty(pwm2, pwmPeriod - duty);
//
//        duty = (duty + dutyInc);
//
//        if (duty == pwmPeriod || (!duty)) {
//            dutyInc = - dutyInc;
//        }
//
//        if((duty_motor>=pwmPeriod)||(duty_motor<=-pwmPeriod))
//            dir_flag = !dir_flag;
//        if(dir_flag==0)
//            duty_motor +=100;
//        else
//            duty_motor -=100;
//        motor_left_right_duty(duty_motor, duty_motor);
//        motor_duty(0,duty_motor);
//        motor_duty(1,duty_motor);
//        motor_duty(2,duty_motor);
//        motor_duty(3,duty_motor);
        usleep(time);
    }
}
