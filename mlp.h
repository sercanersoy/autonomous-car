#ifndef __MLP_H
#define __MLP_H

#define INPUT_DIM 					4
#define HIDDEN_DIM 					40
#define OUTPUT_DIM 					4

#define HIDDEN_W_FRAC_BITS	4
#define HIDDEN_B_FRAC_BITS	2
#define HIDDEN_O_FRAC_BITS	4
#define OUTPUT_W_FRAC_BITS	2
#define OUTPUT_B_FRAC_BITS	1
#define OUTPUT_O_FRAC_BITS	6

void mlp(void);

#endif
