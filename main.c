#include "LPC407x_8x_177x_8x.h"
#include "ultrasonic_sensor.h"
#include "LED.h"
#include "motor_driver.h"
#include "push_button.h"
#include "mlp.h"

void init(void) {
	ultrasonic_init();
	led_init();
	motor_init();
	push_button_init();
}

void handle_ultrasonic_updated(uint8_t sensor_no) {
	ultrasonic_updated[sensor_no] = 0;
	if (ultrasonic_distance[sensor_no] < 15) {
		led_turn_off(sensor_no);
	} else {
		led_turn_on(sensor_no);
	}
}

void handle_push_button_pressed(void) {
	// TODO
}

void update(void) {
	int i;
	for (i = 0; i < n_sensors; i++) {
		if (ultrasonic_updated[i]) {
			handle_ultrasonic_updated(i);
		}
	}
	
	if (push_button_pressed) {
		handle_push_button_pressed();
	}
}

int main(void) {
	init();
	mlp();
	
	while(1) {
		update();
	}
}
