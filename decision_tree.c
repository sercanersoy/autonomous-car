#include "decision_tree.h"

DT_Node dt_nodes[N_NODES] = {DT_MODEL(dt_nodes)};

int8_t dt_result;

void dt_predict(int16_t * input) {
	
	DT_Node * curr = &dt_nodes[0];
	
	while (!(curr->isLeaf)) {
		
		if (input[curr->feature] > curr->threshold) {
			curr = curr->right;
		} else {
			curr = curr->left;
		}
	}
	
	dt_result = curr->value;
}
