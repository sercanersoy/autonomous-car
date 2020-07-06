#include "decision_tree.h"

DT_Node dt_nodes[N_NODES] = {DT_MODEL(dt_nodes)};

uint8_t dt_result;

void dt_predict(uint32_t * input) {
	
	DT_Node * curr = &dt_nodes[0];
	
	while (!(curr->isLeaf)) {
		
		if (input[curr->feature] > curr->threshold) {
			curr = curr->right;
		} else {
			curr = curr->left;
		}
	}
	
	dt_result = (uint8_t) curr->value;
}
