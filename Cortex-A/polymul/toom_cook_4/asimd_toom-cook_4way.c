#include "asimd_scm_avx.c"
#include "asimd_matrix.c"
#include "../../SABER_params.h"
#include "../../SABER_indcpa.h"

#include <arm_neon.h>
#include <stdint.h>

// TODO: chagne "avx" in function name to "neon"
// -> | -> ->
#define ADD(c, a, b)                             \
	c->val[0] = vaddq_u16(a->val[0], b->val[0]); \
	c->val[1] = vaddq_u16(a->val[1], b->val[1])
// . | . . 
#define ADDDOT_DOT(c, a, b)                   \
	c.val[0] = vaddq_u16(a.val[0], b.val[0]); \
	c.val[1] = vaddq_u16(a.val[1], b.val[1])
// . | -> -> 
#define ADDDOT(c, a, b)                         \
	c.val[0] = vaddq_u16(a->val[0], b->val[0]); \
	c.val[1] = vaddq_u16(a->val[1], b->val[1])
// . | . ->
#define ADD_DOT(c, a, b)                       \
	c.val[0] = vaddq_u16(a.val[0], b->val[0]); \
	c.val[1] = vaddq_u16(a.val[1], b->val[1])
// -> | -> ->
#define SUB(c, a, b)                             \
	c->val[0] = vsubq_u16(a->val[0], b->val[0]); \
	c->val[1] = vsubq_u16(a->val[1], b->val[1])
// . | -> -> 
#define SUBDOT(c, a, b)                         \
	c.val[0] = vsubq_u16(a->val[0], b->val[0]); \
	c.val[1] = vsubq_u16(a->val[1], b->val[1])
// . | . ->
#define SUB_DOT(c, a, b)                       \
	c.val[0] = vsubq_u16(a.val[0], b->val[0]); \
	c.val[1] = vsubq_u16(a.val[1], b->val[1])
// . | . . 
#define SUBDOT_DOT(c, a, b)                   \
	c.val[0] = vsubq_u16(a.val[0], b.val[0]); \
	c.val[1] = vsubq_u16(a.val[1], b.val[1])
// -> | ->
#define COPY(c, b)                                  \
	c->val[0] = veorq_u16(b->val[0], vdupq_n_u16(0)); \
	c->val[1] = veorq_u16(b->val[1], vdupq_n_u16(0))
// . | .
#define COPYDOT_DOT(c, b)                                  \
	c.val[0] = veorq_u16(b.val[0], vdupq_n_u16(0)); \
	c.val[1] = veorq_u16(b.val[1], vdupq_n_u16(0))
// -> | .
#define COPY_DOT(c, b)                                  \
	c->val[0] = veorq_u16(b.val[0], vdupq_n_u16(0)); \
	c->val[1] = veorq_u16(b.val[1], vdupq_n_u16(0))
// . | ->
#define COPYDOT(c, b)                                  \
	c.val[0] = veorq_u16(b->val[0], vdupq_n_u16(0)); \
	c.val[1] = veorq_u16(b->val[1], vdupq_n_u16(0))

#define SLLDOT_DOT(c, a, value)              \
	c.val[0] = vshlq_n_u16(a.val[0], value); \
	c.val[1] = vshlq_n_u16(a.val[1], value)

#define SRLDOT_DOT(c, a, value)              \
	c.val[0] = vshrq_n_u16(a.val[0], value); \
	c.val[1] = vshrq_n_u16(a.val[1], value)

#define XORDOT_DOT(c, a, b)                   \
	c.val[0] = veorq_u16(a.val[0], b.val[0]); \
	c.val[1] = veorq_u16(a.val[1], b.val[1])

#define MULNDOT_DOT(c, a, value)              \
	c.val[0] = vmulq_n_u16(a.val[0], value); \
	c.val[1] = vmulq_n_u16(a.val[1], value)

uint16x8x2_t a_extra[2], b_extra[2];

void karatsuba32_fork_avx_new(uint16x8x2_t **a1,
							  uint16x8x2_t **b1,
							  uint8_t position)
{
	COPYDOT(a[position], a1[0]);
	COPYDOT(b[position], b1[0]);

	if ((position + 1) > 15)
	{
		COPYDOT(a_extra[0] , a1[1]);
		COPYDOT(b_extra[0] , b1[1]);

		ADDDOT(a_extra[1], a1[0], a1[1]);
		ADDDOT(b_extra[1], b1[0], b1[1]);
	}
	else
	{
		COPYDOT(a[position + 1] , a1[1]);
		COPYDOT(b[position + 1] , b1[1]);

		ADDDOT(a[position + 2], a1[0], a1[1]);
		ADDDOT(b[position + 2], b1[0], b1[1]);
	}
}

void karatsuba32_fork_avx_partial(uint16x8x2_t **a1,
								  uint16x8x2_t **b1,
								  uint8_t position)
{
	COPYDOT(a[position], a1[1]);
	COPYDOT(b[position], b1[1]);

	ADDDOT(a[position + 1], a1[0], a1[1]);
	ADDDOT(b[position + 1], b1[0], b1[1]);
}

void karatsuba32_fork_avx_partial1(uint16x8x2_t **a1,
								   uint16x8x2_t **b1,
								   uint8_t position)
{
	ADDDOT(a[position], a1[0], a1[1]);
	ADDDOT(b[position], b1[0], b1[1]);
}

void karatsuba32_join_avx_new(uint16x8x2_t **result_final,
							  uint8_t position)
{
	COPY_DOT(result_final[0], c_avx[position]);
	COPY_DOT(result_final[3], c_avx[position + 1 + 16]);

	// b[0] = resultd0[n-1:n/2] + resultd01[n/2-1:0]
	ADDDOT_DOT(b[0], c_avx[position + 16], c_avx[position + 2]);

	// b[1] = resultd01[n-1:n/2] + resultd1[n/2-1:0]
	ADDDOT_DOT(b[1], c_avx[position + 2 + 16], c_avx[position + 1]);

	// b[0] = b[0] - a[0] - a[2]
	SUB_DOT(b[2], b[0], result_final[0]);
	// SUB_DOTDOT(result_final[1], b[2], c_avx[position + 1]);
	result_final[1]->val[0] = vsubq_u16(b[2].val[0], c_avx[position + 1].val[0]);
	result_final[1]->val[1] = vsubq_u16(b[2].val[1], c_avx[position + 1].val[1]);
	// b[1] = b[1] - a[1] - a[3]
	SUBDOT_DOT(b[2], b[1], c_avx[position + 16]);
	// SUB(result_final[2], b[2], result_final[3]);
	result_final[2]->val[0] = vsubq_u16(b[2].val[0], result_final[3]->val[0]);
	result_final[2]->val[1] = vsubq_u16(b[2].val[1], result_final[3]->val[1]);
}

void karatsuba32_join_avx_partial(uint16x8x2_t **result_final,
								  uint8_t position)
{
	COPY_DOT(result_final[0] , c_avx_extra[0]);
	COPY_DOT(result_final[3] , c_avx[position + 16]);

	// b[0] = resultd0[n-1:n/2] + resultd01[n/2-1:0]
	ADDDOT_DOT(b[0], c_avx_extra[1], c_avx[position + 1]);

	// b[1] = resultd01[n-1:n/2] + resultd1[n/2-1:0]
	ADDDOT_DOT(b[1], c_avx[position + 1 + 16], c_avx[position]);

	// b[0] = b[0] - a[0] - a[2]
	SUB_DOT(b[2], b[0], result_final[0]);
	// SUB_DOT(result_final[1], b[2], c_avx[position]);
	result_final[1]->val[0] = vsubq_u16(b[2].val[0], c_avx[position].val[0]);
	result_final[1]->val[1] = vsubq_u16(b[2].val[1], c_avx[position].val[1]);
	
	// b[1] = b[1] - a[1] - a[3]
	SUBDOT_DOT(b[2], b[1], c_avx_extra[1]);
	// SUB(result_final[2], b[2], result_final[3]);
	result_final[2]->val[0] = vsubq_u16(b[2].val[0], result_final[3]->val[0]);
	result_final[2]->val[1] = vsubq_u16(b[2].val[1], result_final[3]->val[1]);
}

void karatsuba32_join_avx_partial2(uint16x8x2_t **result_final,
								   uint8_t position)
{
	COPY_DOT(result_final[0], c_avx_extra[0]);
	COPY_DOT(result_final[3], c_avx_extra[3]);

	// b[0] = resultd0[n-1:n/2] + resultd01[n/2-1:0]
	ADDDOT_DOT(b[0], c_avx_extra[1], c_avx[position]);

	// b[1] = resultd01[n-1:n/2] + resultd1[n/2-1:0]
	ADDDOT_DOT(b[1], c_avx[position + 16], c_avx_extra[2]);

	// b[0] = b[0] - a[0] - a[2]
	SUB_DOT(b[2], b[0], result_final[0]);
	// SUB(result_final[1], b[2], c_avx_extra[2]);
	result_final[1]->val[0] = vsubq_u16(b[2].val[0], c_avx_extra[2].val[0]);
	result_final[1]->val[1] = vsubq_u16(b[2].val[1], c_avx_extra[2].val[1]);
	

	// b[1] = b[1] - a[1] - a[3]
	SUBDOT_DOT(b[2], b[1], c_avx_extra[1]);
	// SUB(result_final[2], b[2], result_final[3]);
	result_final[2]->val[0] = vsubq_u16(b[2].val[0], result_final[3]->val[0]);
	result_final[2]->val[1] = vsubq_u16(b[2].val[1], result_final[3]->val[1]);
}

void join_32coefficient_results(uint16x8x2_t *result_d0[],
								uint16x8x2_t *result_d1[],
								uint16x8x2_t *result_d01[],
								uint16x8x2_t *result_64ks[])
{
	// {b[5],b[4]} = resultd0[63:32] + resultd01[31:0]
	ADDDOT(b[4], result_d0[2], result_d01[0]);
	ADDDOT(b[5], result_d0[3], result_d01[1]);

	// {b[7],b[6]} = resultd01[63:32] + resultd1[31:0]
	ADDDOT(b[6], result_d01[2], result_d1[0]);
	ADDDOT(b[7], result_d01[3], result_d1[1]);

	// {b[7],b[6],b[5],b[4]} <-- {b[7],b[6],b[5],b[4]} - {a[3],a[2],a[1],a[0]} - {a[7],a[6],a[5],a[4]}
	SUB(result_64ks[2], (&b[4]), result_d0[0]);
	SUB(result_64ks[2], result_64ks[2], result_d1[0]);
	SUB(result_64ks[3], (&b[5]), result_d0[1]);
	SUB(result_64ks[3], result_64ks[3], result_d1[1]);
	SUB(result_64ks[4], (&b[6]), result_d0[2]);
	SUB(result_64ks[4], result_64ks[4], result_d1[2]);
	SUB(result_64ks[5], (&b[7]), result_d0[3]);
	SUB(result_64ks[5], result_64ks[5], result_d1[3]);

	COPY(result_64ks[0], result_d0[0]);
	COPY(result_64ks[1], result_d0[1]);
	COPY(result_64ks[6], result_d1[2]);
	COPY(result_64ks[7], result_d1[3]);
}

void batch_64coefficient_multiplications(
	uint16x8x2_t **a0, uint16x8x2_t **b0, uint16x8x2_t **result_final0,
	uint16x8x2_t **a1, uint16x8x2_t **b1, uint16x8x2_t **result_final1,
	uint16x8x2_t **a2, uint16x8x2_t **b2, uint16x8x2_t **result_final2,
	uint16x8x2_t **a3, uint16x8x2_t **b3, uint16x8x2_t **result_final3,
	uint16x8x2_t **a4, uint16x8x2_t **b4, uint16x8x2_t **result_final4,
	uint16x8x2_t **a5, uint16x8x2_t **b5, uint16x8x2_t **result_final5,
	uint16x8x2_t **a6, uint16x8x2_t **b6, uint16x8x2_t **result_final6)
{
	uint16x8x2_t a_lu_temp[2], b_lu_temp[2];
	uint16x8x2_t result_d0[16], result_d1[16], result_d01[16];

	uint16_t i;

	// KS splitting of 1st 64-coeff multiplication
	for (i = 0; i < 2; i++)
	{
		ADDDOT(a_lu_temp[i], a0[i], a0[2 + i]);
		ADDDOT(b_lu_temp[i], b0[i], b0[2 + i]);
	}
	karatsuba32_fork_avx_new(&a0[0], &b0[0], 0);
	karatsuba32_fork_avx_new(&a0[2], &b0[2], 3);
	karatsuba32_fork_avx_new(&a_lu_temp, &b_lu_temp, 6); // TODO: check this

	// KS splitting of 2nd 64-coeff multiplication
	for (i = 0; i < 2; i++)
	{
		ADDDOT(a_lu_temp[i], a1[i], a1[2 + i]);
		ADDDOT(b_lu_temp[i], b1[i], b1[2 + i]);
	}
	karatsuba32_fork_avx_new(&a1[0], &b1[0], 9);
	karatsuba32_fork_avx_new(&a1[2], &b1[2], 12);
	karatsuba32_fork_avx_new(&a_lu_temp, &b_lu_temp, 15); // Partial: loads only one of the three elements in the bucket

	// Compute 16 school-book multiplications in a batch.
	transpose(&a);
	transpose(&b);
	schoolbook_avx_new();
	transpose(&c_avx[0]);
	transpose(&c_avx[16]);

	// store the partial multiplication result.
	COPYDOT_DOT(c_avx_extra[0], c_avx[15]);
	COPYDOT_DOT(c_avx_extra[1], c_avx[15 + 16]);

	karatsuba32_join_avx_new(&result_d0, 0);
	karatsuba32_join_avx_new(&result_d1, 3);
	karatsuba32_join_avx_new(&result_d01, 6);

	// Final result of 1st 64-coeff multiplication
	join_32coefficient_results(&result_d0, 
							   &result_d1, 
							   &result_d01, 
							   result_final0);

	karatsuba32_join_avx_new(&result_d0, 9);
	karatsuba32_join_avx_new(&result_d1, 12);

	// Fork 2 parts of previous operands
	karatsuba32_fork_avx_partial(&a_lu_temp, &b_lu_temp, 0);

	// Fork multiplication of a2*b2
	for (i = 0; i < 2; i++)
	{
		ADDDOT(a_lu_temp[i], a2[i], a2[2 + i]);
		ADDDOT(b_lu_temp[i], b2[i], b2[2 + i]);
	}
	karatsuba32_fork_avx_new(&a2[0], &b2[0], 2);
	karatsuba32_fork_avx_new(&a2[2], &b2[2], 5);
	karatsuba32_fork_avx_new(&a_lu_temp, &b_lu_temp, 8);

	// Fork multiplication of a3*b3
	for (i = 0; i < 2; i++)
	{
		ADDDOT(a_lu_temp[i], a3[i], a3[2 + i]);
		ADDDOT(b_lu_temp[i], b3[i], b3[2 + i]);
	}
	karatsuba32_fork_avx_new(&a3[0], &b3[0], 11);
	karatsuba32_fork_avx_new(&a3[2], &b3[2], 14); // Partial: loads only two of the three elements in the bucket

	transpose(&a);
	transpose(&b);
	schoolbook_avx_new();

	transpose(&c_avx[0]);
	transpose(&c_avx[16]);

	karatsuba32_join_avx_partial(&result_d01, 0); // Combine results of this computation with previous computation
	// Final result of 2nd 64-coeff multiplication
	join_32coefficient_results( &result_d0, 
								&result_d1, 
								&result_d01, 
								result_final1);

	// store the partial multiplication result. they will be combined after next batch multiplication
	COPYDOT_DOT(c_avx_extra[0], c_avx[14]);
	COPYDOT_DOT(c_avx_extra[1], c_avx[14 + 16]);
	COPYDOT_DOT(c_avx_extra[2], c_avx[15]);
	COPYDOT_DOT(c_avx_extra[3], c_avx[15 + 16]);

	karatsuba32_join_avx_new(&result_d0, 2);
	karatsuba32_join_avx_new(&result_d1, 5);
	karatsuba32_join_avx_new(&result_d01, 8);

	// Final result of 3rd 64-coeff multiplication
	join_32coefficient_results( &result_d0, 
								&result_d1, 
								&result_d01, 
								result_final2);

	// Join d0 of 4th 64-coeff multiplication
	karatsuba32_join_avx_new(&result_d0, 11);

	// Fork 1 part of previous operands
	karatsuba32_fork_avx_partial1(&a3[2], &b3[2], 0);
	karatsuba32_fork_avx_new(&a_lu_temp, &b_lu_temp, 1);

	// Fork multiplication of a4*b4
	for (i = 0; i < 2; i++)
	{
		ADDDOT(a_lu_temp[i], a4[i], a4[2 + i]);
		ADDDOT(b_lu_temp[i], b4[i], b4[2 + i]);
	}
	karatsuba32_fork_avx_new(&a4[0], &b4[0], 4);
	karatsuba32_fork_avx_new(&a4[2], &b4[2], 7);
	karatsuba32_fork_avx_new(&a_lu_temp, &b_lu_temp, 10);

	// Fork multiplication of a5*b5
	for (i = 0; i < 2; i++)
	{
		ADDDOT(a_lu_temp[i], a5[i], a5[2 + i]);
		ADDDOT(b_lu_temp[i], b5[i], b5[2 + i]);
	}
	karatsuba32_fork_avx_new(&a5[0], &b5[0], 13);

	transpose(&a);
	transpose(&b);
	schoolbook_avx_new();

	transpose(&c_avx[0]);
	transpose(&c_avx[16]);

	karatsuba32_join_avx_partial2(&result_d1, 0);
	karatsuba32_join_avx_new(&result_d01, 1);

	// Final result of 4th 64-coeff multiplication
	join_32coefficient_results( &result_d0, 
								&result_d1, 
								&result_d01, 
								result_final3);

	karatsuba32_join_avx_new(&result_d0, 4);
	karatsuba32_join_avx_new(&result_d1, 7);
	karatsuba32_join_avx_new(&result_d01, 10);

	// Final result of 5th 64-coeff multiplication
	join_32coefficient_results(&result_d0, 
							   &result_d1, 
							   &result_d01, 
							   result_final4);

	karatsuba32_join_avx_new(&result_d0, 13);

	// Fork remaining 2 parts of a5*b5
	karatsuba32_fork_avx_new(&a5[2], &b5[2], 0);
	karatsuba32_fork_avx_new(&a_lu_temp, &b_lu_temp, 3);

	// Fork multiplication of a6*b6
	for (i = 0; i < 2; i++)
	{
		ADDDOT(a_lu_temp[i], a6[i], a6[2 + i]);
		ADDDOT(b_lu_temp[i], b6[i], b6[2 + i]);
	}

	karatsuba32_fork_avx_new(&a6[0], &b6[0], 6);
	karatsuba32_fork_avx_new(&a6[2], &b6[2], 9);
	karatsuba32_fork_avx_new(&a_lu_temp, &b_lu_temp, 12);

	transpose(&a);
	transpose(&b);
	schoolbook_avx_new();

	transpose(&c_avx[0]);
	transpose(&c_avx[16]);

	karatsuba32_join_avx_new(&result_d1, 0);
	karatsuba32_join_avx_new(&result_d01, 3);

	// Final result of 6th 64-coeff multiplication
	join_32coefficient_results( &result_d0, 
								&result_d1, 
								&result_d01, 
								result_final5);

	karatsuba32_join_avx_new(&result_d0, 6);
	karatsuba32_join_avx_new(&result_d1, 9);
	karatsuba32_join_avx_new(&result_d01, 12);

	// Final result of 6th 64-coeff multiplication
	join_32coefficient_results(	&result_d0, 
								&result_d1, 
								&result_d01, 
								result_final6);
}

void toom_cook_4way_avx(uint16x8x2_t *a1_avx,
						uint16x8x2_t *b1_avx,
						uint64_t p_mod,
						uint16x8x2_t **res_avx_output)
{
	int16_t AVX_N = SABER_N / 16;
	int16_t i;

	int16_t small_len_avx = AVX_N / 4;

	//-----AVX data declaration-----------------
	uint16x8x2_t res_avx[2 * AVX_N];
	uint16x8x2_t a_avx[small_len_avx], b_avx[small_len_avx];
	uint16x8x2_t th_a_avx[small_len_avx], t_h_a_avx[small_len_avx];
	uint16x8x2_t th_b_avx[small_len_avx], t_h_b_avx[small_len_avx];

	uint16x8x2_t w1_avx[2 * small_len_avx], w2_avx[2 * small_len_avx], w3_avx[2 * small_len_avx], w4_avx[2 * small_len_avx], w5_avx[2 * small_len_avx], w6_avx[2 * small_len_avx], w7_avx[2 * small_len_avx];

	uint16x8x2_t temp1_avx[2 * small_len_avx];

	//--------------------these data are created for place holding---------
	// This is memory or register ? For now, let's compiler optimize it
	uint16x8x2_t a1_ph_avx[small_len_avx], b1_ph_avx[small_len_avx];
	uint16x8x2_t a2_ph_avx[small_len_avx], b2_ph_avx[small_len_avx];
	uint16x8x2_t a3_ph_avx[small_len_avx], b3_ph_avx[small_len_avx];
	uint16x8x2_t a4_ph_avx[small_len_avx], b4_ph_avx[small_len_avx];
	uint16x8x2_t a5_ph_avx[small_len_avx], b5_ph_avx[small_len_avx];
	uint16x8x2_t a6_ph_avx[small_len_avx], b6_ph_avx[small_len_avx];

	//--------------------these data are created for place holding ends---------

	// Add for NEON
	const uint16x8_t int0_avx = vdupq_n_u16(0);

	//-----AVX data declaration ends------------

	p_mod = p_mod * 8;

	//do the partial products

	//-------------------t0--------------------

	//create a(0)*b(0)

	for (i = 0; i < small_len_avx; i++)
	{
		COPYDOT(a1_ph_avx[i], a1_avx[0 + i]);
		COPYDOT(b1_ph_avx[i], b1_avx[0 + i]);
	}

	//-------------------t0 ends------------------

	//-------------------th and t_h. th <-a(1/2)*b(1/2). t_h <- a(-1/2)*b(-1/2) ---------------------
	//create partial sum for th and t_h

	for (i = 0; i < small_len_avx; i++)
	{ //th_x_avx contains 4*x[0]
		SLLDOT_DOT(th_a_avx[i], a1_ph_avx[i], 2);
		SLLDOT_DOT(th_b_avx[i], b1_ph_avx[i], 2);

		//th_x_avx contains 4*x[0]+x[2]
		ADD_DOT(th_a_avx[i], th_a_avx[i], a1_avx[small_len_avx * 2 + i]);
		ADD_DOT(th_b_avx[i], th_b_avx[i], b1_avx[small_len_avx * 2 + i]);

		//th_x_avx contains 8*x[0]+2*x[2]
		SLLDOT_DOT(th_a_avx[i], th_a_avx[i], 1);
		SLLDOT_DOT(th_b_avx[i], th_b_avx[i], 1);

		//t_h_x_avx contains x[1]
		COPYDOT(t_h_a_avx[i], a1_avx[small_len_avx * 1 + i]);
		COPYDOT(t_h_b_avx[i], b1_avx[small_len_avx * 1 + i]);

		//t_h_x_avx contains 4*x[1]
		SLLDOT_DOT(t_h_a_avx[i], t_h_a_avx[i], 2);
		SLLDOT_DOT(t_h_b_avx[i], t_h_b_avx[i], 2);

		//th_x_avx contains 4*x[1]+x[3]
		ADD_DOT(t_h_a_avx[i], t_h_a_avx[i], a1_avx[small_len_avx * 3 + i]);
		ADD_DOT(t_h_b_avx[i], t_h_b_avx[i], b1_avx[small_len_avx * 3 + i]);
	}

	//create th

	for (i = 0; i < small_len_avx; i++)
	{
		ADDDOT_DOT(a2_ph_avx[i], th_a_avx[i], t_h_a_avx[i]);
		ADDDOT_DOT(b2_ph_avx[i], th_b_avx[i], t_h_b_avx[i]);
	}

	//create t_h

	for (i = 0; i < small_len_avx; i++)
	{
		SUBDOT_DOT(a3_ph_avx[i], th_a_avx[i], t_h_a_avx[i]);
		SUBDOT_DOT(b3_ph_avx[i], th_b_avx[i], t_h_b_avx[i]);
	}

	//-------------------t1 and t_1. t1 <-a(1)*b(1). t_1 <- a(-1)*b(-1) ---------------------

	//create partial sum for t_1 and t1

	for (i = 0; i < small_len_avx; i++)
	{ //th_x_avx contains x[2]+x[0]
		ADDDOT(th_a_avx[i], a1_avx[small_len_avx * 2 + i], a1_avx[small_len_avx * 0 + i]);
		ADDDOT(th_b_avx[i], b1_avx[small_len_avx * 2 + i], b1_avx[small_len_avx * 0 + i]);

		//th_x_avx contains x[3]+x[1]
		ADDDOT(t_h_a_avx[i], a1_avx[small_len_avx * 3 + i], a1_avx[small_len_avx * 1 + i]);
		ADDDOT(t_h_b_avx[i], b1_avx[small_len_avx * 3 + i], b1_avx[small_len_avx * 1 + i]);
	}

	//create t1
	for (i = 0; i < small_len_avx; i++)
	{ // x[0]+x[1]+x[2]+x[3]
		ADDDOT_DOT(a4_ph_avx[i], th_a_avx[i], t_h_a_avx[i]);
		ADDDOT_DOT(b4_ph_avx[i], th_b_avx[i], t_h_b_avx[i]);
	}

	//create t_1
	for (i = 0; i < small_len_avx; i++)
	{ //-x[3]+x[2]-x[1]+x[0]
		SUBDOT_DOT(a5_ph_avx[i], th_a_avx[i], t_h_a_avx[i]);
		SUBDOT_DOT(b5_ph_avx[i], th_b_avx[i], t_h_b_avx[i]);
	}

	//------------------t_inf------------------------------
	//create t_inf

	for (i = 0; i < small_len_avx; i++)
	{ //x_avx contains x[3]
		COPYDOT(a6_ph_avx[i], a1_avx[small_len_avx * 3 + i]);
		COPYDOT(b6_ph_avx[i], b1_avx[small_len_avx * 3 + i]);
	}

	//-------------------t_inf ends----------------------

	//-------------------t2-------------------------
	for (i = 0; i < small_len_avx; i++)
	{ // 2*x[3]
		ADD_DOT(a_avx[i], a6_ph_avx[i], a1_avx[small_len_avx * 3 + i]);
		ADD_DOT(b_avx[i], b6_ph_avx[i], b1_avx[small_len_avx * 3 + i]);

		// 2*x[3]+x[2]
		ADD_DOT(a_avx[i], a_avx[i], a1_avx[small_len_avx * 2 + i]);
		ADD_DOT(b_avx[i], b_avx[i], b1_avx[small_len_avx * 2 + i]);

		// 4*x[3]+2*x[2]
		SLLDOT_DOT(a_avx[i], a_avx[i], 1);
		SLLDOT_DOT(b_avx[i], b_avx[i], 1);

		// 4*x[3]+2*x[2]+x[1]
		ADD_DOT(a_avx[i], a_avx[i], a1_avx[small_len_avx * 1 + i]);
		ADD_DOT(b_avx[i], b_avx[i], b1_avx[small_len_avx * 1 + i]);

		// 8*x[3]+4*x[2]+2*x[1]
		SLLDOT_DOT(a_avx[i], a_avx[i], 1);
		SLLDOT_DOT(b_avx[i], b_avx[i], 1);

		// 8*x[3]+8*x[2]+2*x[1]+x[0]
		ADD_DOT(a_avx[i], a_avx[i], a1_avx[small_len_avx * 0 + i]);
		ADD_DOT(b_avx[i], b_avx[i], b1_avx[small_len_avx * 0 + i]);
	}
	// TODO: check this
	batch_64coefficient_multiplications(
		&a1_ph_avx, &b1_ph_avx, &w7_avx,
		&a2_ph_avx, &b2_ph_avx, &w5_avx,
		&a3_ph_avx, &b3_ph_avx, &w6_avx,
		&a4_ph_avx, &b4_ph_avx, &w3_avx,
		&a5_ph_avx, &b5_ph_avx, &w4_avx,
		&a6_ph_avx, &b6_ph_avx, &w1_avx,
		&a_avx, &b_avx, &w2_avx);

	/*	--------------------------------------------
		---------------Solution starts--------------
		--------------------------------------------

	*/

	for (i = 0; i < 2 * small_len_avx; i++)
	{

		ADDDOT_DOT(w2_avx[i], w2_avx[i],w5_avx[i]);//w2 <- w2+w5
		SUBDOT_DOT(w6_avx[i], w6_avx[i], w5_avx[i]); // w6 <- w6-w5
		SUBDOT_DOT(w4_avx[i], w4_avx[i], w3_avx[i]); // w4 <- w4-w3

		SUBDOT_DOT(w5_avx[i], w5_avx[i], w1_avx[i]);	// w5 <- w5-w1
		SLLDOT_DOT(temp1_avx[i], w7_avx[i], 6);			//temp <- 64*w7
		SUBDOT_DOT(w5_avx[i], w5_avx[i], temp1_avx[i]); // w5 <- w5-64*w7

		SRLDOT_DOT(w4_avx[i], w4_avx[i], 1);	  //w4 <- w4/2
		ADDDOT_DOT(w3_avx[i], w3_avx[i],w4_avx[i]);//w3 <- w3+w4

		SLLDOT_DOT(temp1_avx[i], w5_avx[i], 1);		 //temp <- 2*w5
		ADDDOT_DOT(w5_avx[i], w6_avx[i],temp1_avx[i]);//w5 <- 2*w5+w6

		SLLDOT_DOT(temp1_avx[i], w3_avx[i], 6);			   //temp <- 64*w3
		ADDDOT_DOT(temp1_avx[i], w3_avx[i], temp1_avx[i]); //temp <- 65*w3
		SUBDOT_DOT(w2_avx[i], w2_avx[i], temp1_avx[i]);	   // w2 <- w2-65*w3

		SUBDOT_DOT(w3_avx[i], w3_avx[i], w7_avx[i]); // w3 <- w3-w7
		SUBDOT_DOT(w3_avx[i], w3_avx[i], w1_avx[i]); // w3 <- w3-w1

		MULNDOT_DOT(temp1_avx[i], w3_avx[i], 45);		//temp <- 45*w3
		ADDDOT_DOT(w2_avx[i], w2_avx[i], temp1_avx[i]); //w2 <- w2+45*w3

		SLLDOT_DOT(temp1_avx[i], w3_avx[i], 3);			//temp <- 8*w3
		SUBDOT_DOT(w5_avx[i], w5_avx[i], temp1_avx[i]); //w5 <- w5-8*w3

		MULNDOT_DOT(w5_avx[i], w5_avx[i], 43691); //w5 <- w5*1/3
		SRLDOT_DOT(w5_avx[i], w5_avx[i], 3);	 //w5 <- w5*1/8 ---> w5=w5/24

		ADDDOT_DOT(w6_avx[i], w2_avx[i], w6_avx[i]);	//w6 <- w6+w2
		SLLDOT_DOT(temp1_avx[i], w4_avx[i], 4);			//temp <- 16*w4
		ADDDOT_DOT(w2_avx[i], w2_avx[i], temp1_avx[i]); //w2 <- w2+16*w4

		MULNDOT_DOT(w2_avx[i], w2_avx[i], 36409); //w2 <- w2*1/9
		SRLDOT_DOT(w2_avx[i], w2_avx[i], 1);	 //w2 <- w2*1/2 ---> w2=w2/18

		SUBDOT_DOT(w3_avx[i], w3_avx[i], w5_avx[i]); //w3 <- w3-w5

		ADDDOT_DOT(w4_avx[i], w4_avx[i], w2_avx[i]); //w4 <- w4+w2

		// SUBDOT_DOT(w4_avx[i], int0_avx, w4_avx[i]); //w4 <- -(w4+w2)
		w4_avx[i].val[0] = vsubq_u16(int0_avx, w4_avx[i].val[0]);
		w4_avx[i].val[1] = vsubq_u16(int0_avx, w4_avx[i].val[1]);

		MULNDOT_DOT(temp1_avx[i], w2_avx[i], 30);		//temp <- w2*30
		SUBDOT_DOT(w6_avx[i], temp1_avx[i], w6_avx[i]); //w6 <- 30*w2-w6

		MULNDOT_DOT(w6_avx[i], w6_avx[i], 61167); //w6 <- w6*1/15
		SRLDOT_DOT(w6_avx[i], w6_avx[i], 2);	 //w6 <- w6*1/4 ---> w6=w6/60

		SUBDOT_DOT(w2_avx[i], w2_avx[i], w6_avx[i]); //w2 <- w2-w6
	}

	for (i = 0; i < 2 * AVX_N; i++)
	{
		XORDOT_DOT(res_avx[i], res_avx[i], res_avx[i]);
	}

	for (i = 0; i < 2 * small_len_avx; i++)
	{
		ADDDOT_DOT(res_avx[0 * small_len_avx + i], res_avx[0 * small_len_avx + i], w7_avx[i]);
		ADDDOT_DOT(res_avx[1 * small_len_avx + i], res_avx[1 * small_len_avx + i], w6_avx[i]);
		ADDDOT_DOT(res_avx[2 * small_len_avx + i], res_avx[2 * small_len_avx + i], w5_avx[i]);
		ADDDOT_DOT(res_avx[3 * small_len_avx + i], res_avx[3 * small_len_avx + i], w4_avx[i]);
		ADDDOT_DOT(res_avx[4 * small_len_avx + i], res_avx[4 * small_len_avx + i], w3_avx[i]);
		ADDDOT_DOT(res_avx[5 * small_len_avx + i], res_avx[5 * small_len_avx + i], w2_avx[i]);
		ADDDOT_DOT(res_avx[6 * small_len_avx + i], res_avx[6 * small_len_avx + i], w1_avx[i]);
	}

	// Reduction by X^256 + 1
	for (i = 0; i < 16; i++)
	{
		res_avx_output[i]->val[0] = vsubq_u16(res_avx[i].val[0], res_avx[i + 16].val[0]);
		res_avx_output[i]->val[1] = vsubq_u16(res_avx[i].val[1], res_avx[i + 16].val[1]);
	}
}