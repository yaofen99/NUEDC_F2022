/*
 * motor.c
 *
 *  Created on: 2021年10月10日
 *      Author: sundm
 */
#include <unistd.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>

/* Driver configuration */
#include "ti_drivers_config.h"
#include "motor.h"

/* Driver Header files */
#include <ti/drivers/PWM.h>
#include <ti/drivers/GPIO.h>

/* Driver configuration */
#include "ti_drivers_config.h"
#include "motor.h"

#include "includes.h"

/*00 停止 01正转 10 反转*/
#define AIN_1(x)     GPIO_write(AIN1, x);
#define AIN_2(x)     GPIO_write(AIN2, x);
#define BIN_1(x)     GPIO_write(BIN1, x);
#define BIN_2(x)     GPIO_write(BIN2, x);
#define CIN_1(x)     GPIO_write(CIN1, x);
#define CIN_2(x)     GPIO_write(CIN2, x);
#define DIN_1(x)     GPIO_write(DIN1, x);
#define DIN_2(x)     GPIO_write(DIN2, x);

int left_duty = 0;
int right_duty = 0;

#define MOTOR_DEAD_VAL 0         //the max of duty 10000

void motor_dir(uint8_t motor_num, uint8_t dir) // motor_num ： 0、1、2、3  dir： 0正转 1反转 2停止
{
    switch(motor_num)
    {
    case 0:
        if((dir == 0) || (dir == 1))
        {
            if(MOTOR0_DIR == 0){
                AIN_1(dir);AIN_2(!dir);}
            else{
                AIN_2(dir);AIN_1(!dir);}
        }
        else
        {
            AIN_1(0);AIN_2(0);
        }
        break;
    case 1:
        if((dir == 0) || (dir == 1))
        {
            if(MOTOR1_DIR == 0){
                BIN_1(dir);BIN_2(!dir);}
            else{
                BIN_2(dir);BIN_1(!dir);}
        }
        else
        {
            BIN_1(0);BIN_2(0);
        }
        break;
    case 2:
        if((dir == 0) || (dir == 1))
        {
            if(MOTOR2_DIR == 0){
                CIN_1(dir);CIN_2(!dir);}
            else{
                CIN_2(dir);CIN_1(!dir);}
        }
        else
        {
            CIN_1(0);CIN_2(0);
        }
        break;
    case 3:
        if((dir == 0) || (dir == 1))
        {
            if(MOTOR3_DIR == 0){
                DIN_1(dir);DIN_2(!dir);}
            else{
                DIN_2(dir);DIN_1(!dir);}
        }
        else
        {
            DIN_1(0);DIN_2(0);
        }
        break;
    default:
        break;
    }
}


PWM_Handle pwm0 = NULL;
PWM_Handle pwm1 = NULL;
PWM_Handle pwm2 = NULL;
PWM_Handle pwm3 = NULL;

void motor_init(void)
{
    /* Period and duty in microseconds */
    uint16_t   pwmPeriod = 3000;
    uint16_t   duty = 0;
    uint16_t   dutyInc = 100;

    /* Sleep time in microseconds */
    uint32_t   time = 50000;
    PWM_Params params;

    /* Call driver init functions. */
    PWM_init();

    PWM_Params_init(&params);
    params.dutyUnits = PWM_DUTY_US;
    params.dutyValue = 0;
    params.periodUnits = PWM_PERIOD_US;
    params.periodValue = pwmPeriod;

    pwm0 = PWM_open(CONFIG_PWM_2, &params);
    if (pwm0 == NULL) {
        /* CONFIG_PWM_0 did not open */
        while (1);
    }

    PWM_start(pwm0);

//    PWM_setDuty(pwm0, 800);

    pwm1 = PWM_open(CONFIG_PWM_3, &params);
    if (pwm1 == NULL) {
        /* CONFIG_PWM_0 did not open */
        while (1);
    }

    PWM_start(pwm1);

    pwm2 = PWM_open(CONFIG_PWM_4, &params);
    if (pwm2 == NULL) {
        /* CONFIG_PWM_0 did not open */
        while (1);
    }

    PWM_start(pwm2);

    pwm3= PWM_open(CONFIG_PWM_5, &params);
    if (pwm3 == NULL) {
        /* CONFIG_PWM_0 did not open */
        while (1);
    }

    PWM_start(pwm3);

    //初始化方向控制引脚
    motor_dir(0,0);
    motor_dir(1,0);
    motor_dir(2,0);
    motor_dir(3,0);
}

void motor_left_right_duty(int left_duty, int right_duty)
{
      if(0 <= left_duty) //电机左边2个   正转
      {
          PWM_setDuty(pwm0, left_duty);
          motor_dir(0,0);
          PWM_setDuty(pwm3, left_duty);
          motor_dir(3,0);
      }
      else                //电机左边2个   反转
      {
          PWM_setDuty(pwm0, -left_duty);
          motor_dir(0,1);
          PWM_setDuty(pwm3, -left_duty);
          motor_dir(3,1);
      }
      if(0 <= right_duty) //电机右边2个   正转
      {
          PWM_setDuty(pwm1, right_duty);
          motor_dir(1,0);
          PWM_setDuty(pwm2, right_duty);
          motor_dir(2,0);
      }
      else                //电机右边2个   反转
      {
          PWM_setDuty(pwm1, -right_duty);
          motor_dir(1,1);
          PWM_setDuty(pwm2, -right_duty);
          motor_dir(2,1);
      }
}

void motor_duty(int motor_num, int duty)
{
    if(duty > 0)
        duty += MOTOR_DEAD_VAL;
    else
        duty -= MOTOR_DEAD_VAL;

    if(duty > PWM_MAX)
        duty = PWM_MAX;
    else if(duty < -PWM_MAX)
        duty = -PWM_MAX;

    switch(motor_num)
    {
    case 0:
        if(duty>=0){
        PWM_setDuty(pwm0, duty);
        motor_dir(0,0);}
        else{
            PWM_setDuty(pwm0, -duty);
            motor_dir(0,1);}
        break;
    case 1:
        if(duty>=0){
        PWM_setDuty(pwm1, duty);
        motor_dir(1,0);}
        else{
        PWM_setDuty(pwm1, -duty);
        motor_dir(1,1);}
        break;
    case 2:
        if(duty>=0){
        PWM_setDuty(pwm2, duty);
        motor_dir(2,0);}
        else{
        PWM_setDuty(pwm2, -duty);
        motor_dir(2,1);}
        break;
    case 3:
        if(duty>=0){
        PWM_setDuty(pwm3, duty);
        motor_dir(3,0);}
        else{
        PWM_setDuty(pwm3, -duty);
        motor_dir(3,1);}
        break;
    default:
        break;
    }

}



