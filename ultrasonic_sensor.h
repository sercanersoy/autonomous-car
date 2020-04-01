#ifndef __ULTRASONIC_SENSOR_H
#define __ULTRASONIC_SENSOR_H

#include "LPC407x_8x_177x_8x.h"
#include "LED.h"

#define RECOMMENDED_WAITING_MICROSECOND		60000

typedef struct {
	const						uint8_t						id;
	const						uint8_t						PCONP_bit;
				volatile	uint32_t*					IOCON_echo;
				volatile	uint32_t*					IOCON_trig;
	const						uint8_t						IOCON_echo_func;
	const						uint8_t						IOCON_trig_func;
									LPC_TIM_TypeDef*	TIM;
				volatile	uint32_t*					TIM_MR;
	const	volatile	uint32_t*					TIM_CR;
	const						uint8_t						TIM_MRI_bit;
	const						uint8_t						TIM_MRR_bit;
	const						uint8_t						TIM_CAPRE_bit;
	const						uint8_t						TIM_CAPFE_bit;
	const						uint8_t						TIM_CAPI_bit;
	const						uint8_t						TIM_EM_bit;
	const						uint8_t						TIM_EMC_bit;
	const						uint8_t						TIM_MRINT_bit;
	const						uint8_t						TIM_CRINT_bit;
	const	enum			IRQn							TIM_IRQn;
} Ultrasonic_Sensor_TypeDef;

extern const uint8_t n_sensors;
extern uint32_t ultrasonic_distance[];
extern uint8_t ultrasonic_updated[];

void ultrasonic_init(void);

#endif
