#include "ml_util.h"

uint16_t argmax_vec_q15(q15_t * vec, uint16_t dim) {
	uint16_t max_i = 0;
	q15_t max = *vec++;
	
	for (uint16_t i = 1; i < dim; i ++) {
		q15_t curr = *vec++;
		if (curr > max) {
			max = curr;
			max_i = i;
		}
	}
	
	return max_i;
}

uint16_t argmax_vec_uint8_t(uint8_t * vec, uint16_t dim) {
	uint16_t max_i = 0;
	uint8_t max = *vec++;
	
	for (uint16_t i = 1; i < dim; i ++) {
		uint8_t curr = *vec++;
		if (curr > max) {
			max = curr;
			max_i = i;
		}
	}
	
	return max_i;
}
