#ifndef BATCH_64COEFFICIENT_MULTIPLICATIONS_H
#define BATCH_64COEFFICIENT_MULTIPLICATIONS_H

#include <arm_neon.h>
#include "../../SABER_indcpa.h"
#include "../../SABER_params.h"
#include "asimd_scm_neon.h"

void batch_64coefficient_multiplications(
    uint16_t *a0, uint16_t *b0, uint16_t *result_final0, 
    uint16_t *a1, uint16_t *b1, uint16_t *result_final1, 
    uint16_t *a2, uint16_t *b2, uint16_t *result_final2, 
    uint16_t *a3, uint16_t *b3, uint16_t *result_final3,
    uint16_t *a4, uint16_t *b4, uint16_t *result_final4, 
    uint16_t *a5, uint16_t *b5, uint16_t *result_final5, 
    uint16_t *a6, uint16_t *b6, uint16_t *result_final6);

#endif