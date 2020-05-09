#include <arm_neon.h>
#include "asimd_scm_neon.h"

static inline
void vmul(uint16x8_t c, uint16x8_t a, uint16x8_t b)
{
    // c = a * b
    c = vmulq_u16(a, b);
}

static inline
void vmla(uint16x8_t c, uint16x8_t a, uint16x8_t b)
{
    // c += a*b 
    c = vmlaq_u16(c, a, b);
}

static inline
void vload(uint16x8_t c, uint16_t *mem)
{
    // Load mem to c 
    c = vld1q_u16(mem);
}

static inline
void vxor(uint16x8_t c, uint16x8_t a, uint16x8_t b)
{
	// c = a ^ b 
	c = veorq_u16(a, b);
}

/*
 * schoolbook_neon_new
 * Assume a, b are already loaded
 */
void schoolbook_neon_new(uint16_t *restrict c, uint16_t *restrict a_mem, uint16_t *restrict b_mem) {
  uint16x8_t tmp, a[16], b[16];
  unsigned int i, j;
  unsigned int pad = 0;
  for (i = 0; i < 2; i++) {
    for (j = 0; j < 16; j++)
    {
        vload(a[j], &a_mem[j*16 + pad]);
        vload(b[j], &b_mem[j*16 + pad]);
    }

    vmul(tmp, a[0], b[0]);
    vst1q_u16(&c[0 + pad], tmp);
    //----
    vmul(tmp, a[0], b[1]);
    vmla(tmp, a[1], b[0]);
    vst1q_u16(&c[16 + pad], tmp);
    //----
    vmul(tmp, a[0], b[2]);
    vmla(tmp, a[1], b[1]);
    vmla(tmp, a[2], b[0]);
    vst1q_u16(&c[32 + pad], tmp);
    //----
    vmul(tmp, a[0], b[3]);
    vmla(tmp, a[1], b[2]);
    vmla(tmp, a[2], b[1]);
    vmla(tmp, a[3], b[0]);
    vst1q_u16(&c[48 + pad], tmp);
    //----
    vmul(tmp, a[0], b[4]);
    vmla(tmp, a[1], b[3]);
    vmla(tmp, a[2], b[2]);
    vmla(tmp, a[3], b[1]);
    vmla(tmp, a[4], b[0]);
    vst1q_u16(&c[64 + pad], tmp);
    //----
    vmul(tmp, a[0], b[5]);
    vmla(tmp, a[1], b[4]);
    vmla(tmp, a[2], b[3]);
    vmla(tmp, a[3], b[2]);
    vmla(tmp, a[4], b[1]);
    vmla(tmp, a[5], b[0]);
    vst1q_u16(&c[80 + pad], tmp);
    //----
    vmul(tmp, a[0], b[6]);
    vmla(tmp, a[1], b[5]);
    vmla(tmp, a[2], b[4]);
    vmla(tmp, a[3], b[3]);
    vmla(tmp, a[4], b[2]);
    vmla(tmp, a[5], b[1]);
    vmla(tmp, a[6], b[0]);
    vst1q_u16(&c[96 + pad], tmp);
    //----
    vmul(tmp, a[0], b[7]);
    vmla(tmp, a[1], b[6]);
    vmla(tmp, a[2], b[5]);
    vmla(tmp, a[3], b[4]);
    vmla(tmp, a[4], b[3]);
    vmla(tmp, a[5], b[2]);
    vmla(tmp, a[6], b[1]);
    vmla(tmp, a[7], b[0]);
    vst1q_u16(&c[112 + pad], tmp);
    //----
    vmul(tmp, a[0], b[8]);
    vmla(tmp, a[1], b[7]);
    vmla(tmp, a[2], b[6]);
    vmla(tmp, a[3], b[5]);
    vmla(tmp, a[4], b[4]);
    vmla(tmp, a[5], b[3]);
    vmla(tmp, a[6], b[2]);
    vmla(tmp, a[7], b[1]);
    vmla(tmp, a[8], b[0]);
    vst1q_u16(&c[128 + pad], tmp);
    //----
    vmul(tmp, a[0], b[9]);
    vmla(tmp, a[1], b[8]);
    vmla(tmp, a[2], b[7]);
    vmla(tmp, a[3], b[6]);
    vmla(tmp, a[4], b[5]);
    vmla(tmp, a[5], b[4]);
    vmla(tmp, a[6], b[3]);
    vmla(tmp, a[7], b[2]);
    vmla(tmp, a[8], b[1]);
    vmla(tmp, a[9], b[0]);
    vst1q_u16(&c[144 + pad], tmp);
    //----
    vmul(tmp, a[0], b[10]);
    vmla(tmp, a[1], b[9]);
    vmla(tmp, a[2], b[8]);
    vmla(tmp, a[3], b[7]);
    vmla(tmp, a[4], b[6]);
    vmla(tmp, a[5], b[5]);
    vmla(tmp, a[6], b[4]);
    vmla(tmp, a[7], b[3]);
    vmla(tmp, a[8], b[2]);
    vmla(tmp, a[9], b[1]);
    vmla(tmp, a[10], b[0]);
    vst1q_u16(&c[160 + pad], tmp);
    //----
    vmul(tmp, a[0], b[11]);
    vmla(tmp, a[1], b[10]);
    vmla(tmp, a[2], b[9]);
    vmla(tmp, a[3], b[8]);
    vmla(tmp, a[4], b[7]);
    vmla(tmp, a[5], b[6]);
    vmla(tmp, a[6], b[5]);
    vmla(tmp, a[7], b[4]);
    vmla(tmp, a[8], b[3]);
    vmla(tmp, a[9], b[2]);
    vmla(tmp, a[10], b[1]);
    vmla(tmp, a[11], b[0]);
    vst1q_u16(&c[176 + pad], tmp);
    //----
    vmul(tmp, a[0], b[12]);
    vmla(tmp, a[1], b[11]);
    vmla(tmp, a[2], b[10]);
    vmla(tmp, a[3], b[9]);
    vmla(tmp, a[4], b[8]);
    vmla(tmp, a[5], b[7]);
    vmla(tmp, a[6], b[6]);
    vmla(tmp, a[7], b[5]);
    vmla(tmp, a[8], b[4]);
    vmla(tmp, a[9], b[3]);
    vmla(tmp, a[10], b[2]);
    vmla(tmp, a[11], b[1]);
    vmla(tmp, a[12], b[0]);
    vst1q_u16(&c[192 + pad], tmp);
    //----
    vmul(tmp, a[0], b[13]);
    vmla(tmp, a[1], b[12]);
    vmla(tmp, a[2], b[11]);
    vmla(tmp, a[3], b[10]);
    vmla(tmp, a[4], b[9]);
    vmla(tmp, a[5], b[8]);
    vmla(tmp, a[6], b[7]);
    vmla(tmp, a[7], b[6]);
    vmla(tmp, a[8], b[5]);
    vmla(tmp, a[9], b[4]);
    vmla(tmp, a[10], b[3]);
    vmla(tmp, a[11], b[2]);
    vmla(tmp, a[12], b[1]);
    vmla(tmp, a[13], b[0]);
    vst1q_u16(&c[208 + pad], tmp);
    //----
    vmul(tmp, a[0], b[14]);
    vmla(tmp, a[1], b[13]);
    vmla(tmp, a[2], b[12]);
    vmla(tmp, a[3], b[11]);
    vmla(tmp, a[4], b[10]);
    vmla(tmp, a[5], b[9]);
    vmla(tmp, a[6], b[8]);
    vmla(tmp, a[7], b[7]);
    vmla(tmp, a[8], b[6]);
    vmla(tmp, a[9], b[5]);
    vmla(tmp, a[10], b[4]);
    vmla(tmp, a[11], b[3]);
    vmla(tmp, a[12], b[2]);
    vmla(tmp, a[13], b[1]);
    vmla(tmp, a[14], b[0]);
    vst1q_u16(&c[224 + pad], tmp);
    //----
    vmul(tmp, a[0], b[15]);
    vmla(tmp, a[1], b[14]);
    vmla(tmp, a[2], b[13]);
    vmla(tmp, a[3], b[12]);
    vmla(tmp, a[4], b[11]);
    vmla(tmp, a[5], b[10]);
    vmla(tmp, a[6], b[9]);
    vmla(tmp, a[7], b[8]);
    vmla(tmp, a[8], b[7]);
    vmla(tmp, a[9], b[6]);
    vmla(tmp, a[10], b[5]);
    vmla(tmp, a[11], b[4]);
    vmla(tmp, a[12], b[3]);
    vmla(tmp, a[13], b[2]);
    vmla(tmp, a[14], b[1]);
    vmla(tmp, a[15], b[0]);
    vst1q_u16(&c[240 + pad], tmp);
    //----
    //=======
    // ci =  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15} 16
    // ai =  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15} 16
    // bi =  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15} 16
    // ----------------PART 2----------------
    vmul(tmp, a[1], b[15]);
    vmla(tmp, a[2], b[14]);
    vmla(tmp, a[3], b[13]);
    vmla(tmp, a[4], b[12]);
    vmla(tmp, a[5], b[11]);
    vmla(tmp, a[6], b[10]);
    vmla(tmp, a[7], b[9]);
    vmla(tmp, a[8], b[8]);
    vmla(tmp, a[9], b[7]);
    vmla(tmp, a[10], b[6]);
    vmla(tmp, a[11], b[5]);
    vmla(tmp, a[12], b[4]);
    vmla(tmp, a[13], b[3]);
    vmla(tmp, a[14], b[2]);
    vmla(tmp, a[15], b[1]);
    vst1q_u16(&c[256 + pad], tmp);
    //-----
    vmul(tmp, a[2], b[15]);
    vmla(tmp, a[3], b[14]);
    vmla(tmp, a[4], b[13]);
    vmla(tmp, a[5], b[12]);
    vmla(tmp, a[6], b[11]);
    vmla(tmp, a[7], b[10]);
    vmla(tmp, a[8], b[9]);
    vmla(tmp, a[9], b[8]);
    vmla(tmp, a[10], b[7]);
    vmla(tmp, a[11], b[6]);
    vmla(tmp, a[12], b[5]);
    vmla(tmp, a[13], b[4]);
    vmla(tmp, a[14], b[3]);
    vmla(tmp, a[15], b[2]);
    vst1q_u16(&c[272 + pad], tmp);
    //-----
    vmul(tmp, a[3], b[15]);
    vmla(tmp, a[4], b[14]);
    vmla(tmp, a[5], b[13]);
    vmla(tmp, a[6], b[12]);
    vmla(tmp, a[7], b[11]);
    vmla(tmp, a[8], b[10]);
    vmla(tmp, a[9], b[9]);
    vmla(tmp, a[10], b[8]);
    vmla(tmp, a[11], b[7]);
    vmla(tmp, a[12], b[6]);
    vmla(tmp, a[13], b[5]);
    vmla(tmp, a[14], b[4]);
    vmla(tmp, a[15], b[3]);
    vst1q_u16(&c[288 + pad], tmp);
    //-----
    vmul(tmp, a[4], b[15]);
    vmla(tmp, a[5], b[14]);
    vmla(tmp, a[6], b[13]);
    vmla(tmp, a[7], b[12]);
    vmla(tmp, a[8], b[11]);
    vmla(tmp, a[9], b[10]);
    vmla(tmp, a[10], b[9]);
    vmla(tmp, a[11], b[8]);
    vmla(tmp, a[12], b[7]);
    vmla(tmp, a[13], b[6]);
    vmla(tmp, a[14], b[5]);
    vmla(tmp, a[15], b[4]);
    vst1q_u16(&c[304 + pad], tmp);
    //-----
    vmul(tmp, a[5], b[15]);
    vmla(tmp, a[6], b[14]);
    vmla(tmp, a[7], b[13]);
    vmla(tmp, a[8], b[12]);
    vmla(tmp, a[9], b[11]);
    vmla(tmp, a[10], b[10]);
    vmla(tmp, a[11], b[9]);
    vmla(tmp, a[12], b[8]);
    vmla(tmp, a[13], b[7]);
    vmla(tmp, a[14], b[6]);
    vmla(tmp, a[15], b[5]);
    vst1q_u16(&c[320 + pad], tmp);
    //-----
    vmul(tmp, a[6], b[15]);
    vmla(tmp, a[7], b[14]);
    vmla(tmp, a[8], b[13]);
    vmla(tmp, a[9], b[12]);
    vmla(tmp, a[10], b[11]);
    vmla(tmp, a[11], b[10]);
    vmla(tmp, a[12], b[9]);
    vmla(tmp, a[13], b[8]);
    vmla(tmp, a[14], b[7]);
    vmla(tmp, a[15], b[6]);
    vst1q_u16(&c[336 + pad], tmp);
    //-----
    vmul(tmp, a[7], b[15]);
    vmla(tmp, a[8], b[14]);
    vmla(tmp, a[9], b[13]);
    vmla(tmp, a[10], b[12]);
    vmla(tmp, a[11], b[11]);
    vmla(tmp, a[12], b[10]);
    vmla(tmp, a[13], b[9]);
    vmla(tmp, a[14], b[8]);
    vmla(tmp, a[15], b[7]);
    vst1q_u16(&c[352 + pad], tmp);
    //-----
    vmul(tmp, a[8], b[15]);
    vmla(tmp, a[9], b[14]);
    vmla(tmp, a[10], b[13]);
    vmla(tmp, a[11], b[12]);
    vmla(tmp, a[12], b[11]);
    vmla(tmp, a[13], b[10]);
    vmla(tmp, a[14], b[9]);
    vmla(tmp, a[15], b[8]);
    vst1q_u16(&c[368 + pad], tmp);
    //-----
    vmul(tmp, a[9], b[15]);
    vmla(tmp, a[10], b[14]);
    vmla(tmp, a[11], b[13]);
    vmla(tmp, a[12], b[12]);
    vmla(tmp, a[13], b[11]);
    vmla(tmp, a[14], b[10]);
    vmla(tmp, a[15], b[9]);
    vst1q_u16(&c[384 + pad], tmp);
    //-----
    vmul(tmp, a[10], b[15]);
    vmla(tmp, a[11], b[14]);
    vmla(tmp, a[12], b[13]);
    vmla(tmp, a[13], b[12]);
    vmla(tmp, a[14], b[11]);
    vmla(tmp, a[15], b[10]);
    vst1q_u16(&c[400 + pad], tmp);
    //-----
    vmul(tmp, a[11], b[15]);
    vmla(tmp, a[12], b[14]);
    vmla(tmp, a[13], b[13]);
    vmla(tmp, a[14], b[12]);
    vmla(tmp, a[15], b[11]);
    vst1q_u16(&c[416 + pad], tmp);
    //-----
    vmul(tmp, a[12], b[15]);
    vmla(tmp, a[13], b[14]);
    vmla(tmp, a[14], b[13]);
    vmla(tmp, a[15], b[12]);
    vst1q_u16(&c[432 + pad], tmp);
    //-----
    vmul(tmp, a[13], b[15]);
    vmla(tmp, a[14], b[14]);
    vmla(tmp, a[15], b[13]);
    vst1q_u16(&c[448 + pad], tmp);
    //-----
    vmul(tmp, a[14], b[15]);
    vmla(tmp, a[15], b[14]);
    vst1q_u16(&c[464 + pad], tmp);
    //-----
    vmul(tmp, a[15], b[15]);
    vst1q_u16(&c[480 + pad], tmp);
    //-----

    pad = 8;
  }
  vxor(tmp, tmp, tmp);
  vst1q_u16(&c[31*16], tmp);
  vst1q_u16(&c[31*16 + 8], tmp);
}
