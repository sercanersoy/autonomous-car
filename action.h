#ifndef __ACTION_H
#define __ACTION_H

#include "LPC407x_8x_177x_8x.h"
#include "motor_driver.h"

enum Action {
	ACTION_MOVE_FORWARD,
	ACTION_SHARP_RIGHT,
	ACTION_SLIGHT_LEFT,
	ACTION_SLIGHT_RIGHT,
	ACTION_IDLE,
	ACTION_BUSY,
};

extern enum Action action_current;

void action_init(void);
void action_enum_to_action(enum Action action);

#endif
