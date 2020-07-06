#ifndef __MLP_H
#define __MLP_H

#include "test_data.h"
#include "arm_math.h"
#include "arm_nnfunctions.h"
#include "ml_util.h"
#include "mlp_model.h"

#define MLP_INPUT_DIM 					4
#define MLP_HIDDEN_DIM 					40
#define MLP_OUTPUT_DIM 					4
#define MLP_INPUT_SIZE					1637

extern uint8_t mlp_result;

void mlp_predict(uint32_t * input);

#endif
