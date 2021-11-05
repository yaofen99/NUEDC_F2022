/*
 * motor.c
 *
 *  Created on: 2021年10月10日
 *      Author: sundm
 */
#include <unistd.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/PWM.h>
#include <ti/drivers/GPIO.h>

/* Driver configuration */
#include "ti_drivers_config.h"
#include "motor.h"

#include "includes.h"



//imprtant param about speed
pid_type_def *motor_speed_pid[4];             //motor speed PID.底盘电机速度pid
pid_type_def *chassis_angle_pid;              //follow angle PID.底盘跟随角度pid
pid_type_def *chassis_speed_pid;

fp32 speed_set;
fp32 speed_set_motor;

fp32 speed_wheel[4];

void chassis_init(void);
//
//
//void *speed_control_Thread(void *arg0)
//{
//    chassis_init();
//    usleep(20000);
//
//
//    while(1){
//        int i=0;
//        //DEBUG_printf("System time in speed module = %lu\n", (ULong)time);
//
//        motor_speed_pid[0]->speed_set  =  2300;
//        motor_speed_pid[1]->speed_set  =  1500;
//        motor_speed_pid[2]->speed_set  =  2300;
//        motor_speed_pid[3]->speed_set  =  1500;
//
//
//
//        //calculate pid
//        //计算pid
//        for (i = 0; i < 4; i++)
//        {
//            PID_calc(motor_speed_pid[i], speed_wheel[i], motor_speed_pid[i]->speed_set);
//        }
//
//
//  //      motor_left_right_duty(1000 , 0 );
//
//
//        DEBUG_printf("speed set:%f, %f, %f, %f \r\n",motor_speed_pid[0]->speed_set,motor_speed_pid[1]->speed_set,motor_speed_pid[2]->speed_set,motor_speed_pid[3]->speed_set);
//
//        DEBUG_printf("speed wheel:%f, %f, %f, %f \r\n",speed_wheel[1],speed_wheel[2],speed_wheel[3],speed_wheel[4]);
//
//        DEBUG_printf("pid.ou :%f, %f, %f, %f \r\n",motor_speed_pid[0]->out,motor_speed_pid[1]->out,motor_speed_pid[2]->out,motor_speed_pid[3]->out);
//
//
//       // motor_duty(1, 20);
//
////       motor_left_right_duty(1000,1000);
//
//        for (i = 0; i < 4; i++)
//        {
//            motor_duty(i, motor_speed_pid[i]->out );
//        }
//
//
//       usleep(200);
//
//    }
//
//}


//abaasdsadsadsa

first_order_filter_type_t chassis_cmd_slow_set_vx;  //use first order filter to slow set-point.使用一阶低通滤波减缓设定值
first_order_filter_type_t chassis_cmd_slow_set_vy;  //use first order filter to slow set-point.使用一阶低通滤波减缓设定值



