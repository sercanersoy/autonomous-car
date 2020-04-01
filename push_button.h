#ifndef __PUSH_BUTTON_H
#define __PUSH_BUTTON_H

#include "LPC407x_8x_177x_8x.h"

// IOCON
#define PUSH_BUTTON_IOCON								LPC_IOCON->P2_10
	// FUNC		
	#define PUSH_BUTTON_IOCON_FUNC				1
		
// EINT		
#define PUSH_BUTTON_EINT_BIT						0
		
// Interrupt		
#define PUSH_BUTTON_EINT_IRQN						EINT0_IRQn
#define PUSH_BUTTON_EINT_IRQHANDLER			EINT0_IRQHandler

extern uint8_t push_button_pressed;

void push_button_init(void);
void PUSH_BUTTON_EINT_IRQHANDLER(void);

#endif
