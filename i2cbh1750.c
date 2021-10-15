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
 *    ======== i2cmasterexample1.c ========
 */
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>

/* Driver Header files */
#include <ti/drivers/I2C.h>
#include <ti/display/Display.h>

/* Driver configuration */
#include "ti_drivers_config.h"

#include "includes.h"

#define BH1750_ADDRESS 0x23
#define CMD_PWN_ON 0x01
#define CMD_RESET 0x07
#define MTERGH 0x49
#define MTERGL 0x6C //设置透光率为 100%
#define MESE_CMD 0x11  //测试命令

static Display_Handle display;

/* Buffers used in this code example */
uint8_t txBuffer[10]={CMD_PWN_ON, CMD_RESET, MTERGH, MTERGL };
uint8_t rxBuffer[10];
uint8_t mirrorRegister[26] = "x Hello this is master";

int lua ;

/*
 *  ======== mainThread ========
 */
void *BH1750_Thread(void *arg0)
{
    bool            retVal = false;
    I2C_Handle      i2c;
    I2C_Params      i2cParams;
    I2C_Transaction i2cTransaction;

    /* Call driver init functions */
    Display_init();
    I2C_init();

    /* Create I2C for usage */
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_100kHz;
    i2c = I2C_open(CONFIG_I2C_0, &i2cParams);

    if (i2c == NULL) {
#ifdef NORMAL_DEBUG
        DEBUG_printf( "Error Initializing I2C!\n ");
#endif
        while (1) {}
    }
#ifdef NORMAL_DEBUG
    DEBUG_printf("Master: the BH1750 device\n");
    DEBUG_printf((char *)"I2C Slave Address: 0x%x", BH1750_ADDRESS);
#endif

    int i;
    for (i = 0; i < 5; i++)
    {
        i2cTransaction.slaveAddress = BH1750_ADDRESS;
        i2cTransaction.writeCount = 1;
        i2cTransaction.writeBuf = txBuffer[i];
        i2cTransaction.readCount = 0;
        i2cTransaction.readBuf = rxBuffer;
        /* Re-try writing to slave till I2C_transfer returns true */
        do {
            retVal = I2C_transfer(i2c, &i2cTransaction);
        } while(!retVal);
    }


    while(1)
    {
        txBuffer[0] = MESE_CMD;
        i2cTransaction.slaveAddress = BH1750_ADDRESS;
        i2cTransaction.writeCount = 1;
        i2cTransaction.writeBuf = txBuffer;
        i2cTransaction.readCount = 0;
        i2cTransaction.readBuf = rxBuffer;
        /* Re-try writing to slave till I2C_transfer returns true */
        do {
            retVal = I2C_transfer(i2c, &i2cTransaction);
        } while(!retVal);

        usleep(20);

        i2cTransaction.slaveAddress = BH1750_ADDRESS;
        i2cTransaction.writeCount = 0;
        i2cTransaction.writeBuf = txBuffer;
        i2cTransaction.readBuf = mirrorRegister;
        i2cTransaction.readCount = 2;

        /* Re-try reading from slave till I2C_transfer returns true */
        do {
            retVal = I2C_transfer(i2c, &i2cTransaction);
        } while(!retVal);


        lua = (mirrorRegister[0]*256+ mirrorRegister[1]) * 10 / 6; // 测量值 * 2 /1.2
        sleep(1);
    }
    /* Deinitialized I2C */
    I2C_close(i2c);
#ifdef NORMAL_DEBUG
    DEBUG_printf("I2C closed!\n");
#endif

    return (0);
}


