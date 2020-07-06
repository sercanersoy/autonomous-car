#include "ultrasonic_sensor.h"
#include "LED.h"
#include "motor_driver.h"
#include "push_button.h"
#include "mlp.h"
#include "action.h"
#include "joystick.h"
#include "svm.h"
#include "decision_tree.h"

// OPERATION MODE
// options: TEST, MLP, SVM, DT
#define TEST

void init(void) {
	ultrasonic_init();
	led_init();
	motor_init();
	push_button_init();
	action_init();
	// besides others, SVM models need to be initialized
	#ifdef SVM
	svm_models_init();
	#endif
}

void handle_ultrasonic_updated_led(uint8_t sensor_no) {
	ultrasonic_updated &= ~(1 << sensor_no);
	
	if (ultrasonic_distance[sensor_no] < 15)
		led_turn_off(sensor_no);
	else 
		led_turn_on(sensor_no);
}

#ifndef TEST

void handle_action_idle() {
	
	enum Action action_result;
	
	#if defined(MLP)
	
	mlp_predict(ultrasonic_distance);
	action_result = (enum Action) mlp_result;
	
	#elif defined(SVM)

	svm_predict(ultrasonic_distance);
	action_result = (enum Action) svm_result;
	
	#elif defined(DT)
	
	dt_predict(ultrasonic_distance);
	action_result = (enum Action) dt_result;
	
	#endif
	
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

#else

uint16_t i = 0;
void handle_push_button_pressed(void) {
	push_button_pressed = 0;
	action_enum_to_action((enum Action) (i++ % 4));
}

#endif

// joystick pins collide with sonic sensor pins, so dont use it
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
	
	#ifndef TEST
	if (action_current == ACTION_IDLE) {
		handle_action_idle();
	}
	#endif
	
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
	
	/*
	static int16_t test_input[INPUT_SIZE * VEC_DIM] = {INPUT_DATA};
	static uint8_t correct_output[INPUT_SIZE] = {CORRECT_LABEL};
	
	int16_t * pInput = test_input;
	uint8_t * pCorrect = correct_output;
	
	uint16_t n_true = 0;
	uint16_t true_count[VEC_DIM];
	uint16_t pred_count[VEC_DIM];
	uint16_t count[VEC_DIM];
	
	memset(true_count, 0, VEC_DIM * sizeof(uint16_t));
	memset(pred_count, 0, VEC_DIM * sizeof(uint16_t));
	memset(count, 0, VEC_DIM * sizeof(uint16_t));
	
	for (int i = 0; i < INPUT_SIZE; i++) {
		mlp_predict(pInput);
		pInput += VEC_DIM;
		
		if (mlp_result == *pCorrect) {
			n_true++;
			true_count[result]++;
		}
		
		pred_count[result]++;
		count[*pCorrect++]++;
	}
	
	// breakpoint here to see the result in debug mode
	float32_t accuracy = (float32_t) n_true / INPUT_SIZE;
	*/
	
	while(1) {
		update();
	}
}
