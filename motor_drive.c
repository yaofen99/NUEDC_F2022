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

#include "includes.h"

//#include "PID.h"
/*
 *  ======== mainThread ========
 *  Task periodically increments the PWM duty for the on board LED.
 */


//imprtant param about speed
pid_type_def *motor_speed_pid[4];             //motor speed PID.底盘电机速度pid
pid_type_def *chassis_angle_pid;              //follow angle PID.底盘跟随角度pid
pid_type_def *chassis_speed_pid;

fp32 speed_set;
fp32 speed_set_motor;

fp32 speed_wheel[4];

float R4wd = 0.125;
float Rw = 0.03;
float Pi = 3.1415926;

void FourWD(float Vx, float Wz)
{
        int i=0;
        float w1,w2,w3,w4,wt;
        w1 = -((Vx/Rw)/(2*Pi))*60;
        w2 = -1*w1;
        w3 = w2;
        w4 = w1;
        wt = ((Wz*R4wd/Rw)/(2*Pi))*60*1.2222;
        w1 += wt;
        w2 += wt;
        w3 += wt;
        w4 += wt;



        motor_speed_pid[0]->speed_set  =  w1;
        motor_speed_pid[1]->speed_set  =  w2;
        motor_speed_pid[2]->speed_set  =  w3;
        motor_speed_pid[3]->speed_set  =  w4;

        //calculate pid
        //计算pid
//        for (i = 0; i < 4; i++)
//        {
//            PID_calc(motor_speed_pid[i], speed_wheel[i], motor_speed_pid[i]->speed_set);
//        }
//        motor_duty(1, motor_speed_pid[0]->out);
//        motor_duty(2, motor_speed_pid[1]->out);
//        motor_duty(3, motor_speed_pid[2]->out);
//        motor_duty(4, motor_speed_pid[3]->out);

  //      motor_left_right_duty(1000 , 0 );

//        DEBUG_printf("speed set:%f, %f, %f, %f \r\n",motor_speed_pid[0]->speed_set,motor_speed_pid[1]->speed_set,motor_speed_pid[2]->speed_set,motor_speed_pid[3]->speed_set);
//        DEBUG_printf("speed wheel:%f, %f, %f, %f \r\n",speed_wheel[1],speed_wheel[2],speed_wheel[3],speed_wheel[4]);
//        DEBUG_printf("pid.ou :%f, %f, %f, %f \r\n",motor_speed_pid[0]->out,motor_speed_pid[1]->out,motor_speed_pid[2]->out,motor_speed_pid[3]->out);

        motor_duty(1, w1);
        motor_duty(2, w2);
        motor_duty(3, w3);
        motor_duty(4, w4);

}


void chassis_init(void)
{
    chassis_speed_pid = &param.speed_pid;

    motor_speed_pid[0] = &param.single_motor_pid[0];
    motor_speed_pid[1] = &param.single_motor_pid[1];
    motor_speed_pid[2] = &param.single_motor_pid[2];
    motor_speed_pid[3] = &param.single_motor_pid[3];

    //chassis motor speed PID
    //底盘速度环pid值
    const static fp32 single_motor_speed_pid[3] = {M3505_MOTOR_SPEED_PID_KP, M3505_MOTOR_SPEED_PID_KI, M3505_MOTOR_SPEED_PID_KD};

    //chassis angle PID
    //底盘角度pid值
    const static fp32 chassis_yaw_pid[3] = {CHASSIS_FOLLOW_GIMBAL_PID_KP, CHASSIS_FOLLOW_GIMBAL_PID_KI, CHASSIS_FOLLOW_GIMBAL_PID_KD};

//    const static fp32 chassis_x_order_filter[1] = {CHASSIS_ACCEL_X_NUM};
//    const static fp32 chassis_y_order_filter[1] = {CHASSIS_ACCEL_Y_NUM};
    uint8_t i;

    for (i = 0; i < 4; i++)
    {
        //PID_init(&motor_speed_pid[i], PID_POSITION, &motor_speed_pid, M3505_MOTOR_SPEED_PID_MAX_OUT, M3505_MOTOR_SPEED_PID_MAX_IOUT);
        PID_init(motor_speed_pid[i], PID_POSITION, single_motor_speed_pid, M3505_MOTOR_SPEED_PID_MAX_OUT, M3505_MOTOR_SPEED_PID_MAX_IOUT);

    }


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

char readgpio_front;

char f1,f2,f3,f4;

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

    chassis_init();
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
        while (1){
        }
    }

    PWM_start(pwm1);

    pwm2 = PWM_open(CONFIG_PWM_1, &params);
    if (pwm2 == NULL) {
        /* CONFIG_PWM_0 did not open */
        while (1);
    }

    PWM_start(pwm2);

    motor_init();

    int speed[4],i;

    for (i=0;i<4;i++)
    {
        speed[i] = 700;
    }

    /* Loop forever incrementing the PWM duty */
    while (1) {
        static

        readgpio_front = GPIO_read(Infr_front_4) + GPIO_read(Infr_front_3)*2 + GPIO_read(Infr_front_2)*4 + GPIO_read(Infr_front_1)*8;
//       f4= GPIO_read(Infr_front_4);
//       f3= GPIO_read(Infr_front_3);
//       f2= GPIO_read(Infr_front_2);
//       f1= GPIO_read(Infr_front_1);
//       DEBUG_printf("%d,%d,%d,%d",f1,f2,f3,f4);

        switch(readgpio_front)
        {
        case OFFSET_TO_LEFT:
            speed[0]+=10;
            speed[2]+=10;
            speed[1]-=10;
            speed[3]-=10;
            break;
        case OFFSET_TO_RIGHT:
            speed[0]-=10;
            speed[2]-=10;
            speed[1]+=10;
            speed[3]+=10;
            break;
        case MOVE_TO_LEFT:
            speed[0]+=20;
            speed[2]+=20;
            speed[1]-=20;
            speed[3]-=20;
            break;
        case MOVE_TO_RIGHT:
            speed[0]-=20;
            speed[2]-=20;
            speed[1]+=20;
            speed[3]+=20;
            break;
        case NO_OFFSET:
            for (i=0;i<4;i++)
            {
                speed[i] = 700;
            }
            break;
        }

        for(i=0;i<4;i++)
        {
            motor_duty(i,speed[i]);
        }

        usleep(time);
    }
}





