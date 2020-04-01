#include "motor_driver.h"

void motor_init(void) {
	// turn on motor driver PWM
	LPC_SC->PCONP |= (1 << MOTOR_PCONP_PWM_BIT);
	
	// change IOCON FUNC bits of the pins
	MOTOR_IOCON_ENA |= MOTOR_IOCON_ENA_FUNC;
	MOTOR_IOCON_ENB |= MOTOR_IOCON_ENB_FUNC;
	
	// change DIR to output for GPIO pins
	MOTOR_GPIO_IN1->DIR |= (1 << MOTOR_GPIO_IN1_PIN);
	MOTOR_GPIO_IN2->DIR |= (1 << MOTOR_GPIO_IN2_PIN);
	MOTOR_GPIO_IN3->DIR |= (1 << MOTOR_GPIO_IN3_PIN);
	MOTOR_GPIO_IN4->DIR |= (1 << MOTOR_GPIO_IN4_PIN);
	
	// both motors forward initially
	motor_both_forward();
	
	// disable and reset counter
	MOTOR_PWM->TCR &= ~(1 << 0);
	MOTOR_PWM->TCR |= (1 << 1);
	
	// set PR to 1 microsecond increment on TC
	MOTOR_PWM->PR = 59;
	
	// reset TC on MR0 match
	MOTOR_PWM->MCR |= (1 << 1);
	
	// set MR0 to obtain 20 ms period
	MOTOR_PWM->MR0 = 20000;
	
	// duty cycle is 0 initially
	MOTOR_PWM->MOTOR_PWM_ENA_MR = 1;
	MOTOR_PWM->MOTOR_PWM_ENB_MR = 1;
	
	// enable PWM mode
	MOTOR_PWM->TCR |= (1 << 3);
	
	// enable PWM output for ENA and ENB
	MOTOR_PWM->PCR |= (1 << MOTOR_PWM_ENA_PWMENA)
									| (1 << MOTOR_PWM_ENB_PWMENA);
	
	// enable counter and clear reset
	MOTOR_PWM->TCR |= (1 << 0);
	MOTOR_PWM->TCR &= ~(1 << 1);
}

void motor1_update_duty_cycle(uint8_t duty_cycle) {
	if (duty_cycle > 100) {
		duty_cycle = 100;
	}
	
	MOTOR_PWM->MOTOR_PWM_ENA_MR = MOTOR_PWM->MR0 * duty_cycle / 100;
	MOTOR_PWM->LER |= (1 << MOTOR_PWM_ENA_CHANNEL);
}

void motor2_update_duty_cycle(uint8_t duty_cycle) {
	if (duty_cycle > 100) {
		duty_cycle = 100;
	}
	
	MOTOR_PWM->MOTOR_PWM_ENB_MR = MOTOR_PWM->MR0 * duty_cycle / 100;
	MOTOR_PWM->LER |= (1 << MOTOR_PWM_ENB_CHANNEL);
}

void motor_both_update_duty_cycle(uint8_t duty_cycle) {
	motor1_update_duty_cycle(duty_cycle);
	motor2_update_duty_cycle(duty_cycle);
}

void motor1_forward(void) {
	MOTOR_GPIO_IN1->CLR |= (1 << MOTOR_GPIO_IN1_PIN);
	MOTOR_GPIO_IN2->SET |= (1 << MOTOR_GPIO_IN2_PIN);
}

void motor2_forward(void) {
	MOTOR_GPIO_IN3->CLR |= (1 << MOTOR_GPIO_IN3_PIN);
	MOTOR_GPIO_IN4->SET |= (1 << MOTOR_GPIO_IN4_PIN);
}

void motor_both_forward(void) {
	motor1_forward();
	motor2_forward();
}

void motor1_backward(void) {
	MOTOR_GPIO_IN1->SET |= (1 << MOTOR_GPIO_IN1_PIN);
	MOTOR_GPIO_IN2->CLR |= (1 << MOTOR_GPIO_IN2_PIN);
}

void motor2_backward(void) {
	MOTOR_GPIO_IN3->SET |= (1 << MOTOR_GPIO_IN3_PIN);
	MOTOR_GPIO_IN4->CLR |= (1 << MOTOR_GPIO_IN4_PIN);
}

void motor_both_backward(void) {
	motor1_backward();
	motor2_backward();
}
