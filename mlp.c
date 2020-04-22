#include "mlp.h"

static q15_t hidden_weights[MLP_HIDDEN_DIM * MLP_INPUT_DIM] = HIDDEN_WEIGHT_X2;
static q15_t output_weights[MLP_OUTPUT_DIM * MLP_HIDDEN_DIM] = OUTPUT_WEIGHT_X2;

static q15_t hidden_bias[MLP_HIDDEN_DIM] = HIDDEN_BIAS;
static q15_t output_bias[MLP_OUTPUT_DIM] = OUTPUT_BIAS;

static q15_t test_input[MLP_INPUT_SIZE * MLP_INPUT_DIM] = INPUT_DATA;
static uint8_t correct_output[MLP_INPUT_SIZE] = CORRECT_DATA;

q15_t hidden[MLP_HIDDEN_DIM];
q15_t output[MLP_OUTPUT_DIM];
q15_t mlp_result[MLP_OUTPUT_DIM];

void mlp_network_forward(q15_t * input) {
	
	// calculate output of hidden layer
	arm_fully_connected_q15(input, hidden_weights, MLP_INPUT_DIM, MLP_HIDDEN_DIM, 2, 0, hidden_bias, hidden, NULL);
	// activate hidden layer with logistic sigmoid
	arm_nn_activations_direct_q15(hidden, MLP_HIDDEN_DIM, 3, ARM_SIGMOID);
	// calculate output of output layer
	arm_fully_connected_q15(hidden, output_weights, MLP_HIDDEN_DIM, MLP_OUTPUT_DIM, 11, 15, output_bias, output, NULL);
	// activate output layer with softmax
	arm_softmax_q15(output, MLP_OUTPUT_DIM, mlp_result);
	
}
