#include "../../SABER_indcpa.h"
#include "../../SABER_params.h"
#include "asimd_matrix.c"
#include "asimd_scm_neon.h"

#include <arm_neon.h>
#include <stdint.h>

static inline 
void vload(uint16x8x2_t c, uint16_t *a)
{
    // load c <= a 
    c = vld2q_u16(a);
}

static inline 
void vstore(uint16_t *c, uint16x8x2_t a)
{
    // store c <= a 
    vst2q_u16(c, a);
}

static inline
void vcopy(uint16_t *c, uint16_t *a)
{
    uint16x8x2_t tmp; 
    vload(tmp, a);
    vstore(c, tmp);
}

static inline 
void vsl(uint16x8x2_t c, uint16x8x2_t a, uint16_t value)
{
    // c = a << value 
    c.val[0] = vshlq_n_u16(a.val[0], value);
    c.val[1] = vshlq_n_u16(a.val[1], value);
}

static inline 
void vsr(uint16x8x2_t c, uint16x8x2_t a, uint16_t value)
{
	// c = a >> value 
	c.val[0] = vshrq_n_u16(a.val[0], value);
  	c.val[1] = vshrq_n_u16(a.val[1], value);
}

static inline 
void vadd(uint16x8x2_t c, uint16x8x2_t a, uint16x8x2_t b)
{
    // c = a + b
    c.val[0] = vaddq_u16(a.val[0], b.val[0]);
    c.val[1] = vaddq_u16(a.val[1], b.val[1]);
}

static inline
void vsub(uint16x8x2_t c, uint16x8x2_t a, uint16x8x2_t b)
{
    // c = a - b
    c.val[0] = vsubq_u16(a.val[0], b.val[0]);
    c.val[1] = vsubq_u16(a.val[1], b.val[1]);

}

static inline
void vmuln(uint16x8x2_t c, uint16x8x2_t a, int16_t value)
{
	// c = a * value 
	c.val[0] = vmulq_n_u16(a.val[0], value);
  	c.val[1] = vmulq_n_u16(a.val[1], value);
}

static inline
void vxor(uint16x8x2_t c, uint16x8x2_t a, uint16x8x2_t b)
{
	// c = a ^ b 
	c.val[0] = veorq_u16(a.val[0], b.val[0]);
	c.val[1] = veorq_u16(a.val[1], b.val[1]);
}


uint16_t a_extra[2*16], b_extra[2*16];
uint16x8x2_t tmp;

// Position <= 14
static inline 
void karatsuba32_fork_avx_new(uint16x8x2_t *a1, uint16x8x2_t *b1, uint8_t position) {
  
  vstore(&a[position*16], a1[0]);
  vstore(&b[position*16], b1[0]);
  
  vstore(&a[(position + 1)*16], a1[1]);
  vstore(&b[(position + 1)*16], b1[1]);

  vadd(tmp, a1[0], a1[1]);
  vstore(&a[(position + 2)*16], tmp);

  vadd(tmp, b1[0], b1[1]);
  vstore(&b[(position + 2)*16], b1[0], b1[1]);

}

// Position > 14
static inline 
void karatsuba32_fork_avx_new1(uint16x8x2_t *a1, uint16x8x2_t *b1, uint8_t position) {

  vstore(&a[position*16], a1[0]);
  vstore(&b[position*16], b1[0]);
  
  vstore(&a_extra[0*16], a1[1]);
  vstore(&b_extra[0*16], b1[1]);

  vadd(tmp, a1[0], a1[1]);
  vstore(&a_extra[1*16], tmp);

  vadd(tmp, b1[0], b1[1]);
  vstore(&b_extra[1*16], tmp);

}

static inline 
void karatsuba32_fork_avx_partial(uint16x8x2_t *a1, uint16x8x2_t *b1, uint8_t position) {

  vstore(&a[position*16], a1[1]);
  vstore(&b[position*16], b1[1]);

  vadd(tmp, a1[0], a1[1]);
  vstore(&a[(position + 1)*16], tmp);

  vadd(tmp, b1[0], b1[1]);
  vstore(&b[(position + 1)*16], tmp);
}

static inline 
void karatsuba32_fork_avx_partial1(uint16x8x2_t *a1, uint16x8x2_t *b1, uint8_t position) {
  
  vadd(tmp, a1[0], a1[1]);
  vstore(&a[position*16], tmp);

  vadd(tmp, b1[0], b1[1]);
  vstore(&b[position*16], tmp);
}

static inline 
void karatsuba32_join_avx_new(uint16_t *result_final, uint8_t position) {

  uint16x8x2_t c1_tmp, c2_tmp;
  uint16x8x2_t b0, b1, b2;
  uint16x8x2_t rf[4];

  vload(rf[0], &c_avx[position*16 ] );
  vload(rf[3], &c_avx[(position + 1 + 16)*16 ] );

  vstore(&result_final[0*16], rf[0]);
  vstore(&result_final[3*16], rf[3]);

  vload(c1_tmp, &c_avx[(position + 16)*16]);
  vload(c2_tmp, &c_avx[(position + 2)*16]);
  // b[0] = resultd0[n-1:n/2] + resultd01[n/2-1:0]
  vadd(b0, c1_tmp, c2_tmp);

  vload(c1_tmp, &c_avx[(position + 2 + 16)*16]);
  vload(c2_tmp, &c_avx[(position + 1)*16]);
  // b[1] = resultd01[n-1:n/2] + resultd1[n/2-1:0]
  vadd(b1, c1_tmp, c2_tmp);

  // b[0] = b[0] - a[0] - a[2]
  vload(c1_tmp, c_avx[(position + 1)*16]);
  vsub(b2, b0, rf[0]);
  vsub(rf[1], b2, c1_tmp);
  vstore(&result_final[1*16], rf[1]);

  // b[1] = b[1] - a[1] - a[3]
  vload(c2_tmp, c_avx[(position + 16)*16]);
  vsub(b2, b1, c2_tmp);
  vsub(rf[2], b2, rf[3]);
  vstore(&result_final[2*16], rf[2]);

}

static inline 
void karatsuba32_join_avx_partial(uint16x8x2_t *result_final, uint8_t position) {
  COPY(result_final[0], c_avx_extra[0]);
  COPY(result_final[3], c_avx[position + 16]);

  // b[0] = resultd0[n-1:n/2] + resultd01[n/2-1:0]
  ADD(b[0], c_avx_extra[1], c_avx[position + 1]);

  // b[1] = resultd01[n-1:n/2] + resultd1[n/2-1:0]
  ADD(b[1], c_avx[position + 1 + 16], c_avx[position]);

  // b[0] = b[0] - a[0] - a[2]
  SUB(b[2], b[0], result_final[0]);
  SUB(result_final[1], b[2], c_avx[position]);
  // result_final[1].val[0] = vsubq_u16(b[2].val[0], c_avx[position].val[0]);
  // result_final[1].val[1] = vsubq_u16(b[2].val[1], c_avx[position].val[1]);

  // b[1] = b[1] - a[1] - a[3]
  SUB(b[2], b[1], c_avx_extra[1]);
  SUB(result_final[2], b[2], result_final[3]);
  // result_final[2].val[0] = vsubq_u16(b[2].val[0], result_final[3].val[0]);
  // result_final[2].val[1] = vsubq_u16(b[2].val[1], result_final[3].val[1]);
}

static inline 
void karatsuba32_join_avx_partial2(uint16x8x2_t *result_final, uint8_t position) {
  COPY(result_final[0], c_avx_extra[0]);
  COPY(result_final[3], c_avx_extra[3]);

  // b[0] = resultd0[n-1:n/2] + resultd01[n/2-1:0]
  ADD(b[0], c_avx_extra[1], c_avx[position]);

  // b[1] = resultd01[n-1:n/2] + resultd1[n/2-1:0]
  ADD(b[1], c_avx[position + 16], c_avx_extra[2]);

  // b[0] = b[0] - a[0] - a[2]
  SUB(b[2], b[0], result_final[0]);
  SUB(result_final[1], b[2], c_avx_extra[2]);
  // result_final[1].val[0] = vsubq_u16(b[2].val[0], c_avx_extra[2].val[0]);
  // result_final[1].val[1] = vsubq_u16(b[2].val[1], c_avx_extra[2].val[1]);

  // b[1] = b[1] - a[1] - a[3]
  SUB(b[2], b[1], c_avx_extra[1]);
  SUB(result_final[2], b[2], result_final[3]);
  // result_final[2].val[0] = vsubq_u16(b[2].val[0], result_final[3].val[0]);
  // result_final[2].val[1] = vsubq_u16(b[2].val[1], result_final[3].val[1]);
}

static inline 
void join_32coefficient_results(uint16x8x2_t *result_d0, uint16x8x2_t *result_d1,
                                              uint16x8x2_t *result_d01, uint16x8x2_t *result_64ks) {
  // {b[5],b[4]} = resultd0[63:32] + resultd01[31:0]
  ADD(b[4], result_d0[2], result_d01[0]);
  ADD(b[5], result_d0[3], result_d01[1]);

  // {b[7],b[6]} = resultd01[63:32] + resultd1[31:0]
  ADD(b[6], result_d01[2], result_d1[0]);
  ADD(b[7], result_d01[3], result_d1[1]);

  // {b[7],b[6],b[5],b[4]} <-- {b[7],b[6],b[5],b[4]} - {a[3],a[2],a[1],a[0]} - {a[7],a[6],a[5],a[4]}
  SUB(result_64ks[2], b[4], result_d0[0]);
  SUB(result_64ks[2], result_64ks[2], result_d1[0]);
  SUB(result_64ks[3], b[5], result_d0[1]);
  SUB(result_64ks[3], result_64ks[3], result_d1[1]);
  SUB(result_64ks[4], b[6], result_d0[2]);
  SUB(result_64ks[4], result_64ks[4], result_d1[2]);
  SUB(result_64ks[5], b[7], result_d0[3]);
  SUB(result_64ks[5], result_64ks[5], result_d1[3]);

  COPY(result_64ks[0], result_d0[0]);
  COPY(result_64ks[1], result_d0[1]);
  COPY(result_64ks[6], result_d1[2]);
  COPY(result_64ks[7], result_d1[3]);
}

/*
 * batch_64coefficient_multiplications
 * a0, a1, ... a6: In memory [64]
 * b0, b1, ... b6: In memory [64]
 * r...ult_final6: In memory [128]
 */

void batch_64coefficient_multiplications(
    uint16_t *a0, uint16_t *b0, uint16_t *result_final0, 
    uint16_t *a1, uint16_t *b1, uint16_t *result_final1, 
    uint16_t *a2, uint16_t *b2, uint16_t *result_final2, 
    uint16_t *a3, uint16_t *b3, uint16_t *result_final3,
    uint16_t *a4, uint16_t *b4, uint16_t *result_final4, 
    uint16_t *a5, uint16_t *b5, uint16_t *result_final5, 
    uint16_t *a6, uint16_t *b6, uint16_t *result_final6)
{
  uint16x8x2_t a_lu_temp[2], b_lu_temp[2];
  uint16x8x2_t result_d0[16], result_d1[16], result_d01[16];

  uint16x8x2_t a_tmp[4], b_tmp[4];
  
  uint16_t i;

  // KS splitting of 1st 64-coeff multiplication
  for (i = 0; i < 2; i++)
  {
    vload(a_tmp[i], &a0[i*16]);
    vload(a_tmp[2 + i], &a0[(2+i)*16]);
    vadd(a_lu_temp[i], a_tmp[i], a_tmp[2+i]);

    vload(b_tmp[i], &b0[i*16]);
    vload(b_tmp[2 + i], &b0[(2+i)*16]);
    vadd(b_lu_temp[i], b_tmp[i], b_tmp[2 + i]);
  }

  karatsuba32_fork_avx_new(&a_tmp[0], &b_tmp[0], 0);
  karatsuba32_fork_avx_new(&a_tmp[2], &b_tmp[2], 3);
  karatsuba32_fork_avx_new(a_lu_temp, b_lu_temp, 6); 

  // KS splitting of 2nd 64-coeff multiplication
  for (i = 0; i < 2; i++) {
    vload(a_tmp[i], &a1[i*16]);
    vload(a_tmp[2 + i], &a1[(2+i)*16]);
    vadd(a_lu_temp[i], a_tmp[i], a_tmp[2+i]);

    vload(b_tmp[i], &b1[i*16]);
    vload(b_tmp[2 + i], &b1[(2+i)*16]);
    vadd(b_lu_temp[i], b_tmp[i], b_tmp[2 + i]);
  }
  
  karatsuba32_fork_avx_new(&a_tmp[0], &b_tmp[0], 9);
  karatsuba32_fork_avx_new(&a_tmp[2], &b_tmp[2], 12);
  // Partial: loads only one of the three elements in the bucket
  karatsuba32_fork_avx_new1(a_lu_temp, b_lu_temp, 15); 

  // Compute 16 school-book multiplications in a batch.
  transpose(a);
  transpose(b);
  schoolbook_neon_new(c_avx, a, b);
  transpose(&c_avx[0]);
  transpose(&c_avx[16*16]);

  // store the partial multiplication result.
  vcopy(c_avx_extra[0], c_avx[15*16]);
  vcopy(c_avx_extra[1], c_avx[31*16]);

  karatsuba32_join_avx_new(result_d0, 0);
  karatsuba32_join_avx_new(result_d1, 3);
  karatsuba32_join_avx_new(result_d01, 6);

  // Final result of 1st 64-coeff multiplication
  join_32coefficient_results(result_d0, result_d1, result_d01, result_final0);

  karatsuba32_join_avx_new(result_d0, 9);
  karatsuba32_join_avx_new(result_d1, 12);

  // Fork 2 parts of previous operands
  karatsuba32_fork_avx_partial(a_lu_temp, b_lu_temp, 0);

  // Fork multiplication of a2*b2
  // for (i = 0; i < 2; i++) {
  //   ADD(a_lu_temp[i], a2[i], a2[2 + i]);
  //   ADD(b_lu_temp[i], b2[i], b2[2 + i]);
  // }
  // Unroll factor 2
  ADD(a_lu_temp[0], a2[0], a2[2 + 0]);
  ADD(b_lu_temp[0], b2[0], b2[2 + 0]);
  ADD(a_lu_temp[1], a2[1], a2[2 + 1]);
  ADD(b_lu_temp[1], b2[1], b2[2 + 1]);

  karatsuba32_fork_avx_new(&a2[0], &b2[0], 2);
  karatsuba32_fork_avx_new(&a2[2], &b2[2], 5);
  karatsuba32_fork_avx_new(a_lu_temp, b_lu_temp, 8);

  // Fork multiplication of a3*b3
  // for (i = 0; i < 2; i++) {
  //   ADD(a_lu_temp[i], a3[i], a3[2 + i]);
  //   ADD(b_lu_temp[i], b3[i], b3[2 + i]);
  // }
  // Unroll factor 2 
  ADD(a_lu_temp[0], a3[0], a3[2 + 0]);
  ADD(b_lu_temp[0], b3[0], b3[2 + 0]);
  ADD(a_lu_temp[1], a3[1], a3[2 + 1]);
  ADD(b_lu_temp[1], b3[1], b3[2 + 1]);


  karatsuba32_fork_avx_new(&a3[0], &b3[0], 11);
  // Partial: loads only two of the three elements in the bucket
  karatsuba32_fork_avx_new(&a3[2], &b3[2], 14); 

  transpose(a);
  transpose(b);
  schoolbook_avx_new();

  transpose(&c_avx[0]);
  transpose(&c_avx[16]);

  karatsuba32_join_avx_partial(result_d01, 0); // Combine results of this computation with previous computation
  // Final result of 2nd 64-coeff multiplication
  join_32coefficient_results(result_d0, result_d1, result_d01, result_final1);

  // store the partial multiplication result. they will be combined after next batch multiplication
  COPY(c_avx_extra[0], c_avx[14]);
  COPY(c_avx_extra[1], c_avx[14 + 16]);
  COPY(c_avx_extra[2], c_avx[15]);
  COPY(c_avx_extra[3], c_avx[15 + 16]);

  karatsuba32_join_avx_new(result_d0, 2);
  karatsuba32_join_avx_new(result_d1, 5);
  karatsuba32_join_avx_new(result_d01, 8);

  // Final result of 3rd 64-coeff multiplication
  join_32coefficient_results(result_d0, result_d1, result_d01, result_final2);

  // Join d0 of 4th 64-coeff multiplication
  karatsuba32_join_avx_new(result_d0, 11);

  // Fork 1 part of previous operands
  karatsuba32_fork_avx_partial1(&a3[2], &b3[2], 0);
  karatsuba32_fork_avx_new(a_lu_temp, b_lu_temp, 1);

  // Fork multiplication of a4*b4
  // for (i = 0; i < 2; i++) {
  //   ADD(a_lu_temp[i], a4[i], a4[2 + i]);
  //   ADD(b_lu_temp[i], b4[i], b4[2 + i]);
  // }
  ADD(a_lu_temp[0], a4[0], a4[2 + 0]);
  ADD(b_lu_temp[0], b4[0], b4[2 + 0]);
  ADD(a_lu_temp[1], a4[1], a4[2 + 1]);
  ADD(b_lu_temp[1], b4[1], b4[2 + 1]);

  karatsuba32_fork_avx_new(&a4[0], &b4[0], 4);
  karatsuba32_fork_avx_new(&a4[2], &b4[2], 7);
  karatsuba32_fork_avx_new(a_lu_temp, b_lu_temp, 10);

  // Fork multiplication of a5*b5
  // for (i = 0; i < 2; i++) {
  //   ADD(a_lu_temp[i], a5[i], a5[2 + i]);
  //   ADD(b_lu_temp[i], b5[i], b5[2 + i]);
  // }
  // Unroll factor 2
  ADD(a_lu_temp[0], a5[0], a5[2 + 0]);
  ADD(b_lu_temp[0], b5[0], b5[2 + 0]);
  ADD(a_lu_temp[1], a5[1], a5[2 + 1]);
  ADD(b_lu_temp[1], b5[1], b5[2 + 1]);

  karatsuba32_fork_avx_new(&a5[0], &b5[0], 13);

  transpose(a);
  transpose(b);
  schoolbook_avx_new();

  transpose(&c_avx[0]);
  transpose(&c_avx[16]);

  karatsuba32_join_avx_partial2(result_d1, 0);
  karatsuba32_join_avx_new(result_d01, 1);

  // Final result of 4th 64-coeff multiplication
  join_32coefficient_results(result_d0, result_d1, result_d01, result_final3);

  karatsuba32_join_avx_new(result_d0, 4);
  karatsuba32_join_avx_new(result_d1, 7);
  karatsuba32_join_avx_new(result_d01, 10);

  // Final result of 5th 64-coeff multiplication
  join_32coefficient_results(result_d0, result_d1, result_d01, result_final4);

  karatsuba32_join_avx_new(result_d0, 13);

  // Fork remaining 2 parts of a5*b5
  karatsuba32_fork_avx_new(&a5[2], &b5[2], 0);
  karatsuba32_fork_avx_new(a_lu_temp, b_lu_temp, 3);

  // Fork multiplication of a6*b6
  // for (i = 0; i < 2; i++) {
  //   ADD(a_lu_temp[i], a6[i], a6[2 + i]);
  //   ADD(b_lu_temp[i], b6[i], b6[2 + i]);
  // }
  // Unroll factor 2
  ADD(a_lu_temp[0], a6[0], a6[2 + 0]);
  ADD(b_lu_temp[0], b6[0], b6[2 + 0]);  
  ADD(a_lu_temp[1], a6[1], a6[2 + 1]);  
  ADD(b_lu_temp[1], b6[1], b6[2 + 1]);  

  karatsuba32_fork_avx_new(&a6[0], &b6[0], 6);
  karatsuba32_fork_avx_new(&a6[2], &b6[2], 9);
  karatsuba32_fork_avx_new(a_lu_temp, b_lu_temp, 12);

  transpose(a);
  transpose(b);
  schoolbook_avx_new();

  transpose(&c_avx[0]);
  transpose(&c_avx[16]);

  karatsuba32_join_avx_new(result_d1, 0);
  karatsuba32_join_avx_new(result_d01, 3);

  // Final result of 6th 64-coeff multiplication
  join_32coefficient_results(result_d0, result_d1, result_d01, result_final5);

  karatsuba32_join_avx_new(result_d0, 6);
  karatsuba32_join_avx_new(result_d1, 9);
  karatsuba32_join_avx_new(result_d01, 12);

  // Final result of 6th 64-coeff multiplication
  join_32coefficient_results(result_d0, result_d1, result_d01, result_final6);
}