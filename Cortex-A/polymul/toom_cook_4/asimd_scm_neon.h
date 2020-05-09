#ifndef ASIMD_SCM_NEON
#define ASIMD_SCM_NEON 

#define SCM_SIZE 16

uint16_t c_avx[16 * 2 * SCM_SIZE];
uint16_t a[16 * SCM_SIZE];
uint16_t b[16 * SCM_SIZE];
uint16_t c_avx_extra[16 * 4];

// void schoolbook_neon_new(uint16_t *restrict c, uint16_t *restrict a_mem, uint16_t *restrict b_mem);
void schoolbook_neon_new(uint16_t *c, uint16_t *a_mem, uint16_t *b_mem);

#endif