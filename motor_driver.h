#ifndef __MOTOR_DRIVER_H
#define __MOTOR_DRIVER_H

#include "LPC407x_8x_177x_8x.h"

// POWER
#define MOTOR_PCONP_PWM_BIT					5

// IOCON
#define MOTOR_IOCON_ENA							LPC_IOCON->P1_2
#define MOTOR_IOCON_ENB							LPC_IOCON->P1_3

#define MOTOR_IOCON_ENA_FUNC				3
#define MOTOR_IOCON_ENB_FUNC				3

// GPIO
#define MOTOR_GPIO_IN1							LPC_GPIO1
#define MOTOR_GPIO_IN2							LPC_GPIO1
#define MOTOR_GPIO_IN3							LPC_GPIO1
#define MOTOR_GPIO_IN4							LPC_GPIO0

#define MOTOR_GPIO_IN1_PIN					24
#define MOTOR_GPIO_IN2_PIN					23
#define MOTOR_GPIO_IN3_PIN					20
#define MOTOR_GPIO_IN4_PIN					21

// PWM
#define MOTOR_PWM										LPC_PWM0

#define MOTOR_PWM_ENA_CHANNEL				1
#define MOTOR_PWM_ENB_CHANNEL				2

#define MOTOR_PWM_ENA_MR						MR1
#define MOTOR_PWM_ENB_MR						MR2

#define MOTOR_PWM_ENA_PWMENA				9
#define MOTOR_PWM_ENB_PWMENA				10

void motor_init(void);
void motor1_update_duty_cycle(uint8_t duty_cycle);
void motor2_update_duty_cycle(uint8_t duty_cycle);
void motor_both_update_duty_cycle(uint8_t duty_cycle);
void motor1_forward(void);
void motor2_forward(void);
void motor_both_forward(void);
void motor1_backward(void);
void motor2_backward(void);
void motor_both_backward(void);

#endif
