/*
 * uart.c
 *
 *  Created on: 2021年7月29日
 *      Author: sundm
 *  Thread for debug and slot drawing
 */


#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "includes.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

/* BIOS module Headers */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Mailbox.h>


/* POSIX Header files */
#include <pthread.h>
#include <semaphore.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/Timer.h>

/* Driver configuration */
#include "ti_drivers_config.h"
#include <ti/drivers/utils/RingBuf.h>

/* Display Header files */
#include <ti/display/Display.h>

#include "includes.h"

#define UART_MAX_READ_SIZE       128
int uart_ready_flag = 0;
/*event variables*/
Event_Struct ecoEvent;
static Event_Handle ecoEventHd;

/*uart */
static   UART_Handle uart1 = NULL;
static   UART_Handle uart2 = NULL;
static   UART_Handle uart3 = NULL;
static   recv_T   uart1Read;
static   uint8_t  write_buf[UART_MAX_READ_SIZE];

/*================= MPU9250 ===============*/


IMU3 imu3;
IMU_ANGLE imu;

void mpu9250_data_analyse(uint8_t* receive_buf);

/*set uart1 receive callback*/
static void uart1ReadCallback(UART_Handle handle, void *rxBuf, size_t size)
{
    uart1Read.size = uart1Read.size + size;
    Event_post(ecoEventHd,ECO_UART_RECV_EVT);
}
/*set uart1 write callback*/
static void uartWriteCallback(UART_Handle handle, void *writeBuf, size_t size)
{

}

/*set uart2 receive callback*/
static void uart2ReadCallback(UART_Handle handle, void *rxBuf, size_t size)
{

}
/*set uart2 write callback*/
static void uart2WriteCallback(UART_Handle handle, void *writeBuf, size_t size)
{

}
static void uart3ReadCallback(UART_Handle handle, void *rxBuf, size_t size)
{

}

/*set uart3 read callback*/
static void uart3WriteCallback(UART_Handle handle, void *rxBuf, size_t size)
{

}

uint8_t CheckSum(uint8_t *Buf,uint8_t Len)
{
  uint8_t i =0;
  uint8_t sum =0;
  uint8_t checksum =0;

  for(i=0; i<Len; i++)
  {
    sum += *Buf++;
  }
  checksum = sum & 0xff;
  return checksum;
}

/*接收后事件*/
static void handleRecvEvt(void)
{
    static uint8_t rx_index = 0;

    /*接收首字节0x55及 和校验正确*/
    if((0x55 == uart1Read.buf[0]) && (uart1Read.buf[10] == CheckSum(uart1Read.buf, 10)))
    {
        rx_index = 0;
        /*一帧接收结束  进行处理*/
        Event_post(ecoEventHd,ECO_MOVE_DATA_EVT);
        return;
    }
    /*接收首字节0x55*/
    if(0x55 == uart1Read.buf[rx_index])
    {
        memset( uart1Read.buf, 0x00, sizeof(uart1Read.buf));
        uart1Read.size = 1;
        uart1Read.buf[0] = 0x55;
        rx_index = 0;
    }
    /*正常接收下一字节*/
    {
        rx_index ++ ;
        /*接收下一个字节*/
        UART_read(uart1, &uart1Read.buf[rx_index], 1);
    }
}

void imu_data_analyse(uint8_t* receive_buf)
{
    switch (receive_buf[1])
    {
        case (0x51) :
        {
            imu3.acc[0]=(int16_t)(receive_buf[3]<<8|receive_buf[2])*9.8*16/32768;
            imu3.acc[1]=(int16_t)(receive_buf[5]<<8|receive_buf[4])*9.8*16/32768;
            imu3.acc[2]=(int16_t)(receive_buf[7]<<8|receive_buf[6])*9.8*16/32768;
            break;
        }
        case (0x52) :
        {
            imu3.gyro[0]=(int16_t)(receive_buf[3]<<8|receive_buf[2])*2000/32768;
            imu3.gyro[1]=(int16_t)(receive_buf[5]<<8|receive_buf[4])*2000/32768;
            imu3.gyro[2]=(int16_t)(receive_buf[7]<<8|receive_buf[6])*2000/32768;
            break;
        }
        case (0x53) :
        {
            imu3.agl[0]=((int16_t)(receive_buf[3]<<8|receive_buf[2]))*180/ 32768;
            imu3.agl[1]=((int16_t)(receive_buf[5]<<8|receive_buf[4]))*180/ 32768;
            imu3.agl[2]=((int16_t)(receive_buf[7]<<8|receive_buf[6]))*180/ 32768;
            imu.rol = imu3.agl[0] ;
            imu.pit = imu3.agl[1] ;
            imu.yaw = imu3.agl[2];
            imu.temp = (int16_t)(receive_buf[9]<<8|receive_buf[8])/100;
            break;
        }
    }
    static int count = 0;
    count++;
    //if(0 == count%50)
    {
#ifdef NORMAL_DEBUG
        DEBUG_printf( "acc:%f,%f,%f\n",imu3.acc[0],imu3.acc[1],imu3.acc[2]);
        DEBUG_printf("gyro:%f,%f,%f\n", imu3.gyro[0],imu3.gyro[1],imu3.gyro[2]);
        DEBUG_printf( "agl:%f,%f,%f\n", imu3.agl[0],imu3.agl[1],imu3.agl[2]);
        DEBUG_printf( "temp:%f\n", imu.temp);
#endif
#ifdef VOFA_DEBUG
        /*
         * costomize debug
         * */
        DEBUG_printf( "%f,%f,%f,%f,%f,%f,%f,%f,%f\n",
                      imu3.acc[0],imu3.acc[1],imu3.acc[2],
                      imu3.gyro[0],imu3.gyro[1],imu3.gyro[2],
                      imu3.agl[0],imu3.agl[1],imu3.agl[2],imu.temp);
#endif
    }
}

/*处理一帧字符事件*/
static void handleMoveEvt(void)
{
    memset(write_buf, 0x00, sizeof(write_buf));
    memcpy(write_buf, uart1Read.buf, uart1Read.size);
//    ecoWriteData(write_buf, uart1Read.size);
    if(0x55 == uart1Read.buf[0])
    {
        imu_data_analyse(uart1Read.buf);
    }
    memset( uart1Read.buf, 0x00, sizeof(uart1Read.buf));
    uart1Read.size = 0;
    /*接收下一帧数据*/
    UART_read(uart1, &uart1Read.buf[0], 1);
}

/*配置串口*/
/*init uart*/
static void User_uart_init(void)
{
    /* Initialize UART */
    UART_Params uartParams;
    UART_init();

    /* Open UART in callback mode for both read and write */
    UART_Params_init(&uartParams);
    uartParams.writeDataMode  = UART_DATA_BINARY;
    uartParams.readDataMode   = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readMode       = UART_MODE_CALLBACK;
    uartParams.writeMode      = UART_MODE_CALLBACK;
    uartParams.readCallback   = uart1ReadCallback;
    uartParams.writeCallback  = uartWriteCallback;
    uartParams.readEcho       = UART_ECHO_OFF;
    uartParams.baudRate       = 9600;
    uartParams.stopBits       = UART_STOP_ONE;
    uartParams.parityType     = UART_PAR_NONE;
    uartParams.dataLength     = UART_LEN_8;

    /*open CONFIG_UART_1 ,get handle */
    uart1 = UART_open(CONFIG_UART_1, &uartParams);

    if (uart1 == NULL)
    {
      /* UART_open() failed*/
      while (1);
    }

    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readMode       = UART_MODE_CALLBACK;
    uartParams.writeMode      = UART_MODE_CALLBACK;
    uartParams.readCallback   = uart2ReadCallback;
    uartParams.writeCallback  = uart2WriteCallback;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 115200;

    uart2 = UART_open(CONFIG_UART_2, &uartParams);

    if (uart2 == NULL) {
        /* UART_open() failed */
        while (1);
    }

    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readMode       = UART_MODE_CALLBACK;
    uartParams.writeMode      = UART_MODE_CALLBACK;
    uartParams.readCallback   = uart3ReadCallback;
    uartParams.writeCallback  = uart3WriteCallback;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 115200;

    uart3 = UART_open(CONFIG_UART_3, &uartParams);

    if (uart3 == NULL) {
        /* UART_open() failed */
        while (1);
    }

    uart_ready_flag = 1;
    UART_write(uart1, "Uart1 init!", 10);
    UART_write(uart2, "Uart2 init!", 10);
    UART_write(uart3, "Uart3 init!", 10);


    /*start receive data from RX*/
    UART_read(uart1, uart1Read.buf, 1);
}

/*初始化任务*/
static void UartTaskInit(void)
{
    /*event init*/
    Event_Params ecoEvtParam;
    Event_Params_init(&ecoEvtParam);
    Event_construct(&ecoEvent,&ecoEvtParam);
    ecoEventHd = Event_handle(&ecoEvent);

    User_uart_init();
}
/*任务函数*/
void *uart_Thread(void *arg0)
{
     UartTaskInit();
    /* Application uart1 loop */
    while(1)
    {
        uint32_t events;
        events = Event_pend(ecoEventHd, Event_Id_NONE, ET_ALL_EVENTS,
                            BIOS_WAIT_FOREVER );
        if(events & ECO_UART_RECV_EVT)
        {
            handleRecvEvt();
        }
        if(events & ECO_MOVE_DATA_EVT)
        {
            handleMoveEvt();
        }
    }
}

/*其它程序调用写出函数*/
void uart2_WriteData(unsigned char *buf,unsigned short size)
{
    UART_write(uart2,buf,size);
}
/*其它程序调用写出函数*/
void uart3_WriteData(unsigned char *buf,unsigned short size)
{
    UART_write(uart3,buf,size);
}
void uart3_SendData(unsigned char dat)
{
    unsigned char tmp = dat;
    UART_write(uart3,&tmp,1);
    usleep(1);
}

/*逐飞上位机CCD协议*/
void ccd_send_data(uint8_t *dat)
{
   uint8_t i = 0;
   uint8_t temp[4] = {0x00, 0xff, 0x01, 0x00};
   uart3_SendData(0x00);
   uart3_SendData(0xFF);
   uart3_SendData(0x01);
   uart3_SendData(0x00);

    for(i=0; i<128; i++)
    {
        uart3_SendData(dat[i]);   //发送8位
    }
}

/*红树伟业CCD上位机协议*/
void ccd_send_data2(uint8_t *dat)
{
   uint8_t i = 0;

    for(i=0; i<128; i++)
    {
        if(dat[i] == 0xFF)
            dat[i]  = 0XFE;
        uart3_SendData(dat[i] );
    }
    uart3_SendData(0XFF);
}
