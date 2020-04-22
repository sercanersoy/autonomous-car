#include "joystick.h"

// IOCON
#define JOYSTICK_LEFT_IOCON							LPC_IOCON->P5_0
#define JOYSTICK_DOWN_IOCON							LPC_IOCON->P5_1
#define JOYSTICK_UP_IOCON								LPC_IOCON->P5_2
#define JOYSTICK_CENTER_IOCON						LPC_IOCON->P5_3
#define JOYSTICK_RIGHT_IOCON						LPC_IOCON->P5_4

// GPIO
#define JOYSTICK_GPIO										LPC_GPIO5
#define JOYSTICK_MASK										0x1F

void joystick_init(void) {
	// change IOCON functionality to GPIO
	JOYSTICK_LEFT_IOCON		 &= ~(7 << 0);
	JOYSTICK_DOWN_IOCON		 &= ~(7 << 0);
	JOYSTICK_UP_IOCON			 &= ~(7 << 0);
	JOYSTICK_CENTER_IOCON	 &= ~(7 << 0);
	JOYSTICK_RIGHT_IOCON	 &= ~(7 << 0);
}

uint8_t joystick_pressed(void) {
	return ~(JOYSTICK_GPIO->PIN | ~(JOYSTICK_MASK));
}
