#ifndef _PARAM_H
#define _PARAM_H

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

}param_ty;

extern param_ty param;


#endif
