#include "led.h"

#define LED1_GPIO								LPC_GPIO1
#define LED2_GPIO								LPC_GPIO0
#define LED3_GPIO								LPC_GPIO1
#define	LED4_GPIO								LPC_GPIO2

#define LED1_GPIO_PIN_BIT				18
#define LED2_GPIO_PIN_BIT				13
#define LED3_GPIO_PIN_BIT				13
#define LED4_GPIO_PIN_BIT				19

void led_init(void) {
	// make pins output
	LED1_GPIO->DIR |= (1 << LED1_GPIO_PIN_BIT);
	LED2_GPIO->DIR |= (1 << LED2_GPIO_PIN_BIT);
	LED3_GPIO->DIR |= (1 << LED3_GPIO_PIN_BIT);
	LED4_GPIO->DIR |= (1 << LED4_GPIO_PIN_BIT);

	// turn them off initially
	LED1_GPIO->SET |= (1 << LED1_GPIO_PIN_BIT);
	LED2_GPIO->SET |= (1 << LED2_GPIO_PIN_BIT);
	LED3_GPIO->CLR |= (1 << LED3_GPIO_PIN_BIT);
	LED4_GPIO->CLR |= (1 << LED4_GPIO_PIN_BIT);
}

void led_turn_on(uint8_t led_no) {
	switch (led_no) {
		case 0:
			LED1_GPIO->CLR |= (1 << LED1_GPIO_PIN_BIT);
			break;
		case 1:
			LED2_GPIO->CLR |= (1 << LED2_GPIO_PIN_BIT);
			break;
		case 2:
			LED3_GPIO->SET |= (1 << LED3_GPIO_PIN_BIT);
			break;
		case 3:
			LED4_GPIO->SET |= (1 << LED4_GPIO_PIN_BIT);
			break;
	}
}

void led_turn_off(uint8_t led_no) {
	switch (led_no) {
		case 0:
			LED1_GPIO->SET |= (1 << LED1_GPIO_PIN_BIT);
			break;
		case 1:
			LED2_GPIO->SET |= (1 << LED2_GPIO_PIN_BIT);
			break;
		case 2:
			LED3_GPIO->CLR |= (1 << LED3_GPIO_PIN_BIT);
			break;
		case 3:
			LED4_GPIO->CLR |= (1 << LED4_GPIO_PIN_BIT);
			break;
	}
}

void led_turn_off_all(void) {
	for (int i = 0; i < LED_COUNT; i++)
		led_turn_off(i);
}

void led_turn_on_solo(uint8_t led_no) {
	for (int i = 0; i < LED_COUNT; i++)
		if (i == led_no)
			led_turn_on(i);
		else
			led_turn_off(i);
}
