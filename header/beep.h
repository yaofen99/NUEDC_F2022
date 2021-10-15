/*
* Copyright (c) 2006-2018, RT-Thread Development Team
*
* SPDX-License-Identifier: Apache-2.0
*
* Change Logs:
* Date           Author       Notes
* 2018-10-17     flybreak      the first version
*/

#ifndef BEEP_H
#define BEEP_H

#include <unistd.h>
#include <stddef.h>

int beep_init(void);
int beep_on(void);
int beep_off(void);
int beep_set(uint16_t freq, uint8_t volume);

#endif
