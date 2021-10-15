/*
 * Copyright (c) 2015-2019, Texas Instruments Incorporated
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
 *  ======== gpiointerrupt.c ========
 */
#include <stdint.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>

/* Driver configuration */
#include "ti_drivers_config.h"

int encoder1_value = 0;
int encoder2_value = 0;
int encoder3_value = 0;
int encoder4_value = 0;

void ENCODER_1A_CB(uint_least8_t index)
{
    /* Clear the GPIO interrupt and toggle an LED */
    GPIO_toggle(CONFIG_LED_0_GPIO);
    if(0 == GPIO_read(ENCODER_1B))
        encoder1_value++;
    else
        encoder1_value--;
}

void ENCODER_1B_CB(uint_least8_t index)
{
    /* Clear the GPIO interrupt and toggle an LED */
    GPIO_toggle(CONFIG_LED_1_GPIO);
    if(0 == GPIO_read(ENCODER_1A))
        encoder1_value--;
    else
        encoder1_value++;
}

void ENCODER_2A_CB(uint_least8_t index)
{
    /* Clear the GPIO interrupt and toggle an LED */
    GPIO_toggle(CONFIG_LED_0_GPIO);
    if(0 == GPIO_read(ENCODER_2B))
        encoder2_value++;
    else
        encoder2_value--;
}

void ENCODER_2B_CB(uint_least8_t index)
{
    /* Clear the GPIO interrupt and toggle an LED */
    GPIO_toggle(CONFIG_LED_1_GPIO);
    if(0 == GPIO_read(ENCODER_2A))
        encoder2_value--;
    else
        encoder2_value++;
}
void ENCODER_3A_CB(uint_least8_t index)
{
    /* Clear the GPIO interrupt and toggle an LED */
    GPIO_toggle(CONFIG_LED_0_GPIO);
    if(0 == GPIO_read(ENCODER_3B))
        encoder3_value++;
    else
        encoder3_value--;
}

void ENCODER_3B_CB(uint_least8_t index)
{
    /* Clear the GPIO interrupt and toggle an LED */
    GPIO_toggle(CONFIG_LED_1_GPIO);
    if(0 == GPIO_read(ENCODER_3A))
        encoder3_value--;
    else
        encoder3_value++;
}
void ENCODER_4A_CB(uint_least8_t index)
{
    /* Clear the GPIO interrupt and toggle an LED */
    GPIO_toggle(CONFIG_LED_0_GPIO);
    if(0 == GPIO_read(ENCODER_4B))
        encoder4_value++;
    else
        encoder4_value--;
}

void ENCODER_4B_CB(uint_least8_t index)
{
    /* Clear the GPIO interrupt and toggle an LED */
    GPIO_toggle(CONFIG_LED_1_GPIO);
    if(0 == GPIO_read(ENCODER_4A))
        encoder4_value--;
    else
        encoder4_value++;
}


void *encoder_Thread(void *arg0)
{
    GPIO_init();

    /*encoder1*/
    GPIO_setConfig(ENCODER_1A, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    GPIO_setConfig(ENCODER_1B, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    /* install Button callback */
    GPIO_setCallback(ENCODER_1A, ENCODER_1A_CB);
    GPIO_setCallback(ENCODER_1B, ENCODER_1B_CB);
    /* Enable interrupts */
    GPIO_enableInt(ENCODER_1A);
    GPIO_enableInt(ENCODER_1B);

    /*encoder2*/
    GPIO_setConfig(ENCODER_2A, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    GPIO_setConfig(ENCODER_2B, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    /* install Button callback */
    GPIO_setCallback(ENCODER_2A, ENCODER_2A_CB);
    GPIO_setCallback(ENCODER_2B, ENCODER_2B_CB);
    /* Enable interrupts */
    GPIO_enableInt(ENCODER_2A);
    GPIO_enableInt(ENCODER_2B);

    /*encoder3*/
    GPIO_setConfig(ENCODER_3A, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    GPIO_setConfig(ENCODER_3B, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    /* install Button callback */
    GPIO_setCallback(ENCODER_3A, ENCODER_3A_CB);
    GPIO_setCallback(ENCODER_3B, ENCODER_3B_CB);
    /* Enable interrupts */
    GPIO_enableInt(ENCODER_3A);
    GPIO_enableInt(ENCODER_3B);

    /*encoder4*/
    GPIO_setConfig(ENCODER_4A, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    GPIO_setConfig(ENCODER_4B, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    /* install Button callback */
    GPIO_setCallback(ENCODER_4A, ENCODER_4A_CB);
    GPIO_setCallback(ENCODER_4B, ENCODER_4B_CB);
    /* Enable interrupts */
    GPIO_enableInt(ENCODER_4A);
    GPIO_enableInt(ENCODER_4B);


    return (NULL);
}

int get_encoder1(void)
{
  return -encoder1_value;
}

int get_encoder2(void)
{
  return encoder2_value;
}

int get_encoder3(void)
{
  return -encoder4_value;
}

int get_encoder4(void)
{
  return encoder3_value;
}
