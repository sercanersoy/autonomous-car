#include "LPC407x_8x_177x_8x.h"
#include "ultrasonic_sensor.h"
#include "LED.h"

void init(void) {
	ultrasonic_init();
	LED_init();
}

void update(void) {
	if (distance_updated) {
		if (current_distance < 10) {
			turn_off_LED(1);
			turn_off_LED(2);
			turn_off_LED(3);
			turn_off_LED(4);
		} else if (current_distance < 20) {
			turn_on_LED(1);
			turn_off_LED(2);
			turn_off_LED(3);
			turn_off_LED(4);
		} else if (current_distance < 30) {
			turn_on_LED(1);
			turn_on_LED(2);
			turn_off_LED(3);
			turn_off_LED(4);
		} else if (current_distance < 40) {
			turn_on_LED(1);
			turn_on_LED(2);
			turn_on_LED(3);
			turn_off_LED(4);
		} else {
			turn_on_LED(1);
			turn_on_LED(2);
			turn_on_LED(3);
			turn_on_LED(4);
		}
	}
}

int main(void) {
	init();
	
	while(1) {
		update();
	}
}
