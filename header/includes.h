/*
 * includes.h
 *
 *  Created on: 2021年7月29日
 *      Author: sundm
 */

#ifndef INCLUDES_H_
#define INCLUDES_H_

#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

/* Driver Header files */
#include <ti/display/Display.h>
#include "uart.h"

/* Driver Configuration */
#include "ti_drivers_config.h"

#include "spi_lcd.h"
#include "lcd_font.h"
#include "ec11.h"
#include "ui.h"
#include "param.h"
#include "encoder.h"
#include "motor.h"
#include "event.h"

#include <ti/drivers/apps/Button.h>

#include "ti/sysbios/knl/Clock.h"

#include "param.h"
#include "pid.h"

#include "receiver.h"

#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>

#include "struct_typedef.h"

#define FALSE 0U
#define TRUE 1U

extern fp32 speed_wheel[4];

typedef struct _IMU3
{
    float acc[3];
    float gyro[3];
    float agl[3];
} IMU3;

typedef struct _IMU_ANGLE
{
    float yaw;
    float pit;
    float rol;
    float temp;
} IMU_ANGLE;


#define UART_MAX_READ_SIZE       128
typedef struct
{
    size_t       size;
    uint8_t      buf[UART_MAX_READ_SIZE];
}recv_T;

extern Display_Handle displayHandle;
extern IMU3 imu3;
extern IMU_ANGLE imu;
extern int lua ;
extern uint8_t gPixel[128];
extern int lua ;

//
//extern pid_type_def motor_speed_pid[4];             //motor speed PID.底盘电机速度pid
//extern pid_type_def chassis_angle_pid;              //follow angle PID.底盘跟随角度pid
//extern pid_type_def chassis_speed_pid;

void Console_Print( const char *buffer);

#define __DEBUG

#ifdef __DEBUG
#define DEBUG_printf(format, ...) Display_printf(displayHandle, 0, 0,format, ##__VA_ARGS__);
#define DEBUG(format, ...) Display_printf(displayHandle, 0, 0,"FILE: "__FILE__", LINE: %d: "format"/n", __LINE__, ##__VA_ARGS__);
#else
#define DEBUG_printf(format, ...)
#define DEBUG(format, ...)
#endif

#endif /* INCLUDES_H_ */
