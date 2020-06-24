#ifndef __LED_H
#define __LED_H

#include "LPC407x_8x_177x_8x.h"

#define LED_COUNT 			4

void led_init(void);
void led_turn_on(uint8_t led_no);
void led_turn_off(uint8_t led_no);
void led_turn_off_all(void);
void led_turn_on_solo(uint8_t led_no);

#endif
