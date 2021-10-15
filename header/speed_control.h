/*
 * motor.h
 *
 *  Created on: 2021年10月10日
 *      Author: sundm
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#define PWM_MAX 3000

#define MOTOR0_DIR 0
#define MOTOR1_DIR 1
#define MOTOR2_DIR 1
#define MOTOR3_DIR 0

extern int single_duty;
extern int left_duty;
extern int right_duty;

void motor_init(void);
void motor_duty(int motor_num, int duty);
void motor_left_right_duty(int left_duty, int right_duty);
static inline int get_left_duty(void) {return left_duty;};
static inline int get_right_duty(void) {return right_duty;};


#endif /* MOTOR_H_ */
