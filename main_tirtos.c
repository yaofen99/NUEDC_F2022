
/*
 * Copyright (c) 2016-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,

 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== main_tirtos.c ========
 */
#include <stdint.h>

/* POSIX Header files */
#include <pthread.h>

/* RTOS header files */
#include <ti/sysbios/BIOS.h>

/* Driver configuration */
#include <ti/drivers/Board.h>

#include "includes.h"

extern void *motor_pwm_Thread(void *arg0);
extern void *TSL1401_adc_Thread(void *arg0);
extern void *consoleThread(void *arg0);
extern void *uart_Thread(void *arg0);
extern void *buttonled_Thread(void *arg0);
extern void *BH1750_Thread(void *arg0);
extern void *lcd_Thread(void *arg0);


extern void *encoder_Thread(void *arg0);
extern void *beep_pwm_Thread(void *arg0);
//@user add
extern void *speed_control_Thread(void *arg0);
extern void *speed_measure_Thread(void *arg0);

/* Stack size in bytes */
#define THREADSTACKSIZE    1024

Void clk0Fxn(UArg arg0);
Void clk1Fxn(UArg arg0);

Clock_Struct clk0Struct, clk1Struct;
Clock_Handle clk2Handle;

/*
 *  ======== main ========
 */
int main(void)
{
    pthread_t           thread;
    pthread_attr_t      attrs;
    struct sched_param  priParam;
    int                 retc;

    /* Call driver init functions */
    Board_init();


    /* Initialize the attributes structure with default values */
    pthread_attr_init(&attrs);

    /*motor pwm output*/
    /* Set priority, detach state, and stack size attributes */
    priParam.sched_priority = 2;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    retc |= pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    if (retc != 0) {
        /* failed to set attributes */
        while (1) {}
    }

    retc = pthread_create(&thread, &attrs, motor_pwm_Thread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1) {}
    }

    /* adc */
    priParam.sched_priority = 1;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    if (retc != 0) {
        /* failed to set priority */
        while (1) {}
    }

    retc = pthread_create(&thread, &attrs, TSL1401_adc_Thread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1) {}
    }

    /*console config */
    priParam.sched_priority = 3;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    if (retc != 0) {
        /* failed to set priority */
        while (1) {}
    }

    retc = pthread_create(&thread, &attrs, consoleThread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1) {}
    }

    /*uart config */
    priParam.sched_priority = 4;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    if (retc != 0) {
        /* failed to set priority */
        while (1) {}
    }

    retc = pthread_create(&thread, &attrs, uart_Thread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1) {}
    }

    /*button led config */
    priParam.sched_priority = 5;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    if (retc != 0) {
        /* failed to set priority */
        while (1) {}
    }

    retc = pthread_create(&thread, &attrs, buttonled_Thread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1) {}
    }

    /*BH1750_Thread config */
    priParam.sched_priority = 6;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    if (retc != 0) {
        /* failed to set priority */
        while (1) {}
    }

    retc = pthread_create(&thread, &attrs, BH1750_Thread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1) {}
    }

    /*encoder_Thread config */
    pthread_attr_init(&attrs);

    /* Set priority, detach state, and stack size attributes */
    priParam.sched_priority = 8;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    if (retc != 0) {
        /* failed to set attributes */
        while (1) {}
    }

    retc = pthread_create(&thread, &attrs, encoder_Thread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1) {}
    }


    /*beep_Thread config */
    priParam.sched_priority = 9;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    if (retc != 0) {
        /* failed to set priority */
        while (1) {}
    }

    retc = pthread_create(&thread, &attrs, beep_pwm_Thread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1) {}
    }

    /*beep_Thread config */
    priParam.sched_priority = 9;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    if (retc != 0) {
        /* failed to set priority */
        while (1) {}
    }

    retc = pthread_create(&thread, &attrs, speed_control_Thread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1) {}
    }

    /*beep_Thread config */
    priParam.sched_priority = 10;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    if (retc != 0) {
        /* failed to set priority */
        while (1) {}
    }

    retc = pthread_create(&thread, &attrs, speed_measure_Thread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1) {}
    }

    /*lcd_Thread config */
    pthread_attr_init(&attrs);

    /* Set priority, detach state, and stack size attributes */
    priParam.sched_priority = 1;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    if (retc != 0) {
        /* failed to set attributes */
        while (1) {}
    }
    retc |= pthread_attr_setstacksize(&attrs, 0x4000);
    retc = pthread_create(&thread, &attrs, lcd_Thread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1) {}
    }

    Clock_Params clockParams;

    Clock_Handle myClock;
    Error_Block eb;
    Error_init(&eb);
    Clock_Params_init(&clockParams);
    clockParams.period =  500;
    // 周期性打开中断回调函数有概率爆bug
    clockParams.startFlag = FALSE;
    // 只使用了定时功能，把下面的 NULL 替换成需要的函数handle就可
    myClock = Clock_create(NULL,  0, &clockParams, &eb);

    BIOS_start();

    return (0);
}




/*
 *  ======== clk0Fxn =======
 */
Void clk0Fxn(UArg arg0)
{
    UInt32 time;
    time = Clock_getTicks();
    DEBUG_printf("System time in clk0Fxn = %lu\n", (ULong)time);
}

/*
 *  ======== clk1Fxn =======
 */
Void clk1Fxn(UArg arg0)
{
    UInt32 time;

    time = Clock_getTicks();
    DEBUG_printf("System time in clk1Fxn = %lu\n", (ULong)time);
    DEBUG_printf("Calling BIOS_exit() from clk1Fxn\n");
    //BIOS_exit(0);
}
