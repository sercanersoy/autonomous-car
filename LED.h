#ifndef LED_H
#define LED_H

#include "LPC407x_8x_177x_8x.h"

#define LED1_GPIO								LPC_GPIO1
#define LED2_GPIO								LPC_GPIO0
#define LED3_GPIO								LPC_GPIO1
#define	LED4_GPIO								LPC_GPIO2

#define LED1_GPIO_PIN_BIT				18
#define LED2_GPIO_PIN_BIT				13
#define LED3_GPIO_PIN_BIT				13
#define LED4_GPIO_PIN_BIT				19

void LED_init(void);
void turn_on_LED(uint8_t led_no);
void turn_off_LED(uint8_t led_no);

#endif
