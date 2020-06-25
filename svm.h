#ifndef __SVM_H
#define __SVM_H

#include "svm_model.h"
#include "arm_math.h"
#include "ml_util.h"

#define VEC_DIM		4
#define N_CLASSES	4
#define N_MODELS	6

extern int32_t svm_result;

void svm_models_init(void);

void svm_predict(float32_t * input);

#endif
