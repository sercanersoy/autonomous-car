#include "svm.h"

const uint32_t n_sv_models[N_MODELS] = {N_SV_MODELS};

const uint32_t sv_models_i[N_MODELS] = {SV_MODELS_I};

const uint32_t n_sv_classes[N_CLASSES] = {N_SV_CLASSES};

const uint32_t sv_classes_i[N_CLASSES] = {SV_CLASSES_I};

// intercepts for each model
const float32_t intercepts[N_MODELS] = {INTERCEPTS};

// dual coefficients for each model
const float32_t dual_coeffs[(N_CLASSES - 1) * N_SUP_VECS] = {DUAL_COEFFS};

// support vectors for each model
float32_t sup_vecs[(N_CLASSES - 1) * N_SUP_VECS * VEC_DIM];

int32_t classes[N_MODELS * 2];

arm_svm_rbf_instance_f32 models[N_MODELS];

uint8_t svm_result;

void svm_models_init() {
	
	const static float32_t supVecs[N_SUP_VECS * VEC_DIM] = {SUP_VECS};
	
	int n = 0;
	
	for (int i = 0; i < N_CLASSES; i++) {
		
		for (int j = i+1; j < N_CLASSES; j++) {
			
			memcpy(&sup_vecs[sv_models_i[n] * VEC_DIM], &supVecs[sv_classes_i[i] * VEC_DIM], sizeof(float32_t) * n_sv_classes[i] * VEC_DIM);
			memcpy(&sup_vecs[(sv_models_i[n] + n_sv_classes[i]) * VEC_DIM], &supVecs[sv_classes_i[j] * VEC_DIM], sizeof(float32_t) * n_sv_classes[j] * VEC_DIM);
			
			classes[n * 2] = j;
			classes[n * 2 + 1] = i;
			
			arm_svm_rbf_init_f32(
				&models[n],
				n_sv_models[n],
				VEC_DIM,
				intercepts[n],
				&dual_coeffs[sv_models_i[n]],
				&sup_vecs[sv_models_i[n] * VEC_DIM],
				&classes[n * 2],
				GAMMA
			);
			
			n++;
		}
	}
}

void svm_predict(uint32_t * input) {
	
	float32_t f_input[VEC_DIM];
	for (uint8_t i = 0; i < VEC_DIM; i++) f_input[i] = (float32_t) input[i];
	
	uint8_t votes[N_CLASSES];
	memset(votes, 0, N_CLASSES * sizeof(uint8_t));
	
	int n = 0;
	
	for (int i = 0; i < N_CLASSES; i++) {
		
		for (int j = i+1; j < N_CLASSES; j++) {
			
			int32_t result = -1;
			
			arm_svm_rbf_predict_f32(&models[n], f_input, &result);
			votes[result]++;
			
			n++;
		}
	}
	
	svm_result = (uint8_t) argmax_vec_uint8_t(votes, N_CLASSES);
}
