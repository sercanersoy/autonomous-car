#include "mlp.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "test_data.h"
#include "arm_math.h"
#include "arm_nnfunctions.h"

static q7_t hidden_weights[HIDDEN_DIM * INPUT_DIM] = HIDDEN_WEIGHT;
static q7_t output_weights[OUTPUT_DIM * HIDDEN_DIM] = OUTPUT_WEIGHT;

static q7_t hidden_bias[HIDDEN_DIM] = HIDDEN_BIAS;
static q7_t output_bias[OUTPUT_DIM] = OUTPUT_BIAS;

static q15_t test_input1[INPUT_DIM] = INPUT_DATA1;
static q15_t test_input2[INPUT_DIM] = INPUT_DATA2;

q15_t hidden[HIDDEN_DIM];
q15_t output[OUTPUT_DIM];
q15_t result[OUTPUT_DIM];

void mlp_apply(q15_t * input, q15_t * hidden, q15_t * output, q15_t * result, uint16_t input_size, uint16_t hidden_size,
	uint16_t output_size, q7_t * hidden_weights, q7_t * output_weights, q7_t * hidden_bias, q7_t * output_bias) {

	uint8_t hidden_bias_shift 	= 0 								 + HIDDEN_W_FRAC_BITS - HIDDEN_B_FRAC_BITS;
	uint8_t hidden_output_shift = 0 								 + HIDDEN_W_FRAC_BITS - HIDDEN_O_FRAC_BITS;
	uint8_t output_bias_shift 	= HIDDEN_O_FRAC_BITS + OUTPUT_W_FRAC_BITS - OUTPUT_B_FRAC_BITS;
	uint8_t output_output_shift = HIDDEN_O_FRAC_BITS + OUTPUT_W_FRAC_BITS - OUTPUT_O_FRAC_BITS;
	
	// calculate hidden layer
	arm_fully_connected_mat_q7_vec_q15_opt(input, hidden_weights, input_size, hidden_size, hidden_bias_shift, hidden_output_shift, hidden_bias, hidden, NULL);
	// sigmoid activation for hidden layer
	arm_nn_activations_direct_q15(hidden, hidden_size, 3, ARM_SIGMOID);
	// calculate output layer
	arm_fully_connected_mat_q7_vec_q15_opt(hidden, output_weights, hidden_size, output_size, output_bias_shift, output_output_shift, output_bias, output, NULL);
	// softmax activation for output layer
	arm_softmax_q15(output, output_size, result);
}

void mlp(void) {
	mlp_apply(test_input1, hidden, output, result, INPUT_DIM, HIDDEN_DIM, OUTPUT_DIM, hidden_weights, output_weights, hidden_bias, output_bias);
	
	mlp_apply(test_input2, hidden, output, result, INPUT_DIM, HIDDEN_DIM, OUTPUT_DIM, hidden_weights, output_weights, hidden_bias, output_bias);
}
