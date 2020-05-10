#include <arm_neon.h>
#include "asimd_scm_neon.h"

// c = aa * bb
#define vmul(c, aa, bb) c = vmulq_u16(aa, bb);

// c += aa*bb 
#define vmla(c, aa, bb) c = vmlaq_u16(c, aa, bb);

// Load mem to c 
#define vload(c, mem) c = vld1q_u16(mem);

// c = aa ^ bb 
#define vxor(c, aa, bb) c = veorq_u16(aa, bb);

// static inline
// void vmul(uint16x8_t c, uint16x8_t aa, uint16x8_t bb)
// {
//     // c = aa * bb
//     c = vmulq_u16(aa, bb);
// }

// static inline
// void vmla(uint16x8_t c, uint16x8_t aa, uint16x8_t bb)
// {
//     // c += aa*bb 
//     c = vmlaq_u16(c, aa, bb);
// }

// static inline
// void vload(uint16x8_t c, uint16_t *mem)
// {
//     // Load mem to c 
//     c = vld1q_u16(mem);
// }

// static inline
// void vxor(uint16x8_t c, uint16x8_t aa, uint16x8_t bb)
// {
// 	// c = aa ^ bb 
// 	c = veorq_u16(aa, bb);
// }

/*
 * schoolbook_neon_new
 * Assume aa, bb are already loaded
 */
void schoolbook_neon_new(uint16_t *restrict c_mem, uint16_t *restrict a_mem, uint16_t *restrict b_mem){
  uint16x8_t tmp, aa[16], bb[16];
  uint16_t i, j;
  uint16_t pad = 0;
  for (i = 0; i < 2; i++) {
    for (j = 0; j < 16; j++)
    {
        vload(aa[j], &a_mem[j*16 + pad]);
        vload(bb[j], &b_mem[j*16 + pad]);
    }

    vmul(tmp, aa[0], bb[0]);
    vst1q_u16(&c_mem[0 + pad], tmp);
    //----
    vmul(tmp, aa[0], bb[1]);
    vmla(tmp, aa[1], bb[0]);
    vst1q_u16(&c_mem[16 + pad], tmp);
    //----
    vmul(tmp, aa[0], bb[2]);
    vmla(tmp, aa[1], bb[1]);
    vmla(tmp, aa[2], bb[0]);
    vst1q_u16(&c_mem[32 + pad], tmp);
    //----
    vmul(tmp, aa[0], bb[3]);
    vmla(tmp, aa[1], bb[2]);
    vmla(tmp, aa[2], bb[1]);
    vmla(tmp, aa[3], bb[0]);
    vst1q_u16(&c_mem[48 + pad], tmp);
    //----
    vmul(tmp, aa[0], bb[4]);
    vmla(tmp, aa[1], bb[3]);
    vmla(tmp, aa[2], bb[2]);
    vmla(tmp, aa[3], bb[1]);
    vmla(tmp, aa[4], bb[0]);
    vst1q_u16(&c_mem[64 + pad], tmp);
    //----
    vmul(tmp, aa[0], bb[5]);
    vmla(tmp, aa[1], bb[4]);
    vmla(tmp, aa[2], bb[3]);
    vmla(tmp, aa[3], bb[2]);
    vmla(tmp, aa[4], bb[1]);
    vmla(tmp, aa[5], bb[0]);
    vst1q_u16(&c_mem[80 + pad], tmp);
    //----
    vmul(tmp, aa[0], bb[6]);
    vmla(tmp, aa[1], bb[5]);
    vmla(tmp, aa[2], bb[4]);
    vmla(tmp, aa[3], bb[3]);
    vmla(tmp, aa[4], bb[2]);
    vmla(tmp, aa[5], bb[1]);
    vmla(tmp, aa[6], bb[0]);
    vst1q_u16(&c_mem[96 + pad], tmp);
    //----
    vmul(tmp, aa[0], bb[7]);
    vmla(tmp, aa[1], bb[6]);
    vmla(tmp, aa[2], bb[5]);
    vmla(tmp, aa[3], bb[4]);
    vmla(tmp, aa[4], bb[3]);
    vmla(tmp, aa[5], bb[2]);
    vmla(tmp, aa[6], bb[1]);
    vmla(tmp, aa[7], bb[0]);
    vst1q_u16(&c_mem[112 + pad], tmp);
    //----
    vmul(tmp, aa[0], bb[8]);
    vmla(tmp, aa[1], bb[7]);
    vmla(tmp, aa[2], bb[6]);
    vmla(tmp, aa[3], bb[5]);
    vmla(tmp, aa[4], bb[4]);
    vmla(tmp, aa[5], bb[3]);
    vmla(tmp, aa[6], bb[2]);
    vmla(tmp, aa[7], bb[1]);
    vmla(tmp, aa[8], bb[0]);
    vst1q_u16(&c_mem[128 + pad], tmp);
    //----
    vmul(tmp, aa[0], bb[9]);
    vmla(tmp, aa[1], bb[8]);
    vmla(tmp, aa[2], bb[7]);
    vmla(tmp, aa[3], bb[6]);
    vmla(tmp, aa[4], bb[5]);
    vmla(tmp, aa[5], bb[4]);
    vmla(tmp, aa[6], bb[3]);
    vmla(tmp, aa[7], bb[2]);
    vmla(tmp, aa[8], bb[1]);
    vmla(tmp, aa[9], bb[0]);
    vst1q_u16(&c_mem[144 + pad], tmp);
    //----
    vmul(tmp, aa[0], bb[10]);
    vmla(tmp, aa[1], bb[9]);
    vmla(tmp, aa[2], bb[8]);
    vmla(tmp, aa[3], bb[7]);
    vmla(tmp, aa[4], bb[6]);
    vmla(tmp, aa[5], bb[5]);
    vmla(tmp, aa[6], bb[4]);
    vmla(tmp, aa[7], bb[3]);
    vmla(tmp, aa[8], bb[2]);
    vmla(tmp, aa[9], bb[1]);
    vmla(tmp, aa[10], bb[0]);
    vst1q_u16(&c_mem[160 + pad], tmp);
    //----
    vmul(tmp, aa[0], bb[11]);
    vmla(tmp, aa[1], bb[10]);
    vmla(tmp, aa[2], bb[9]);
    vmla(tmp, aa[3], bb[8]);
    vmla(tmp, aa[4], bb[7]);
    vmla(tmp, aa[5], bb[6]);
    vmla(tmp, aa[6], bb[5]);
    vmla(tmp, aa[7], bb[4]);
    vmla(tmp, aa[8], bb[3]);
    vmla(tmp, aa[9], bb[2]);
    vmla(tmp, aa[10], bb[1]);
    vmla(tmp, aa[11], bb[0]);
    vst1q_u16(&c_mem[176 + pad], tmp);
    //----
    vmul(tmp, aa[0], bb[12]);
    vmla(tmp, aa[1], bb[11]);
    vmla(tmp, aa[2], bb[10]);
    vmla(tmp, aa[3], bb[9]);
    vmla(tmp, aa[4], bb[8]);
    vmla(tmp, aa[5], bb[7]);
    vmla(tmp, aa[6], bb[6]);
    vmla(tmp, aa[7], bb[5]);
    vmla(tmp, aa[8], bb[4]);
    vmla(tmp, aa[9], bb[3]);
    vmla(tmp, aa[10], bb[2]);
    vmla(tmp, aa[11], bb[1]);
    vmla(tmp, aa[12], bb[0]);
    vst1q_u16(&c_mem[192 + pad], tmp);
    //----
    vmul(tmp, aa[0], bb[13]);
    vmla(tmp, aa[1], bb[12]);
    vmla(tmp, aa[2], bb[11]);
    vmla(tmp, aa[3], bb[10]);
    vmla(tmp, aa[4], bb[9]);
    vmla(tmp, aa[5], bb[8]);
    vmla(tmp, aa[6], bb[7]);
    vmla(tmp, aa[7], bb[6]);
    vmla(tmp, aa[8], bb[5]);
    vmla(tmp, aa[9], bb[4]);
    vmla(tmp, aa[10], bb[3]);
    vmla(tmp, aa[11], bb[2]);
    vmla(tmp, aa[12], bb[1]);
    vmla(tmp, aa[13], bb[0]);
    vst1q_u16(&c_mem[208 + pad], tmp);
    //----
    vmul(tmp, aa[0], bb[14]);
    vmla(tmp, aa[1], bb[13]);
    vmla(tmp, aa[2], bb[12]);
    vmla(tmp, aa[3], bb[11]);
    vmla(tmp, aa[4], bb[10]);
    vmla(tmp, aa[5], bb[9]);
    vmla(tmp, aa[6], bb[8]);
    vmla(tmp, aa[7], bb[7]);
    vmla(tmp, aa[8], bb[6]);
    vmla(tmp, aa[9], bb[5]);
    vmla(tmp, aa[10], bb[4]);
    vmla(tmp, aa[11], bb[3]);
    vmla(tmp, aa[12], bb[2]);
    vmla(tmp, aa[13], bb[1]);
    vmla(tmp, aa[14], bb[0]);
    vst1q_u16(&c_mem[224 + pad], tmp);
    //----
    vmul(tmp, aa[0], bb[15]);
    vmla(tmp, aa[1], bb[14]);
    vmla(tmp, aa[2], bb[13]);
    vmla(tmp, aa[3], bb[12]);
    vmla(tmp, aa[4], bb[11]);
    vmla(tmp, aa[5], bb[10]);
    vmla(tmp, aa[6], bb[9]);
    vmla(tmp, aa[7], bb[8]);
    vmla(tmp, aa[8], bb[7]);
    vmla(tmp, aa[9], bb[6]);
    vmla(tmp, aa[10], bb[5]);
    vmla(tmp, aa[11], bb[4]);
    vmla(tmp, aa[12], bb[3]);
    vmla(tmp, aa[13], bb[2]);
    vmla(tmp, aa[14], bb[1]);
    vmla(tmp, aa[15], bb[0]);
    vst1q_u16(&c_mem[240 + pad], tmp);
    //----
    //=======
    // ci =  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15} 16
    // ai =  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15} 16
    // bi =  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15} 16
    // ----------------PART 2----------------
    vmul(tmp, aa[1], bb[15]);
    vmla(tmp, aa[2], bb[14]);
    vmla(tmp, aa[3], bb[13]);
    vmla(tmp, aa[4], bb[12]);
    vmla(tmp, aa[5], bb[11]);
    vmla(tmp, aa[6], bb[10]);
    vmla(tmp, aa[7], bb[9]);
    vmla(tmp, aa[8], bb[8]);
    vmla(tmp, aa[9], bb[7]);
    vmla(tmp, aa[10], bb[6]);
    vmla(tmp, aa[11], bb[5]);
    vmla(tmp, aa[12], bb[4]);
    vmla(tmp, aa[13], bb[3]);
    vmla(tmp, aa[14], bb[2]);
    vmla(tmp, aa[15], bb[1]);
    vst1q_u16(&c_mem[256 + pad], tmp);
    //-----
    vmul(tmp, aa[2], bb[15]);
    vmla(tmp, aa[3], bb[14]);
    vmla(tmp, aa[4], bb[13]);
    vmla(tmp, aa[5], bb[12]);
    vmla(tmp, aa[6], bb[11]);
    vmla(tmp, aa[7], bb[10]);
    vmla(tmp, aa[8], bb[9]);
    vmla(tmp, aa[9], bb[8]);
    vmla(tmp, aa[10], bb[7]);
    vmla(tmp, aa[11], bb[6]);
    vmla(tmp, aa[12], bb[5]);
    vmla(tmp, aa[13], bb[4]);
    vmla(tmp, aa[14], bb[3]);
    vmla(tmp, aa[15], bb[2]);
    vst1q_u16(&c_mem[272 + pad], tmp);
    //-----
    vmul(tmp, aa[3], bb[15]);
    vmla(tmp, aa[4], bb[14]);
    vmla(tmp, aa[5], bb[13]);
    vmla(tmp, aa[6], bb[12]);
    vmla(tmp, aa[7], bb[11]);
    vmla(tmp, aa[8], bb[10]);
    vmla(tmp, aa[9], bb[9]);
    vmla(tmp, aa[10], bb[8]);
    vmla(tmp, aa[11], bb[7]);
    vmla(tmp, aa[12], bb[6]);
    vmla(tmp, aa[13], bb[5]);
    vmla(tmp, aa[14], bb[4]);
    vmla(tmp, aa[15], bb[3]);
    vst1q_u16(&c_mem[288 + pad], tmp);
    //-----
    vmul(tmp, aa[4], bb[15]);
    vmla(tmp, aa[5], bb[14]);
    vmla(tmp, aa[6], bb[13]);
    vmla(tmp, aa[7], bb[12]);
    vmla(tmp, aa[8], bb[11]);
    vmla(tmp, aa[9], bb[10]);
    vmla(tmp, aa[10], bb[9]);
    vmla(tmp, aa[11], bb[8]);
    vmla(tmp, aa[12], bb[7]);
    vmla(tmp, aa[13], bb[6]);
    vmla(tmp, aa[14], bb[5]);
    vmla(tmp, aa[15], bb[4]);
    vst1q_u16(&c_mem[304 + pad], tmp);
    //-----
    vmul(tmp, aa[5], bb[15]);
    vmla(tmp, aa[6], bb[14]);
    vmla(tmp, aa[7], bb[13]);
    vmla(tmp, aa[8], bb[12]);
    vmla(tmp, aa[9], bb[11]);
    vmla(tmp, aa[10], bb[10]);
    vmla(tmp, aa[11], bb[9]);
    vmla(tmp, aa[12], bb[8]);
    vmla(tmp, aa[13], bb[7]);
    vmla(tmp, aa[14], bb[6]);
    vmla(tmp, aa[15], bb[5]);
    vst1q_u16(&c_mem[320 + pad], tmp);
    //-----
    vmul(tmp, aa[6], bb[15]);
    vmla(tmp, aa[7], bb[14]);
    vmla(tmp, aa[8], bb[13]);
    vmla(tmp, aa[9], bb[12]);
    vmla(tmp, aa[10], bb[11]);
    vmla(tmp, aa[11], bb[10]);
    vmla(tmp, aa[12], bb[9]);
    vmla(tmp, aa[13], bb[8]);
    vmla(tmp, aa[14], bb[7]);
    vmla(tmp, aa[15], bb[6]);
    vst1q_u16(&c_mem[336 + pad], tmp);
    //-----
    vmul(tmp, aa[7], bb[15]);
    vmla(tmp, aa[8], bb[14]);
    vmla(tmp, aa[9], bb[13]);
    vmla(tmp, aa[10], bb[12]);
    vmla(tmp, aa[11], bb[11]);
    vmla(tmp, aa[12], bb[10]);
    vmla(tmp, aa[13], bb[9]);
    vmla(tmp, aa[14], bb[8]);
    vmla(tmp, aa[15], bb[7]);
    vst1q_u16(&c_mem[352 + pad], tmp);
    //-----
    vmul(tmp, aa[8], bb[15]);
    vmla(tmp, aa[9], bb[14]);
    vmla(tmp, aa[10], bb[13]);
    vmla(tmp, aa[11], bb[12]);
    vmla(tmp, aa[12], bb[11]);
    vmla(tmp, aa[13], bb[10]);
    vmla(tmp, aa[14], bb[9]);
    vmla(tmp, aa[15], bb[8]);
    vst1q_u16(&c_mem[368 + pad], tmp);
    //-----
    vmul(tmp, aa[9], bb[15]);
    vmla(tmp, aa[10], bb[14]);
    vmla(tmp, aa[11], bb[13]);
    vmla(tmp, aa[12], bb[12]);
    vmla(tmp, aa[13], bb[11]);
    vmla(tmp, aa[14], bb[10]);
    vmla(tmp, aa[15], bb[9]);
    vst1q_u16(&c_mem[384 + pad], tmp);
    //-----
    vmul(tmp, aa[10], bb[15]);
    vmla(tmp, aa[11], bb[14]);
    vmla(tmp, aa[12], bb[13]);
    vmla(tmp, aa[13], bb[12]);
    vmla(tmp, aa[14], bb[11]);
    vmla(tmp, aa[15], bb[10]);
    vst1q_u16(&c_mem[400 + pad], tmp);
    //-----
    vmul(tmp, aa[11], bb[15]);
    vmla(tmp, aa[12], bb[14]);
    vmla(tmp, aa[13], bb[13]);
    vmla(tmp, aa[14], bb[12]);
    vmla(tmp, aa[15], bb[11]);
    vst1q_u16(&c_mem[416 + pad], tmp);
    //-----
    vmul(tmp, aa[12], bb[15]);
    vmla(tmp, aa[13], bb[14]);
    vmla(tmp, aa[14], bb[13]);
    vmla(tmp, aa[15], bb[12]);
    vst1q_u16(&c_mem[432 + pad], tmp);
    //-----
    vmul(tmp, aa[13], bb[15]);
    vmla(tmp, aa[14], bb[14]);
    vmla(tmp, aa[15], bb[13]);
    vst1q_u16(&c_mem[448 + pad], tmp);
    //-----
    vmul(tmp, aa[14], bb[15]);
    vmla(tmp, aa[15], bb[14]);
    vst1q_u16(&c_mem[464 + pad], tmp);
    //-----
    vmul(tmp, aa[15], bb[15]);
    vst1q_u16(&c_mem[480 + pad], tmp);
    //-----

    pad = 8;
  }
  vxor(tmp, tmp, tmp);
  vst1q_u16(&c_mem[31*16], tmp);
  vst1q_u16(&c_mem[31*16 + 8], tmp);
}
