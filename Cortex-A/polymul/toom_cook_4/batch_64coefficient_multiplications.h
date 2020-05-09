#ifndef BATCH_64COEFFICIENT_MULTIPLICATIONS_H
#define BATCH_64COEFFICIENT_MULTIPLICATIONS_H

#include <arm_neon.h>
#include "../../SABER_indcpa.h"
#include "../../SABER_params.h"
#include "asimd_scm_neon.h"

void batch_64coefficient_multiplications(
    uint16_t restrict *a0, uint16_t restrict *b0, uint16_t restrict *result_final0, 
    uint16_t restrict *a1, uint16_t restrict *b1, uint16_t restrict *result_final1, 
    uint16_t restrict *a2, uint16_t restrict *b2, uint16_t restrict *result_final2, 
    uint16_t restrict *a3, uint16_t restrict *b3, uint16_t restrict *result_final3,
    uint16_t restrict *a4, uint16_t restrict *b4, uint16_t restrict *result_final4, 
    uint16_t restrict *a5, uint16_t restrict *b5, uint16_t restrict *result_final5, 
    uint16_t restrict *a6, uint16_t restrict *b6, uint16_t restrict *result_final6);
    
#endif