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


void *speed_control_Thread(void *arg0)
{
    chassis_init();
    usleep(2000);


    while(1){
        int i=0;
        //DEBUG_printf("System time in speed module = %lu\n", (ULong)time);



        //calculate pid
        //计算pid
        for (i = 0; i < 4; i++)
        {
            PID_calc(&motor_speed_pid[i], speed_wheel[i], (*motor_speed_pid[i]).speed_set);
            //@user confused
        }


  //      motor_left_right_duty(1000 , 0 );

       // motor_duty(1, 20);
       PWM_setDuty(pwm0, 800);

        usleep(200);

    }

}


//abaasdsadsadsa

first_order_filter_type_t chassis_cmd_slow_set_vx;  //use first order filter to slow set-point.使用一阶低通滤波减缓设定值
first_order_filter_type_t chassis_cmd_slow_set_vy;  //use first order filter to slow set-point.使用一阶低通滤波减缓设定值


void chassis_init(void)
{
    chassis_speed_pid = &param.speed_pid;

    motor_speed_pid[0] = &param.single_motor_pid;
//    motor_speed_pid[0] = &param.single_motor_pid[0][0];
//    motor_speed_pid[0] = &param.single_motor_pid[0][1];
//    motor_speed_pid[0] = &param.single_motor_pid[0][2];

    //chassis motor speed PID
    //底盘速度环pid值
    const static fp32 motor_speed_pid[3] = {M3505_MOTOR_SPEED_PID_KP, M3505_MOTOR_SPEED_PID_KI, M3505_MOTOR_SPEED_PID_KD};

    //chassis angle PID
    //底盘角度pid值
    const static fp32 chassis_yaw_pid[3] = {CHASSIS_FOLLOW_GIMBAL_PID_KP, CHASSIS_FOLLOW_GIMBAL_PID_KI, CHASSIS_FOLLOW_GIMBAL_PID_KD};

    const static fp32 chassis_x_order_filter[1] = {CHASSIS_ACCEL_X_NUM};
    const static fp32 chassis_y_order_filter[1] = {CHASSIS_ACCEL_Y_NUM};
    uint8_t i;

    //we do not use the initializing function as we initialize it in file param.h
    //get chassis motor data point,  initialize motor speed PID
    //获取底盘电机数据指针，初始化PID
//    for (i = 0; i < 4; i++)
//    {
//        //PID_init(&motor_speed_pid[i], PID_POSITION, &motor_speed_pid, M3505_MOTOR_SPEED_PID_MAX_OUT, M3505_MOTOR_SPEED_PID_MAX_IOUT);
//        PID_init(&motor_speed_pid[i], PID_POSITION, motor_speed_pid, M3505_MOTOR_SPEED_PID_MAX_OUT, M3505_MOTOR_SPEED_PID_MAX_IOUT);
//
//    }
    //initialize angle PID
    //初始化角度PID
    //PID_init(&chassis_angle_pid, PID_POSITION, chassis_yaw_pid, ANGLE_PID_MAX_OUT, ANGLEL_PID_MAX_IOUT);

//    DEBUG_printf("%f,%f,%f",*chassis_speed_pid.Kp,*chassis_speed_pid.Ki,*chassis_speed_pid.Kd);
//    DEBUG_printf("231disojciodjoa");
    //first order low-pass filter  replace ramp function
    //用一阶滤波代替斜波函数生成
    //first_order_filter_init(&chassis_cmd_slow_set_vx, CHASSIS_CONTROL_TIME, chassis_x_order_filter);
    //first_order_filter_init(&chassis_cmd_slow_set_vy, CHASSIS_CONTROL_TIME, chassis_y_order_filter);
}
