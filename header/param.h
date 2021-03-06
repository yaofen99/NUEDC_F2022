/*
 * main adjustment file
 * */

#ifndef _PARAM_H
#define _PARAM_H

#include "includes.h"

                                                            /*  debug mode  */
//#define NORMAL_DEBUG
//#define VOFA_DEBUG
#define SERIAL_PID_DEBUG


                                                       /*  PID controller params  */
#define M3505_MOTOR_SPEED_PID_KP 1
#define M3505_MOTOR_SPEED_PID_KI 0.005
#define M3505_MOTOR_SPEED_PID_KD 0

#define CHASSIS_FOLLOW_GIMBAL_PID_KP 1
#define CHASSIS_FOLLOW_GIMBAL_PID_KI 0
#define CHASSIS_FOLLOW_GIMBAL_PID_KD 0

//PID amplitude limit
#define ANGLE_PID_MAX_OUT 1000
#define ANGLEL_PID_MAX_IOUT 1000

#define M3505_MOTOR_SPEED_PID_MAX_OUT   2900
#define M3505_MOTOR_SPEED_PID_MAX_IOUT  2500

#define CHASSIS_ACCEL_X_NUM 1
#define CHASSIS_ACCEL_Y_NUM 1

#define CHASSIS_CONTROL_TIME 0.002f


typedef struct
{
    uint8_t sector_num;
    uint8_t * data_addr;
    uint32_t data_size;
}flash_data;


typedef struct
{
  float x;
  float y;
  float z;
}XYZ;


//typedef struct pid
//{
//      float ref;     //系统待调节量的设定值
//      float fdb;  //系统待调节量的反馈值，就是传感器实际测量的值
//
//      float KP;                 //比例系数
//      float KI;                 //积分系数
//      float KD;                 //微分系数
//
//      float T;        //离散化系统的采样周期
//
//      float a0;         //变量代替三项的运算结果
//      float a1;
//      float a2;
//
//      float error;    //当前偏差e（k）
//      float error_1; //前一步的偏差
//      float error_2; //前前一步的偏差
//
//      float output;      //pid控制器的输出
//      float output_1;      //pid的前一步输出
//      float out_max;      //输出上限
//      float out_min;      //输出下限
//}PID_value;

typedef struct
{
    uint8_t mode;
    //PID 三参数
    float Kp;
    float Ki;
    float Kd;

    float max_out;  //最大输出
    float max_iout; //最大积分输出

    float set;
    float fdb;

    float out;
    float Pout;
    float Iout;
    float Dout;
    float Dbuf[3];  //微分项 0最新 1上一次 2上上次
    float error[3]; //误差项 0最新 1上一次 2上上次

    float speed_set;

} pid_type_def;

typedef struct
{
    float bend_steer_kp;
    float bend_steer_ki;
    float bend_steer_kd;
    float motor_kp;
    float motor_ki;
    float motor_kd;
    float circle_pid_ratio;
    int   velocity;
    int circle_size[4];
    int circle_turndis[4];
    int circle_ad[4];
    float circle_in_angle[4];
    float circle_exit_angle[4];
    int circle_num;
    int circle_en;

    pid_type_def speed_pid;

    pid_type_def single_motor_pid[4];

}param_ty;


extern param_ty param;


#endif
