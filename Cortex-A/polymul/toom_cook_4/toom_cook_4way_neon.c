#include <arm_neon.h>

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
	
    // All in memory
	uint16_t    w1_avx[2 * 16 * small_len_avx], 
                w2_avx[2 * 16 * small_len_avx], 
                w3_avx[2 * 16 * small_len_avx], 
                w4_avx[2 * 16 * small_len_avx], 
                w5_avx[2 * 16 * small_len_avx], 
                w6_avx[2 * 16 * small_len_avx], 
                w7_avx[2 * 16 * small_len_avx];

	uint16x8x2_t temp1_avx[2 * small_len_avx];

	//--------------------these data are created for place holding---------
	// This is memory or register ? For now, let's compiler optimize it
	uint16_t a1_ph_avx[small_len_avx*16], b1_ph_avx[small_len_avx*16];
	uint16_t a2_ph_avx[small_len_avx*16], b2_ph_avx[small_len_avx*16];
	uint16_t a3_ph_avx[small_len_avx*16], b3_ph_avx[small_len_avx*16];
	uint16_t a4_ph_avx[small_len_avx*16], b4_ph_avx[small_len_avx*16];
	uint16_t a5_ph_avx[small_len_avx*16], b5_ph_avx[small_len_avx*16];
	uint16_t a6_ph_avx[small_len_avx*16], b6_ph_avx[small_len_avx*16];
	uint16_t a_avx[small_len_avx*16], b_avx[small_len_avx*16];

    uint16x8x2_t a1_tmp, b1_tmp;
    uint16x8x2_t a2_tmp, b2_tmp;
    uint16x8x2_t th_a_neon, th_b_neon;
    uint16x8x2_t t_h_a_neon, t_h_b_neon;

	//--------------------these data are created for place holding ends---------

	// Add for NEON
	const uint16x8_t int0_avx = vdupq_n_u16(0);

	//-----AVX data declaration ends------------

	p_mod = p_mod * 8;

	//do the partial products


	//-------------------th and t_h. th <-a(1/2)*b(1/2). t_h <- a(-1/2)*b(-1/2) ---------------------
	//create partial sum for th and t_h

	for (i = 0; i < small_len_avx; i++)
	{ 
	    //create a(0)*b(0)
	    //-------------------t0--------------------
        vload(a1_tmp, &a1_avx[ (0 + i)*16 ]);
        vload(b1_tmp, &b1_avx[ (0 + i)*16 ]);

        vstore(&a1_ph_avx[i*16], a1_tmp);
        vstore(&b1_ph_avx[i*16], b1_tmp);    
	    //-------------------t0 ends------------------

        //th_x_avx contains 4*x[0]
		vsl(th_a_neon, a1_tmp, 2);
		vsl(th_b_neon, b1_tmp, 2);

        vload(a1_tmp, &a1_avx[(small_len_avx * 2 + i)*16 ] );
        vload(b1_tmp, &b1_avx[(small_len_avx * 2 + i)*16 ] );

		//th_x_avx contains 4*x[0]+x[2]
		vadd(th_a_neon, th_a_neon,  a1_tmp);
		vadd(th_b_neon, th_b_neon,  b1_tmp);

		//th_x_avx contains 8*x[0]+2*x[2]
		vsl(th_a_neon, th_a_neon, 1);
		vsl(th_b_neon, th_b_neon, 1);

		//t_h_x_avx contains x[1]
        vload(t_h_a_neon, &a1_avx[(small_len_avx * 1 + i)*16 ] )
        vload(t_h_b_neon, &b1_avx[(small_len_avx * 1 + i)*16 ] )

		//t_h_x_avx contains 4*x[1]
		vsl(t_h_a_neon, t_h_a_neon, 2);
		vsl(t_h_b_neon, t_h_b_neon, 2);

		//th_x_avx contains 4*x[1]+x[3]
        vload(a1_tmp, &a1_avx[ (small_len_avx * 3 + i)*16 ] );
        vload(b1_tmp, &b1_avx[ (small_len_avx * 3 + i)*16 ] );
		
        vadd(t_h_a_neon, t_h_a_neon, a1_tmp);
		vadd(t_h_b_neon, t_h_b_neon, b1_tmp);
	

    	//create th
        vadd(a1_tmp, th_a_neon, t_h_a_neon);
		vadd(b1_tmp, th_b_neon, t_h_b_neon);

        vstore(&a2_ph_avx[i*16], a1_tmp);
        vstore(&b2_ph_avx[i*16], b1_tmp)

	    //create t_h

        vsub(a1_tmp, th_a_neon, t_h_a_neon);
		vsub(b1_tmp, th_b_neon, t_h_b_neon);

        vstore(&a3_ph_avx[i*16], a1_tmp);
        vstore(&a3_ph_avx[i*16], b1_tmp);

	//-------------------t1 and t_1. t1 <-a(1)*b(1). t_1 <- a(-1)*b(-1) ---------------------

	    //create partial sum for t_1 and t1

	    //th_x_avx contains x[2]+x[0]
        vload(a1_tmp, &a1_avx[(small_len_avx * 2 + i)*16] );
        vload(b1_tmp, &b1_avx[(small_len_avx * 2 + i)*16] );
        vload(a2_tmp, &a1_avx[(small_len_avx * 0 + i)*16] );
        vload(b2_tmp, &b1_avx[(small_len_avx * 0 + i)*16] );

		vadd(th_a_neon, a1_tmp, a2_tmp);
		vadd(th_b_neon, b1_tmp, b2_tmp);

		//th_x_avx contains x[3]+x[1]
        vload(a1_tmp, &a1_avx[(small_len_avx * 3 + i)*16] );
        vload(b1_tmp, &b1_avx[(small_len_avx * 3 + i)*16] );
        vload(a2_tmp, &a1_avx[(small_len_avx * 1 + i)*16] );
        vload(b2_tmp, &b1_avx[(small_len_avx * 1 + i)*16] );

		vadd(t_h_a_neon, a1_tmp, a2_tmp);
		vadd(t_h_b_neon, b1_tmp, b2_tmp);
	

	    //create t1
	    // x[0]+x[1]+x[2]+x[3]
        vadd(a1_tmp, th_a_neon, t_h_a_neon);
		vadd(b1_tmp, th_b_neon, t_h_b_neon);

        vstore(&a4_ph_avx[i*16], a1_tmp);
        vstore(&b4_ph_avx[i*16], b1_tmp)
	

	    //create t_1
	    //-x[3]+x[2]-x[1]+x[0]
        vsub(a2_tmp, th_a_neon, t_h_a_neon);
		vsub(b2_tmp, th_b_neon, t_h_b_neon);

        vstore(&a5_ph_avx[i*16], a2_tmp);
        vstore(&b5_ph_avx[i*16], b2_tmp);
	

	//------------------t_inf------------------------------
	//create t_inf

	 //x_avx contains x[3]
        vload(a1_tmp, &a1_avx[(small_len_avx * 3 + i)*16] ); 
        vload(b1_tmp, &b1_avx[(small_len_avx * 3 + i)*16] ); 

        vstore(&a6_ph_avx[i*16], a1_tmp);
        vstore(&b6_ph_avx[i*16], b1_tmp);

	//-------------------t_inf ends----------------------

	//-------------------t2-------------------------
        // 2*x[3]
        vload(a2_tmp, &a1_avx[(small_len_avx * 3 + i)*16] );
        vload(b2_tmp, &b1_avx[(small_len_avx * 3 + i)*16] );

        vadd(a1_tmp, a1_tmp, a2_tmp);
		vadd(b1_tmp, b1_tmp, b2_tmp);

		// 2*x[3]+x[2]
        vload(a2_tmp, &a1_avx[(small_len_avx * 2 + i)*16] );
        vload(b2_tmp, &b1_avx[(small_len_avx * 2 + i)*16] );

		vadd(a1_tmp, a1_tmp, a2_tmp);
		vadd(b1_tmp, b1_tmp, b2_tmp);

		// 4*x[3]+2*x[2]
		vsl(a1_tmp, a1_tmp, 1);
		vsl(b1_tmp, b1_tmp, 1);

        vload(a2_tmp, &a1_avx[(small_len_avx * 1 + i)*16] );
        vload(b2_tmp, &b1_avx[(small_len_avx * 1 + i)*16] );

		// 4*x[3]+2*x[2]+x[1]
		vadd(a1_tmp, a1_tmp, a2_tmp);
		vadd(b1_tmp, b1_tmp, b2_tmp);

		// 8*x[3]+4*x[2]+2*x[1]
		vsl(a1_tmp, a1_tmp, 1);
		vsl(b1_tmp, b1_tmp, 1);

		// 8*x[3]+8*x[2]+2*x[1]+x[0]
        vload(a2_tmp, &a1_avx[(small_len_avx * 0 + i)*16] );
        vload(b2_tmp, &b1_avx[(small_len_avx * 0 + i)*16] );


		vadd(a1_tmp, a1_tmp, a2_tmp);
		vadd(b1_tmp, b1_tmp, b2_tmp);

        vstore(&a_avx[i*16], a1_tmp);
        vstore(&b_avx[i*16], b1_tmp);
	}

	batch_64coefficient_multiplications(
		a1_ph_avx, b1_ph_avx, w7_avx,
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