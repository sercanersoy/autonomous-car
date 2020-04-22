#include "ultrasonic_sensor.h"
#include "LED.h"
#include "motor_driver.h"
#include "push_button.h"
#include "mlp.h"
#include "action.h"
#include "joystick.h"

void init(void) {
	ultrasonic_init();
	led_init();
	motor_init();
	push_button_init();
	action_init();
}

void handle_ultrasonic_updated_led(uint8_t sensor_no) {
	ultrasonic_updated &= ~(1 << sensor_no);
	
	if (ultrasonic_distance[sensor_no] < 15)
		led_turn_off(sensor_no);
	else 
		led_turn_on(sensor_no);
}

void handle_action_idle() {
	q15_t input[MLP_INPUT_DIM];
	
	for (uint8_t i = 0; i < MLP_INPUT_DIM; i++)
		input[i] = __SSAT(ultrasonic_distance[i], 16);
	
	mlp_network_forward(input);
	enum Action action_result = (enum Action) argmax_vec_q15(mlp_result, MLP_INPUT_DIM);
	
	action_enum_to_action(action_result);
}

void handle_push_button_pressed(void) {
	push_button_pressed = 0;
	if (action_current == ACTION_BUSY) {
		action_current = ACTION_IDLE;
	} else {
		motor_both_update_duty_cycle(0);
		action_current = ACTION_BUSY;
	}
}

int i = 0;
void handle_push_button_pressed_test(void) {
	push_button_pressed = 0;
	action_enum_to_action((enum Action) (i++ % 4));
}

/*
void handle_joystick_pressed(void) {
	switch (joystick_pressed()) {
		case JOYSTICK_UP:
			action_enum_to_action(ACTION_MOVE_FORWARD);
			led_turn_on_solo(0);
			break;
		
		case JOYSTICK_RIGHT:
			action_enum_to_action(ACTION_SLIGHT_RIGHT);
			led_turn_on_solo(1);
			break;
		
		case JOYSTICK_DOWN:
			action_enum_to_action(ACTION_SHARP_RIGHT);
			led_turn_on_solo(2);
			break;
		
		case JOYSTICK_LEFT:
			action_enum_to_action(ACTION_SLIGHT_LEFT);
			led_turn_on_solo(3);
			break;
	}
}
*/

void update(void) {
	for (int i = 0; i < n_sensors; i++) {
		if ((ultrasonic_updated & (1 << i)) != 0) {
			handle_ultrasonic_updated_led(i);
		}
	}
	
	if (action_current == ACTION_IDLE) {
		handle_action_idle();
	}
	
	if (push_button_pressed) {
		handle_push_button_pressed();
	}
	
	/*
	if (joystick_pressed()) {
		handle_joystick_pressed();
	}
	*/
}

int main(void) {
	init();
	
	while(1) {
		update();
	}
}
