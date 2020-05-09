#ifndef ASIMD_TOOM_COOK_4WAY_NEON_H
#define ASIMD_TOOM_COOK_4WAY_NEON_H

/*
 * toom_cook_4way_neon
 * Main function 
 * a1_avx: In memory [256]
 * b1_avx: In memory  [256]
 * res_avx_output: In memory [512]
 */
void toom_cook_4way_neon(uint16_t *a1_avx,
						uint16_t *b1_avx,
						uint16_t p_mod,
						uint16_t *res_avx_output);


#endif