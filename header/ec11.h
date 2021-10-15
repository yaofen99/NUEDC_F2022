/*
 * ec11.h
 *
 *  Created on: 2021年2月24日
 *      Author: sundm
 */

#ifndef CODE_EC11_H_
#define CODE_EC11_H_

enum EC11                                                  /**< ★ 按键状态 */
{
    NO_PRESS = 0, KEY_SHORT , KEY_DOUBLE, KEY_INCREASE, KEY_DECREASE,
    KEY_LONG_INCREASE, KEY_LONG_DECREASE
};
void ec11_read(enum EC11 *ec11_state);
void EC11_GPIO_Init(void);
void Encoder_EC11_Init(unsigned char Set_EC11_TYPE);
int Encoder_EC11_Scan(void);
int Encoder_EC11_Analyze(int EC11_Value);

#endif /* CODE_EC11_H_ */
