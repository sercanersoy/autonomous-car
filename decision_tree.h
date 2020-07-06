#ifndef __DECISION_TREE_H
#define __DECISION_TREE_H

#include "arm_math.h"
#include "dt_model.h"

typedef struct Node {
	uint8_t isLeaf;
	int8_t value;
	struct Node * left;
	struct Node * right;
	int8_t feature;
	float32_t threshold;
} DT_Node;

extern uint8_t dt_result;

void dt_predict(uint32_t * input);

#endif
