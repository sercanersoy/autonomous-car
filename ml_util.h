#ifndef __ML_UTIL_H
#define __ML_UTIL_H

#include "LPC407x_8x_177x_8x.h"
#include "arm_math.h"

uint16_t argmax_vec_q15(q15_t * vec, uint16_t dim);

#endif
