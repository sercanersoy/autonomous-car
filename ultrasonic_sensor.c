#include "ultrasonic_sensor.h"

Ultrasonic_Sensor_TypeDef ultrasonic0 = {
	0,
	23,
	&LPC_IOCON->P0_23,
	&LPC_IOCON->P5_2,
	3,
	3,
	LPC_TIM3,
	&LPC_TIM3->MR2,
	&LPC_TIM3->CR0,
	6,
	7,
	0,
	1,
	2,
	2,
	8,
	2,
	4,
	TIMER3_IRQn
};

Ultrasonic_Sensor_TypeDef ultrasonic1 = {
	1,
	22,
	&LPC_IOCON->P0_4,
	&LPC_IOCON->P0_9,
	3,
	3,
	LPC_TIM2,
	&LPC_TIM2->MR3,
	&LPC_TIM2->CR0,
	9,
	10,
	0,
	1,
	2,
	3,
	10,
	3,
	4,
	TIMER2_IRQn
};

Ultrasonic_Sensor_TypeDef ultrasonic2 = {
	2,
	22,
	&LPC_IOCON->P0_5,
	&LPC_IOCON->P0_9,
	3,
	3,
	LPC_TIM2,
	&LPC_TIM2->MR3,
	&LPC_TIM2->CR1,
	9,
	10,
	3,
	4,
	5,
	3,
	10,
	3,
	5,
	TIMER2_IRQn
};

Ultrasonic_Sensor_TypeDef ultrasonic3 = {
	3,
	23,
	&LPC_IOCON->P0_24,
	&LPC_IOCON->P5_2,
	3,
	3,
	LPC_TIM3,
	&LPC_TIM3->MR2,
	&LPC_TIM3->CR1,
	6,
	7,
	3,
	4,
	5,
	2,
	8,
	2,
	5,
	TIMER3_IRQn
};

Ultrasonic_Sensor_TypeDef* ultrasonic[] = {&ultrasonic0, &ultrasonic1, &ultrasonic2, &ultrasonic3};
const uint8_t n_sensors = sizeof(ultrasonic) / sizeof(Ultrasonic_Sensor_TypeDef*);

Ultrasonic_Sensor_TypeDef* TIM2_using_sensors[] = {&ultrasonic1, &ultrasonic2};
const uint8_t n_TIM2_using_sensors = sizeof(TIM2_using_sensors) / sizeof(Ultrasonic_Sensor_TypeDef*);

Ultrasonic_Sensor_TypeDef* TIM3_using_sensors[] = {&ultrasonic0, &ultrasonic3};
const uint8_t n_TIM3_using_sensors = sizeof(TIM3_using_sensors) / sizeof(Ultrasonic_Sensor_TypeDef*);

uint8_t is_trig_rising = 0xF; // used as a binary array
uint8_t is_echo_rising = 0xF; // used as a binary array
uint32_t last_measurement[] = {0, 0, 0, 0};

uint32_t ultrasonic_distance[] = {0, 0, 0, 0};
uint8_t ultrasonic_updated = 0; // used as a binary array

void ultrasonic_init_single(Ultrasonic_Sensor_TypeDef* sensor) {
	// turn on TIM
	LPC_SC->PCONP |= (1 << sensor->PCONP_bit);
	
	// change IOCON functionalities
	*sensor->IOCON_echo |= sensor->IOCON_echo_func;
	*sensor->IOCON_trig |= sensor->IOCON_trig_func;
	
	// disable and reset counter
	sensor->TIM->TCR &= ~(1);
	sensor->TIM->TCR |= (1 << 1);
	
	// set PR to make TC count for every 1 microseconds
	sensor->TIM->PR = 59;
	
	// interrupt on MR3 match
	sensor->TIM->MCR |= (1 << sensor->TIM_MRI_bit);
	
	// clear external match pin when match occurs
	sensor->TIM->EMR |= (1 << sensor->TIM_EMC_bit);
	
	// interrupt on capture rising edge
	sensor->TIM->CCR |= (1 << sensor->TIM_CAPRE_bit)
										| (1 << sensor->TIM_CAPI_bit);
	
	// enable interrupts
	NVIC_EnableIRQ(sensor->TIM_IRQn);
	
	// start counting
	sensor->TIM->TCR |= 1;
	sensor->TIM->TCR &= ~(1 << 1);
}

void start_triggering(Ultrasonic_Sensor_TypeDef* sensor) {
	// set external match high
	sensor->TIM->EMR |= (1 << sensor->TIM_EM_bit);
	
	// interrupt after 10 microseconds
	*sensor->TIM_MR = sensor->TIM->TC + 10;
}

void ultrasonic_init(void) {
	uint8_t i;
	for (i = 0; i < n_sensors; i++) {
		ultrasonic_init_single(ultrasonic[i]);
	}
	is_trig_rising &= ~(1 << 0);
	start_triggering(ultrasonic[0]);
}

void ultrasonic_handle_single(Ultrasonic_Sensor_TypeDef* sensor) {
	// check which interrupt occurred
	if ((sensor->TIM->IR & (1 << sensor->TIM_MRINT_bit)) != 0) {
		// match occurred
		// clear interrupt
		sensor->TIM->IR = (1 << sensor->TIM_MRINT_bit);
		
		if ((is_trig_rising & (1 << sensor->id)) != 0) {
			// set external match pin to start triggering
			sensor->TIM->EMR |= (1 << sensor->TIM_EM_bit);
			
			// end triggering after 10 microseconds
			*sensor->TIM_MR = sensor->TIM->TC + 10;
			
			// next is falling
			is_trig_rising &= ~(1 << sensor->id);
		} else {
			// go on to next sensor
			uint8_t next_sensor = sensor->id + 1;
			if (next_sensor >= n_sensors / 2) {
				next_sensor = 0;
			}
			
			*ultrasonic[next_sensor]->TIM_MR = ultrasonic[next_sensor]->TIM->TC + RECOMMENDED_WAITING_MICROSECOND / (n_sensors / 2);
			
			// next is rising
			is_trig_rising |= (1 << next_sensor);
		}
	} else if ((sensor->TIM->IR & (1 << sensor->TIM_CRINT_bit)) != 0) {
		// capture occurred
		// clear interrupt
		sensor->TIM->IR = (1 << sensor->TIM_CRINT_bit);
		
		if ((is_echo_rising & (1 << sensor->id)) != 0) {
			// interrupt on falling edge
			sensor->TIM->CCR |= (1 << sensor->TIM_CAPFE_bit);
			sensor->TIM->CCR &= ~(1 << sensor->TIM_CAPRE_bit);
			
			last_measurement[sensor->id] = *sensor->TIM_CR;
			
			// next is falling
			is_echo_rising &= ~(1 << sensor->id);
		} else {
			// interrupt on rising edge
			sensor->TIM->CCR &= ~(1 << sensor->TIM_CAPFE_bit);
			sensor->TIM->CCR |= (1 << sensor->TIM_CAPRE_bit);
			
			ultrasonic_distance[sensor->id] = (*sensor->TIM_CR - last_measurement[sensor->id]) / 58;
			ultrasonic_updated |= (1 << sensor->id);
			
			// next is rising
			is_echo_rising |= (1 << sensor->id);
		}
	}
}

void TIMER2_IRQHandler(void) {
	uint8_t i;
	
	for (i = 0; i < n_TIM2_using_sensors; i++) {
		ultrasonic_handle_single(TIM2_using_sensors[i]);
	}
}

void TIMER3_IRQHandler(void) {
	uint8_t i;
	
	for (i = 0; i < n_TIM3_using_sensors; i++) {
		ultrasonic_handle_single(TIM3_using_sensors[i]);
	}
}
