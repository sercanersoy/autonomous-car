#include "LED.h"

void LED_init(void) {
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

void turn_on_LED(uint8_t led_no) {
	switch (led_no) {
		case 1:
			LED1_GPIO->CLR |= (1 << LED1_GPIO_PIN_BIT);
			break;
		case 2:
			LED2_GPIO->CLR |= (1 << LED2_GPIO_PIN_BIT);
			break;
		case 3:
			LED3_GPIO->SET |= (1 << LED3_GPIO_PIN_BIT);
			break;
		case 4:
			LED4_GPIO->SET |= (1 << LED4_GPIO_PIN_BIT);
			break;
	}
}

void turn_off_LED(uint8_t led_no) {
	switch (led_no) {
		case 1:
			LED1_GPIO->SET |= (1 << LED1_GPIO_PIN_BIT);
			break;
		case 2:
			LED2_GPIO->SET |= (1 << LED2_GPIO_PIN_BIT);
			break;
		case 3:
			LED3_GPIO->CLR |= (1 << LED3_GPIO_PIN_BIT);
			break;
		case 4:
			LED4_GPIO->CLR |= (1 << LED4_GPIO_PIN_BIT);
			break;
	}
}
