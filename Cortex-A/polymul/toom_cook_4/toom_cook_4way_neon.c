#include <arm_neon.h>

static inline 
void vload(uint16x8x2_t c, uint16_t a)
{
    // load c <= a 
    c = vld2q_u16(a);
}



/*
 * toom_cook_4way_avx
 * Main function 
 * a1_avx: In memory [256]
 * b1_avx: In memory  [256]
 * res_avx_output: In memory [512]
 */
void toom_cook_4way_avx(uint16_t *a1_avx,
						uint16_t *b1_avx,
						uint16_t p_mod,
						uint16_t *res_avx_output)
{
	const int16_t AVX_N = SABER_N / 16;
	int16_t i;

	const int16_t small_len_avx = AVX_N / 4;

	//-----AVX data declaration-----------------
	uint16x8x2_t res_avx[2 * AVX_N];
	uint16x8x2_t a_avx[small_len_avx], b_avx[small_len_avx];
	uint16x8x2_t th_a_avx[small_len_avx], t_h_a_avx[small_len_avx];
	uint16x8x2_t th_b_avx[small_len_avx], t_h_b_avx[small_len_avx];

    // All in memory
	uint16x8x2_t w1_avx[2 * small_len_avx], w2_avx[2 * small_len_avx], w3_avx[2 * small_len_avx], 
                w4_avx[2 * small_len_avx], w5_avx[2 * small_len_avx], 
                w6_avx[2 * small_len_avx], w7_avx[2 * small_len_avx];

	uint16x8x2_t temp1_avx[2 * small_len_avx];

	//--------------------these data are created for place holding---------
	// This is memory or register ? For now, let's compiler optimize it
	uint16x8x2_t a1_ph_avx[small_len_avx], b1_ph_avx[small_len_avx];
	uint16x8x2_t a2_ph_avx[small_len_avx], b2_ph_avx[small_len_avx];
	uint16x8x2_t a3_ph_avx[small_len_avx], b3_ph_avx[small_len_avx];
	uint16x8x2_t a4_ph_avx[small_len_avx], b4_ph_avx[small_len_avx];
	uint16x8x2_t a5_ph_avx[small_len_avx], b5_ph_avx[small_len_avx];
	uint16x8x2_t a6_ph_avx[small_len_avx], b6_ph_avx[small_len_avx];

    uint16x8x2_t a1_ph_neon, b1_ph_neon;

	//--------------------these data are created for place holding ends---------

	// Add for NEON
	const uint16x8_t int0_avx = vdupq_n_u16(0);

	//-----AVX data declaration ends------------

	p_mod = p_mod * 8;

	//do the partial products

	//-------------------t0--------------------

	//create a(0)*b(0)

    // No need for this 
	// for (i = 0; i < small_len_avx; i++)
	// {
	// 	COPY(a1_ph_avx[i], a1_avx[0 + i]);
	// 	COPY(b1_ph_avx[i], b1_avx[0 + i]);
	// }

	//-------------------t0 ends------------------

	//-------------------th and t_h. th <-a(1/2)*b(1/2). t_h <- a(-1/2)*b(-1/2) ---------------------
	//create partial sum for th and t_h

	for (i = 0; i < small_len_avx; i++)
	{ //th_x_avx contains 4*x[0]
        vload(a1_ph_avx, a1_avx[ (0 + i)*16 ]);
        vload(b1_ph_avx, b1_avx[ (0 + i)*16 ]);

		SLL(th_a_avx[i], a1_ph_avx, 2);
		SLL(th_b_avx[i], b1_ph_avx, 2);

		//th_x_avx contains 4*x[0]+x[2]
		ADD(th_a_avx[i], th_a_avx[i], a1_avx[small_len_avx * 2 + i]);
		ADD(th_b_avx[i], th_b_avx[i], b1_avx[small_len_avx * 2 + i]);

		//th_x_avx contains 8*x[0]+2*x[2]
		SLL(th_a_avx[i], th_a_avx[i], 1);
		SLL(th_b_avx[i], th_b_avx[i], 1);

		//t_h_x_avx contains x[1]
		COPY(t_h_a_avx[i], a1_avx[small_len_avx * 1 + i]);
		COPY(t_h_b_avx[i], b1_avx[small_len_avx * 1 + i]);

		//t_h_x_avx contains 4*x[1]
		SLL(t_h_a_avx[i], t_h_a_avx[i], 2);
		SLL(t_h_b_avx[i], t_h_b_avx[i], 2);

		//th_x_avx contains 4*x[1]+x[3]
		ADD(t_h_a_avx[i], t_h_a_avx[i], a1_avx[small_len_avx * 3 + i]);
		ADD(t_h_b_avx[i], t_h_b_avx[i], b1_avx[small_len_avx * 3 + i]);
	

	//create th

	
		ADD(a2_ph_avx[i], th_a_avx[i], t_h_a_avx[i]);
		ADD(b2_ph_avx[i], th_b_avx[i], t_h_b_avx[i]);
	

	//create t_h

	
		SUB(a3_ph_avx[i], th_a_avx[i], t_h_a_avx[i]);
		SUB(b3_ph_avx[i], th_b_avx[i], t_h_b_avx[i]);
	

	//-------------------t1 and t_1. t1 <-a(1)*b(1). t_1 <- a(-1)*b(-1) ---------------------

	//create partial sum for t_1 and t1

	
	 //th_x_avx contains x[2]+x[0]
		ADD(th_a_avx[i], a1_avx[small_len_avx * 2 + i], a1_avx[small_len_avx * 0 + i]);
		ADD(th_b_avx[i], b1_avx[small_len_avx * 2 + i], b1_avx[small_len_avx * 0 + i]);

		//th_x_avx contains x[3]+x[1]
		ADD(t_h_a_avx[i], a1_avx[small_len_avx * 3 + i], a1_avx[small_len_avx * 1 + i]);
		ADD(t_h_b_avx[i], b1_avx[small_len_avx * 3 + i], b1_avx[small_len_avx * 1 + i]);
	

	//create t1
	 // x[0]+x[1]+x[2]+x[3]
		ADD(a4_ph_avx[i], th_a_avx[i], t_h_a_avx[i]);
		ADD(b4_ph_avx[i], th_b_avx[i], t_h_b_avx[i]);
	

	//create t_1
	 //-x[3]+x[2]-x[1]+x[0]
		SUB(a5_ph_avx[i], th_a_avx[i], t_h_a_avx[i]);
		SUB(b5_ph_avx[i], th_b_avx[i], t_h_b_avx[i]);
	

	//------------------t_inf------------------------------
	//create t_inf

	 //x_avx contains x[3]
		COPY(a6_ph_avx[i], a1_avx[small_len_avx * 3 + i]);
		COPY(b6_ph_avx[i], b1_avx[small_len_avx * 3 + i]);
	

	//-------------------t_inf ends----------------------

	//-------------------t2-------------------------
	 // 2*x[3]
		ADD(a_avx[i], a6_ph_avx[i], a1_avx[small_len_avx * 3 + i]);
		ADD(b_avx[i], b6_ph_avx[i], b1_avx[small_len_avx * 3 + i]);

		// 2*x[3]+x[2]
		ADD(a_avx[i], a_avx[i], a1_avx[small_len_avx * 2 + i]);
		ADD(b_avx[i], b_avx[i], b1_avx[small_len_avx * 2 + i]);

		// 4*x[3]+2*x[2]
		SLL(a_avx[i], a_avx[i], 1);
		SLL(b_avx[i], b_avx[i], 1);

		// 4*x[3]+2*x[2]+x[1]
		ADD(a_avx[i], a_avx[i], a1_avx[small_len_avx * 1 + i]);
		ADD(b_avx[i], b_avx[i], b1_avx[small_len_avx * 1 + i]);

		// 8*x[3]+4*x[2]+2*x[1]
		SLL(a_avx[i], a_avx[i], 1);
		SLL(b_avx[i], b_avx[i], 1);

		// 8*x[3]+8*x[2]+2*x[1]+x[0]
		ADD(a_avx[i], a_avx[i], a1_avx[small_len_avx * 0 + i]);
		ADD(b_avx[i], b_avx[i], b1_avx[small_len_avx * 0 + i]);
	}
	// TODO: check this
	batch_64coefficient_multiplications(
        // a1_avx[0-63], b1_avx[0-63], 
		a1_ph_avx, b1_ph_avx, w7_avx,
        // 
		a2_ph_avx, b2_ph_avx, w5_avx,
		a3_ph_avx, b3_ph_avx, w6_avx,
		a4_ph_avx, b4_ph_avx, w3_avx,
		a5_ph_avx, b5_ph_avx, w4_avx,
		a6_ph_avx, b6_ph_avx, w1_avx,
		a_avx, b_avx, w2_avx);

	/*	--------------------------------------------
		---------------Solution starts--------------
		--------------------------------------------

	*/

	for (i = 0; i < 2 * small_len_avx; i++)
	{

		ADD(w2_avx[i], w2_avx[i],w5_avx[i]);//w2 <- w2+w5
		SUB(w6_avx[i], w6_avx[i], w5_avx[i]); // w6 <- w6-w5
		SUB(w4_avx[i], w4_avx[i], w3_avx[i]); // w4 <- w4-w3

		SUB(w5_avx[i], w5_avx[i], w1_avx[i]);	// w5 <- w5-w1
		SLL(temp1_avx[i], w7_avx[i], 6);			//temp <- 64*w7
		SUB(w5_avx[i], w5_avx[i], temp1_avx[i]); // w5 <- w5-64*w7

		SRL(w4_avx[i], w4_avx[i], 1);	  //w4 <- w4/2
		ADD(w3_avx[i], w3_avx[i],w4_avx[i]);//w3 <- w3+w4

		SLL(temp1_avx[i], w5_avx[i], 1);		 //temp <- 2*w5
		ADD(w5_avx[i], w6_avx[i],temp1_avx[i]);//w5 <- 2*w5+w6

		SLL(temp1_avx[i], w3_avx[i], 6);			   //temp <- 64*w3
		ADD(temp1_avx[i], w3_avx[i], temp1_avx[i]); //temp <- 65*w3
		SUB(w2_avx[i], w2_avx[i], temp1_avx[i]);	   // w2 <- w2-65*w3

		SUB(w3_avx[i], w3_avx[i], w7_avx[i]); // w3 <- w3-w7
		SUB(w3_avx[i], w3_avx[i], w1_avx[i]); // w3 <- w3-w1

		MULN(temp1_avx[i], w3_avx[i], 45);		//temp <- 45*w3
		ADD(w2_avx[i], w2_avx[i], temp1_avx[i]); //w2 <- w2+45*w3

		SLL(temp1_avx[i], w3_avx[i], 3);			//temp <- 8*w3
		SUB(w5_avx[i], w5_avx[i], temp1_avx[i]); //w5 <- w5-8*w3

		MULN(w5_avx[i], w5_avx[i], 43691); //w5 <- w5*1/3
		SRL(w5_avx[i], w5_avx[i], 3);	 //w5 <- w5*1/8 --. w5=w5/24

		ADD(w6_avx[i], w2_avx[i], w6_avx[i]);	//w6 <- w6+w2
		SLL(temp1_avx[i], w4_avx[i], 4);			//temp <- 16*w4
		ADD(w2_avx[i], w2_avx[i], temp1_avx[i]); //w2 <- w2+16*w4

		MULN(w2_avx[i], w2_avx[i], 36409); //w2 <- w2*1/9
		SRL(w2_avx[i], w2_avx[i], 1);	 //w2 <- w2*1/2 --. w2=w2/18

		SUB(w3_avx[i], w3_avx[i], w5_avx[i]); //w3 <- w3-w5

		ADD(w4_avx[i], w4_avx[i], w2_avx[i]); //w4 <- w4+w2

		// SUB(w4_avx[i], int0_avx, w4_avx[i]); //w4 <- -(w4+w2)
		w4_avx[i].val[0] = vsubq_u16(int0_avx, w4_avx[i].val[0]);
		w4_avx[i].val[1] = vsubq_u16(int0_avx, w4_avx[i].val[1]);

		MULN(temp1_avx[i], w2_avx[i], 30);		//temp <- w2*30
		SUB(w6_avx[i], temp1_avx[i], w6_avx[i]); //w6 <- 30*w2-w6

		MULN(w6_avx[i], w6_avx[i], 61167); //w6 <- w6*1/15
		SRL(w6_avx[i], w6_avx[i], 2);	 //w6 <- w6*1/4 --. w6=w6/60

		SUB(w2_avx[i], w2_avx[i], w6_avx[i]); //w2 <- w2-w6
	}

	for (i = 0; i < 2 * AVX_N; i++)
	{
		XOR(res_avx[i], res_avx[i], res_avx[i]);
	}

	for (i = 0; i < 2 * small_len_avx; i++)
	{
		ADD(res_avx[0 * small_len_avx + i], res_avx[0 * small_len_avx + i], w7_avx[i]);
		ADD(res_avx[1 * small_len_avx + i], res_avx[1 * small_len_avx + i], w6_avx[i]);
		ADD(res_avx[2 * small_len_avx + i], res_avx[2 * small_len_avx + i], w5_avx[i]);
		ADD(res_avx[3 * small_len_avx + i], res_avx[3 * small_len_avx + i], w4_avx[i]);
		ADD(res_avx[4 * small_len_avx + i], res_avx[4 * small_len_avx + i], w3_avx[i]);
		ADD(res_avx[5 * small_len_avx + i], res_avx[5 * small_len_avx + i], w2_avx[i]);
		ADD(res_avx[6 * small_len_avx + i], res_avx[6 * small_len_avx + i], w1_avx[i]);
	}

	// Reduction by X^256 + 1
	for (i = 0; i < 16; i++)
	{
		res_avx_output[i].val[0] = vsubq_u16(res_avx[i].val[0], res_avx[i + 16].val[0]);
		res_avx_output[i].val[1] = vsubq_u16(res_avx[i].val[1], res_avx[i + 16].val[1]);
	}
}