#include "push_button.h"

uint8_t push_button_pressed = 0;

void push_button_init(void) {
	// change IOCON FUNC
	PUSH_BUTTON_IOCON |= PUSH_BUTTON_IOCON_FUNC;
	
	// change EXTMODE to edge-sensitive
	LPC_SC->EXTMODE |= (1 << PUSH_BUTTON_EINT_BIT);
	
	// change EXTPOLAR to falling-edge sensitive
	LPC_SC->EXTPOLAR &= ~(1 << PUSH_BUTTON_EINT_BIT);
	
	// enable interrupts
	NVIC_EnableIRQ(PUSH_BUTTON_EINT_IRQN);
}

void PUSH_BUTTON_EINT_IRQHANDLER(void) {
	// clear interrupt
	LPC_SC->EXTINT |= (1 << PUSH_BUTTON_EINT_BIT);
	
	push_button_pressed = 1;
}
