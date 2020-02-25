#include "ultrasonic_sensor.h"

uint8_t is_triggering = 1;
uint8_t is_rising_edge = 1;
uint32_t last_measurement = 0;

uint32_t current_distance = 0;
uint8_t distance_updated = 0;

void ultrasonic_init(void) {
	// turn on timer
	LPC_SC->PCONP |= (1 << ULTRASONIC_TIMER_PCONP_BIT);
	
	// change IOCON functionalities
	ULTRASONIC_ECHO_IOCON |= ULTRASONIC_ECHO_IOCON_FUNC;
	ULTRASONIC_TRIGGER_IOCON |= ULTRASONIC_TRIGGER_IOCON_FUNC;
	
	// disable and reset counter
	ULTRASONIC_TIMER->TCR &= ~(1);
	ULTRASONIC_TIMER->TCR |= (1 << 1);
	
	// set PR to make TC count for every 1 microseconds
	ULTRASONIC_TIMER->PR = 59;
	
	// interrupt on MR3 match
	ULTRASONIC_TIMER->MCR |= (1 << ULTRASONIC_TIMER_MRI_BIT);
	
	// interrupt after 10 microseconds
	ULTRASONIC_TIMER->ULTRASONIC_TIMER_MR = 10;
	
	// interrupt on capture rising edge
	ULTRASONIC_TIMER->CCR |= (1 << ULTRASONIC_TIMER_CAPRE_BIT)
												 | (1 << ULTRASONIC_TIMER_CAPI_BIT);
	
	// clear external match pin when match occurs
	ULTRASONIC_TIMER->EMR |= (1 << ULTRASONIC_TIMER_EMC_BIT);
	
	// enable interrupts
	NVIC_EnableIRQ(ULTRASONIC_TIMER_IRQN);
	
	// set external match high
	ULTRASONIC_TIMER->EMR |= (1 << ULTRASONIC_TIMER_EM_BIT);
	
	// start counting
	ULTRASONIC_TIMER->TCR |= 1;
	ULTRASONIC_TIMER->TCR &= ~(1 << 1);
}

void ULTRASONIC_TIMER_IRQHANDLER(void) {
	int i = 0;
	// check which interrupt occurred
	if ((ULTRASONIC_TIMER->IR & (1 << ULTRASONIC_TIMER_MRINT_BIT)) != 0) {
		// match occurred
		// clear interrupt
		ULTRASONIC_TIMER->IR = (1 << ULTRASONIC_TIMER_MRINT_BIT);
		
		// decide if triggering or time-out
		if (is_triggering) {
			// end of trigger
			is_triggering = 0;
			
			// reset timer when match occurs
			ULTRASONIC_TIMER->MCR |= (1 << ULTRASONIC_TIMER_MRR_BIT);
			
			// trigger again after 60 ms (recommended)
			ULTRASONIC_TIMER->ULTRASONIC_TIMER_MR = ULTRASONIC_TIMER->TC + 60000;
		} else {
			// time-out, trigger again
			is_triggering = 1;
			
			// do not reset timer when match occurs
			ULTRASONIC_TIMER->MCR &= ~(1 << ULTRASONIC_TIMER_MRR_BIT);
			
			// set external match pin to start triggering
			ULTRASONIC_TIMER->EMR |= (1 << ULTRASONIC_TIMER_EM_BIT);
			
			// end triggering after 10 microseconds
			ULTRASONIC_TIMER->ULTRASONIC_TIMER_MR = ULTRASONIC_TIMER->TC + 10;
		}
		
	} else if ((ULTRASONIC_TIMER->IR & (1 << ULTRASONIC_TIMER_CRINT_BIT)) != 0) {
		// capture occurred
		// clear interrupt
		ULTRASONIC_TIMER->IR = (1 << ULTRASONIC_TIMER_CRINT_BIT);
		
		if (is_rising_edge) {
			// rising edge
			is_rising_edge = 0;
			// interrupt on falling edge
			ULTRASONIC_TIMER->CCR |= (1 << ULTRASONIC_TIMER_CAPFE_BIT);
			ULTRASONIC_TIMER->CCR &= ~(1 << ULTRASONIC_TIMER_CAPRE_BIT);
			
			last_measurement = ULTRASONIC_TIMER->ULTRASONIC_TIMER_CR;
		} else {
			// falling edge
			is_rising_edge = 1;
			// interrupt on rising edge
			ULTRASONIC_TIMER->CCR &= ~(1 << ULTRASONIC_TIMER_CAPFE_BIT);
			ULTRASONIC_TIMER->CCR |= (1 << ULTRASONIC_TIMER_CAPRE_BIT);
			
			current_distance = (ULTRASONIC_TIMER->ULTRASONIC_TIMER_CR - last_measurement) / 58;
			distance_updated = 1;
		}
		
	}
	
}
