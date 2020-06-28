#include "action.h"

#define ACTION_FORWARD_MICROSECOND		600000
#define ACTION_SHARP_MICROSECOND			600000
#define ACTION_SLIGHT_MICROSECOND			300000
#define ACTION_FORWARD_SPEED					40
#define ACTION_TURN_SPEED							80

// POWER
#define ACTION_PCONP_TIM_BIT					5

// IOCON
#define ACTION_IOCON_TIM							LPC_IOCON->P1_28
#define ACTION_IOCON_TIM_FUNC					3

// PWM
#define ACTION_TIM										LPC_TIM0
#define ACTION_TIM_MR									LPC_TIM0->MR0
#define ACTION_TIM_MRINT_BIT					0
#define ACTION_TIM_MRI_BIT						0
#define ACTION_TIM_MRR_BIT						1
#define ACTION_TIM_MRS_BIT						2

// Interrupt
#define ACTION_TIM_IRQN								TIMER0_IRQn
#define ACTION_TIMER_IRQHANDLER				TIMER0_IRQHandler

// busy at the beginning
enum Action action_current = ACTION_BUSY;

void action_init(void) {
	// turn on TIM
	LPC_SC->PCONP |= (1 << ACTION_PCONP_TIM_BIT);
	
	// change IOCON functionality
	ACTION_IOCON_TIM |= ACTION_IOCON_TIM_FUNC;
	
	// disable and reset counter
	ACTION_TIM->TCR &= ~(1 << 0);
	ACTION_TIM->TCR |= (1 << 1);
	
	// set PR to make TC count for every 1 microseconds
	ACTION_TIM->PR = 59;
	
	// interrupt, reset and stop on MR0 match
	ACTION_TIM->MCR |= (1 << ACTION_TIM_MRI_BIT)
									 | (1 << ACTION_TIM_MRS_BIT);
	
	// enable interrupts
	NVIC_EnableIRQ(ACTION_TIM_IRQN);
}

void start_timer_and_stop_after(uint32_t microseconds) {
	// set MR to stop after action time
	ACTION_TIM_MR = microseconds;
	// start counting
	ACTION_TIM->TCR |= (1 << 0);
	ACTION_TIM->TCR &= ~(1 << 1);
}

void action_move_forward(void) {
	motor_both_forward();
	action_current = ACTION_MOVE_FORWARD;
	motor_both_update_duty_cycle(ACTION_FORWARD_SPEED);
	start_timer_and_stop_after(ACTION_FORWARD_MICROSECOND);
}

void action_sharp_right_turn(void) {
	motor1_forward();
	motor2_backward();
	motor_both_update_duty_cycle(ACTION_TURN_SPEED);
	action_current = ACTION_SHARP_RIGHT;
	start_timer_and_stop_after(ACTION_SHARP_MICROSECOND);
}

void action_slight_left_turn(void) {
	motor1_backward();
	motor2_forward();
	motor_both_update_duty_cycle(ACTION_TURN_SPEED);
	action_current = ACTION_SLIGHT_LEFT;
	start_timer_and_stop_after(ACTION_SLIGHT_MICROSECOND);
}

void action_slight_right_turn(void) {
	motor1_forward();
	motor2_backward();
	motor_both_update_duty_cycle(ACTION_TURN_SPEED);
	action_current = ACTION_SLIGHT_RIGHT;
	start_timer_and_stop_after(ACTION_SLIGHT_MICROSECOND);
}

void action_enum_to_action(enum Action action) {
	switch (action) {
		case ACTION_MOVE_FORWARD:
			action_move_forward();
			break;
		
		case ACTION_SHARP_RIGHT:
			action_sharp_right_turn();
			break;
		
		case ACTION_SLIGHT_LEFT:
			action_slight_left_turn();
			break;
		
		case ACTION_SLIGHT_RIGHT:
			action_slight_right_turn();
			break;
		
		default:
			break;
	}
}

void ACTION_TIMER_IRQHANDLER(void) {
	// match occurred
	if ((ACTION_TIM->IR & (1 << ACTION_TIM_MRINT_BIT)) != 0) {
		// clear interrupt
		ACTION_TIM->IR |= (1 << ACTION_TIM_MRINT_BIT);
		ACTION_TIM->TC = 0;
		
		if (action_current == ACTION_MOVE_FORWARD) {
			// stop the motors
			motor_both_update_duty_cycle(0);
			// update action as idle
			action_current = ACTION_IDLE;
		} else if (action_current == ACTION_SHARP_RIGHT ||
							 action_current == ACTION_SLIGHT_LEFT ||
							 action_current == ACTION_SLIGHT_RIGHT) {
			// current action is a turn
			// next action will be move forward
			action_move_forward();
		}
	}
}
