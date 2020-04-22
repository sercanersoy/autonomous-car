#ifndef __JOYSTICK_H
#define __JOYSTICK_H

#include "LPC407x_8x_177x_8x.h"

#define JOYSTICK_LEFT										0x1
#define JOYSTICK_DOWN										0x2
#define JOYSTICK_UP											0x4
#define JOYSTICK_CENTER									0x8
#define JOYSTICK_RIGHT									0x10

void joystick_init(void);
uint8_t joystick_pressed(void);

#endif
