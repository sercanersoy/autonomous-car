#include "mlp.h"

static q15_t hidden_weights[MLP_HIDDEN_DIM * MLP_INPUT_DIM] = {HIDDEN_WEIGHT_X2};
static q15_t output_weights[MLP_OUTPUT_DIM * MLP_HIDDEN_DIM] = {OUTPUT_WEIGHT_X2};

static q15_t hidden_bias[MLP_HIDDEN_DIM] = {HIDDEN_BIAS};
static q15_t output_bias[MLP_OUTPUT_DIM] = {OUTPUT_BIAS};

q15_t hidden[MLP_HIDDEN_DIM];
q15_t output[MLP_OUTPUT_DIM];
q15_t result[MLP_OUTPUT_DIM];
uint8_t mlp_result;

void mlp_predict(uint32_t * input) {
	
	q15_t q_input[MLP_INPUT_DIM];
	for (uint8_t i = 0; i < MLP_INPUT_DIM; i++) q_input[i] = (q15_t) input[i];
	
	// calculate output of hidden layer
	arm_fully_connected_q15(q_input, hidden_weights, MLP_INPUT_DIM, MLP_HIDDEN_DIM, 1, 0, hidden_bias, hidden, NULL);
	// activate hidden layer with logistic sigmoid
	arm_nn_activations_direct_q15(hidden, MLP_HIDDEN_DIM, 4, ARM_SIGMOID);
	// calculate output of output layer
	arm_fully_connected_q15(hidden, output_weights, MLP_HIDDEN_DIM, MLP_OUTPUT_DIM, 11, 16, output_bias, output, NULL);
	// activate output layer with softmax
	arm_softmax_q15(output, MLP_OUTPUT_DIM, result);
	// find the resulting class using argmax
	mlp_result = (uint8_t) argmax_vec_q15(result, MLP_OUTPUT_DIM);
}
