#include "pack_unpack.h"
//#include "randombytes.h"
#include "rng.h"
#include "cbd.h"
#include "SABER_params.h"
#include "polymul/toom_cook_4/asimd_toom-cook_4way.c"
#include "fips202.h"

#if __aarch64__
#include <libkeccak.so.headers/SimpleFIPS202.h>
#else
#error "Install SIMD Keccak"
#endif

#define h1 4 //2^(EQ-EP-1)

#define h2 ( (1<<(SABER_EP-2)) - (1<<(SABER_EP-SABER_ET-1)) + (1<<(SABER_EQ-SABER_EP-1)) )
// . | . . 
#define XORDOT_DOT(c, a, b)                   \
	c.val[0] = veorq_u16(a.val[0], b.val[0]); \
	c.val[1] = veorq_u16(a.val[1], b.val[1])
// . | . . 
#define ADDDOT_DOT(c, a, b)                   \
	c.val[0] = vaddq_u16(a.val[0], b.val[0]); \
	c.val[1] = vaddq_u16(a.val[1], b.val[1])
//  . | .
#define ADDDOT_DOTVAL(c, a, value)                   \
	c.val[0] = vaddq_u16(a.val[0], value); \
	c.val[1] = vaddq_u16(a.val[1], value)
// . | . . 
#define ANDDOT_DOT(c, a, b)\
    c.val[0] = vandq_u16(a.val[0], b.val[0]); \
	c.val[1] = vandq_u16(a.val[1], b.val[1])
//  . | . 
#define ANDDOT_DOTVAL(c, a, value)\
    c.val[0] = vandq_u16(a.val[0], value); \
	c.val[1] = vandq_u16(a.val[1], value)

// . | . 
#define SRLDOT_DOT(c, a, value)              \
	c.val[0] = vshrq_n_u16(a.val[0], value); \
	c.val[1] = vshrq_n_u16(c.val[1], value)
// . | . 
#define SLLDOT_DOT(c, a, value)              \
	c.val[0] = vshlq_n_u16(a.val[0], value); \
	c.val[1] = vshlq_n_u16(c.val[1], value)
// . | . 
#define SUBDOT_DOT(c, a, b)                   \
	c.val[0] = vsubq_u16(a.val[0], b.val[0]); \
	c.val[1] = vsubq_u16(a.val[1], b.val[1])






void POL2MSG(uint16_t *message_dec_unpacked, unsigned char *message_dec){

	int32_t i,j;

	for(j=0; j<SABER_KEYBYTES; j++)
	{
		message_dec[j] = 0;
		for(i=0; i<8; i++)
		message_dec[j] = message_dec[j] | (message_dec_unpacked[j*8 + i] <<i);
	} 

}

/*--------------------------------------------------------------------------------------
	This routine loads the constant values for Toom-Cook multiplication 
----------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------
	This routine generates a=[Matrix K x K] of 256-coefficient polynomials 
-------------------------------------------------------------------------------------*/



void BS2POLq(const unsigned char *bytes, uint16_t data[SABER_N]){
	
	uint32_t j;
	uint32_t offset_data=0,offset_byte=0;	
	
	offset_byte=0;

		for(j=0;j<SABER_N/8;j++){
			offset_byte=13*j;
			offset_data=8*j;
			data[offset_data + 0]= ( bytes[ offset_byte + 0 ] & (0xff)) | ((bytes[offset_byte + 1] & 0x1f)<<8);
			data[offset_data + 1]= ( bytes[ offset_byte + 1 ]>>5 & (0x07)) | ((bytes[offset_byte + 2] & 0xff)<<3) | ((bytes[offset_byte + 3] & 0x03)<<11);
			data[offset_data + 2]= ( bytes[ offset_byte + 3 ]>>2 & (0x3f)) | ((bytes[offset_byte + 4] & 0x7f)<<6);
			data[offset_data + 3]= ( bytes[ offset_byte + 4 ]>>7 & (0x01)) | ((bytes[offset_byte + 5] & 0xff)<<1) | ((bytes[offset_byte + 6] & 0x0f)<<9);
			data[offset_data + 4]= ( bytes[ offset_byte + 6 ]>>4 & (0x0f)) | ((bytes[offset_byte + 7] & 0xff)<<4) | ((bytes[offset_byte + 8] & 0x01)<<12);
			data[offset_data + 5]= ( bytes[ offset_byte + 8]>>1 & (0x7f)) | ((bytes[offset_byte + 9] & 0x3f)<<7);
			data[offset_data + 6]= ( bytes[ offset_byte + 9]>>6 & (0x03)) | ((bytes[offset_byte + 10] & 0xff)<<2) | ((bytes[offset_byte + 11] & 0x07)<<10);
			data[offset_data + 7]= ( bytes[ offset_byte + 11]>>3 & (0x1f)) | ((bytes[offset_byte + 12] & 0xff)<<5);
		}


}


void GenMatrix(polyvec *a, const unsigned char *seed) 
{
  unsigned int one_vector=13*SABER_N/8;
  unsigned int byte_bank_length=SABER_K*SABER_K*one_vector;
  unsigned char buf[byte_bank_length];

  uint16_t temp_ar[SABER_N];

  int i,j,k;
  uint16_t mod = (SABER_Q-1);

  shake128(buf,byte_bank_length,seed,SABER_SEEDBYTES);
  
  for(i=0;i<SABER_K;i++)
  {
    for(j=0;j<SABER_K;j++)
    {
	BS2POLq(buf+(i*SABER_K+j)*one_vector,temp_ar);
	for(k=0;k<SABER_N;k++){
		a[i].vec[j].coeffs[k] = (temp_ar[k])& mod ;
	}
    }
  }
}

void GenSecret(uint16_t r[SABER_K][SABER_N],const unsigned char *seed){


		uint32_t i;

		int32_t buf_size= SABER_MU*SABER_N*SABER_K/8;

		uint8_t buf[buf_size];

		shake128(buf, buf_size, seed,SABER_NOISESEEDBYTES);

		for(i=0;i<SABER_K;i++)
		{
			cbd(r[i],buf+i*SABER_MU*SABER_N/8);
		}
}


void indcpa_kem_keypair(unsigned char *pk, unsigned char *sk)
{
 
  polyvec a[SABER_K];

  uint16_t skpv1[SABER_K][SABER_N];

  
 
  unsigned char seed[SABER_SEEDBYTES];
  unsigned char noiseseed[SABER_COINBYTES];
  int32_t i,j,k;


//--------------AVX declaration------------------
	
  uint16x8x2_t sk_avx[SABER_K][SABER_N/16];
  uint16x8x2_t res_avx[SABER_K][SABER_N/16];
  uint16x8x2_t a_avx[SABER_K][SABER_K][SABER_N/16];
  uint16x8x2_t acc[2*SABER_N/16];

  const uint16x8_t mod   =vdupq_n_u16(SABER_Q-1);
  const uint16x8_t H1_avx=vdupq_n_u16(h1);


//--------------AVX declaration ends------------------

  randombytes(seed, SABER_SEEDBYTES);
 


  shake128(seed, SABER_SEEDBYTES, seed, SABER_SEEDBYTES); // for not revealing system RNG state
  randombytes(noiseseed, SABER_COINBYTES);

  
  GenMatrix(a, seed); //sample matrix A
 
 
  GenSecret(skpv1,noiseseed);
 

 // Load sk into avx vectors		
 for(i=0;i<SABER_K;i++)
 {
	for(j=0; j<SABER_N/16; j++){
            sk_avx[i][j] = vld2q_u16 ( (&skpv1[i][j*16]));
	}

  }

  // Load a into avx vectors	
  for(i=0;i<SABER_K;i++){ 
	  for(j=0;j<SABER_K;j++){
		  for(k=0;k<SABER_N/16;k++){
			a_avx[i][j][k]=vld2q_u16 ( (&a[i].vec[j].coeffs[k*16]));
		  }
	  }
  }	



  //------------------------do the matrix vector multiplication and rounding------------


	for(i=0;i<SABER_K;i++){
		for(j=0;j<SABER_N/16;j++){
			XORDOT_DOT(res_avx[i][j], res_avx[i][j],res_avx[i][j]);
		}
	}

	// Matrix-vector multiplication; Matrix in transposed order
	for(i=0;i<SABER_K;i++){
		for(j=0;j<SABER_K;j++){
			toom_cook_4way_avx(a_avx[j][i], sk_avx[j], SABER_Q, acc);

			for(k=0;k<SABER_N/16;k++){
				ADDDOT_DOT( res_avx[i][k], res_avx[i][k],acc[k]);
				ANDDOT_DOTVAL( res_avx[i][k], res_avx[i][k],mod); //reduction mod p
				XORDOT_DOT( acc[k], acc[k],acc[k]); //clear the accumulator
			}
		}
	}

	
	// Now truncation
	for(i=0;i<SABER_K;i++){ //shift right EQ-EP bits
		for(j=0;j<SABER_N/16;j++){
			ADDDOT_DOTVAL( res_avx[i][j], res_avx[i][j], H1_avx);
			SRLDOT_DOT( res_avx[i][j], res_avx[i][j], (SABER_EQ-SABER_EP) );
			ANDDOT_DOTVAL( res_avx[i][j], res_avx[i][j], mod);			
		}
	}

	//------------------Pack sk into byte string-------
		
	POLVEC2BS(sk,skpv1,SABER_Q);

	//------------------Pack pk into byte string-------
	
	for(i=0;i<SABER_K;i++){ // reuses skpv1[] for unpacking avx of public-key
		  for(j=0;j<SABER_N/16;j++){
		  	vst2q_u16 ((uint16_t *) (skpv1[i]+j*16), res_avx[i][j]);
		  }
	  }
	POLVEC2BS(pk,skpv1,SABER_P); // load the public-key into pk byte string 	


	for(i=0;i<SABER_SEEDBYTES;i++){ // now load the seedbytes in PK. Easy since seed bytes are kept in byte format.
		pk[SABER_POLYVECCOMPRESSEDBYTES + i]=seed[i]; 
	}

}



void indcpa_kem_enc(unsigned char *message_received, 
					unsigned char *noiseseed, 
					const unsigned char *pk, 
					unsigned char *ciphertext)
{ 

	uint32_t i,j,k;
	polyvec a[SABER_K];		// skpv;
	unsigned char seed[SABER_SEEDBYTES];
	uint16_t pkcl[SABER_K][SABER_N]; 	//public key of received by the client


	uint16_t skpv1[SABER_K][SABER_N];
	uint16_t temp[SABER_K][SABER_N];
	uint16_t message[SABER_KEYBYTES*8];

	unsigned char msk_c[SABER_SCALEBYTES_KEM];
	//--------------AVX declaration------------------
	
	  uint16x8x2_t sk_avx[SABER_K][SABER_N/16];
	  uint16x8x2_t res_avx[SABER_K][SABER_N/16];
	  uint16x8x2_t vprime_avx[SABER_N/16];
	  uint16x8x2_t a_avx[SABER_K][SABER_K][SABER_N/16];
	  uint16x8x2_t acc[2*SABER_N/16];

	  uint16x8x2_t pkcl_avx[SABER_K][SABER_N/16];

      uint16x8x2_t message_avx[SABER_N/16];
      
	  const uint16x8_t mod   =vdupq_n_u16 (SABER_Q-1);
	  const uint16x8_t mod_p =vdupq_n_u16 (SABER_P-1);
	  const uint16x8_t H1_avx=vdupq_n_u16(h1);
 
	//--------------AVX declaration ends------------------
	
      
	for(i=0;i<SABER_SEEDBYTES;i++){ // Load the seedbytes in the client seed from PK.
		seed[i]=pk[ SABER_POLYVECCOMPRESSEDBYTES + i]; 
	}


	GenMatrix(a, seed);				

	GenSecret(skpv1,noiseseed);

	// ----------- Load skpv1 into avx vectors ---------- 
	for(i=0;i<SABER_K;i++){ 
		for(j=0; j<SABER_N/16; j++){
		    sk_avx[i][j] = vld2q_u16 ((&skpv1[i][j*16]));
		}
  	}

	// ----------- Load skpv1 into avx vectors ---------- 
	  for(i=0;i<SABER_K;i++){ 
		  for(j=0;j<SABER_K;j++){
			  for(k=0;k<SABER_N/16;k++){
				a_avx[i][j][k]=vld2q_u16 ( (&a[i].vec[j].coeffs[k*16]));
			  }
		  }
 	 }


	//-----------------matrix-vector multiplication and rounding

	// Initialize res_avx[][] to 0	
	for(i=0;i<SABER_K;i++){
		for(j=0;j<SABER_N/16;j++){
			XORDOT_DOT(res_avx[i][j], res_avx[i][j],res_avx[i][j]);
		}
	}	

	// Matrix-vector multiplication; 
	for(i=0;i<SABER_K;i++){
		for(j=0;j<SABER_K;j++){

			toom_cook_4way_avx(a_avx[i][j], sk_avx[j], SABER_Q, acc);

			for(k=0;k<SABER_N/16;k++){
				ADDDOT_DOT(res_avx[i][k], res_avx[i][k],acc[k]);
				ANDDOT_DOTVAL(res_avx[i][k], res_avx[i][k],mod); //reduction
				XORDOT_DOT(acc[k], acc[k],acc[k]); //clear the accumulator
			}
			
		}
	}
	
	// Now truncation

	for(i=0;i<SABER_K;i++){ //shift right EQ-EP bits
		for(j=0;j<SABER_N/16;j++){
			ADDDOT_DOTVAL( res_avx[i][j], res_avx[i][j], H1_avx);
			SRLDOT_DOT( res_avx[i][j], res_avx[i][j], (SABER_EQ-SABER_EP) );
			ANDDOT_DOTVAL( res_avx[i][j], res_avx[i][j], mod);			

		}
	}


	//-----this result should be put in b_prime for later use in server.
	for(i=0;i<SABER_K;i++){ // first store in 16 bit arrays
		  for(j=0;j<SABER_N/16;j++){
			vst2q_u16  ((uint16_t *)(temp[i]+j*16), res_avx[i][j]);
		  }
	  }
	
	POLVEC2BS(ciphertext,temp, SABER_P); // Pack b_prime into ciphertext byte string

//**************client matrix-vector multiplication ends******************//

	//------now calculate the v'

	//-------unpack the public_key
	BS2POLVEC(pk, pkcl, SABER_P);

	for(i=0;i<SABER_K;i++){
		for(j=0; j<SABER_N/16; j++){
		    pkcl_avx[i][j] = vld2q_u16( (&pkcl[i][j*16]));
		}
	}

	// InnerProduct
	for(k=0;k<SABER_N/16;k++){
		XORDOT_DOT( vprime_avx[k], vprime_avx[k],vprime_avx[k]);
	}

	// vector-vector scalar multiplication with mod p
	
	
	for(j=0;j<SABER_K;j++){
		toom_cook_4way_avx(pkcl_avx[j], sk_avx[j], SABER_P, acc);

			for(k=0;k<SABER_N/16;k++){
				ADDDOT_DOT(vprime_avx[k] , vprime_avx[k],acc[k]);
				ANDDOT_DOTVAL( vprime_avx[k], vprime_avx[k],mod_p); //reduction
				XORDOT_DOT( acc[k], acc[k],acc[k]); //clear the accumulator
			}
	}
	
	

	// Computation of v'+h1 
	for(i=0;i<SABER_N/16;i++){//adding h1
 		ADDDOT_DOTVAL(vprime_avx[i], vprime_avx[i], H1_avx);
	}

	// unpack message_received;
	for(j=0; j<SABER_KEYBYTES; j++)
	{
		for(i=0; i<8; i++)
		{
			message[8*j+i] = ((message_received[j]>>i) & 0x01);
		}
	}
	// message encoding
	for(i=0; i<SABER_N/16; i++)
	{
		message_avx[i] = vld2q_u16 ( (&message[i*16]));
		SLLDOT_DOT (message_avx[i] , message_avx[i], (SABER_EP-1) );
	}	

	// SHIFTRIGHT(v'+h1-m mod p, EP-ET)
	for(k=0;k<SABER_N/16;k++)
	{
		SUBDOT_DOT( vprime_avx[k], vprime_avx[k], message_avx[k]);
		ANDDOT_DOTVAL( vprime_avx[k], vprime_avx[k], mod_p);
		SRLDOT_DOT( vprime_avx[k],  vprime_avx[k], (SABER_EP-SABER_ET) );
	}

	// Unpack avx
	for(j=0;j<SABER_N/16;j++)
	{
			vst2q_u16  ((uint16_t *) (temp[0]+j*16), vprime_avx[j]);
	}
	
	#if Saber_type == 1
		SABER_pack_3bit(msk_c, temp[0]);
	#elif Saber_type == 2
		SABER_pack_4bit(msk_c, temp[0]);
	#elif Saber_type == 3
		SABER_pack_6bit(msk_c, temp[0]);
	#endif


	for(j=0;j<SABER_SCALEBYTES_KEM;j++){
		ciphertext[SABER_CIPHERTEXTBYTES + j] = msk_c[j];
	}

}


void indcpa_kem_dec(const unsigned char *sk, 
					const unsigned char *ciphertext, 
					unsigned char message_dec[])
{
	uint32_t i,j,k;
	
	
	uint16_t sksv[SABER_K][SABER_N]; //secret key of the server
	

	uint16_t pksv[SABER_K][SABER_N];
	
	uint16_t message_dec_unpacked[SABER_KEYBYTES*8];	// one element containes on decrypted bit;

	uint8_t scale_ar[SABER_SCALEBYTES_KEM];
	uint16_t op[SABER_N];

	//--------------AVX declaration------------------
	

	  
	  uint16x8x2_t v_avx[SABER_N/16];
	  
	  uint16x8x2_t acc[2*SABER_N/16];

	  uint16x8x2_t sksv_avx[SABER_K][SABER_N/16];
	  uint16x8x2_t pksv_avx[SABER_K][SABER_N/16];
	  

	  const uint16x8_t mod_p=vdupq_n_u16(SABER_P-1);

	  

	

	//--------------AVX declaration ends------------------
	
	//-------unpack the public_key

	BS2POLVEC(sk, sksv, SABER_Q); //sksv is the secret-key
	BS2POLVEC(ciphertext, pksv, SABER_P); //pksv is the ciphertext

	for(i=0;i<SABER_K;i++){
		for(j=0; j<SABER_N/16; j++){
		    sksv_avx[i][j] = vld2q_u16  ( (&sksv[i][j*16]));
		    pksv_avx[i][j] = vld2q_u16  ( (&pksv[i][j*16]));
		}
	}

	for(i=0;i<SABER_N/16;i++){
		XORDOT_DOT( v_avx[i], v_avx[i],v_avx[i]);
	}


	// InnerProduct(b', s, mod p)
	for(j=0;j<SABER_K;j++){

		toom_cook_4way_avx(pksv_avx[j], sksv_avx[j], SABER_P, acc);

			for(k=0;k<SABER_N/16;k++){
				ADDDOT_DOT(v_avx[k], v_avx[k],acc[k]);
				ANDDOT_DOTVAL( v_avx[k], v_avx[k],mod_p); //reduction
				XORDOT_DOT( acc[k], acc[k],acc[k]); //clear the accumulator
			}
	}

	for(i=0; i<SABER_N/16; i++){
		vst2q_u16 ((uint16_t *)(message_dec_unpacked+i*16), v_avx[i]);
	}


	for(i=0;i<SABER_SCALEBYTES_KEM;i++){
		scale_ar[i]=ciphertext[SABER_CIPHERTEXTBYTES+i];
	}

	#if Saber_type == 1
		SABER_un_pack3bit(scale_ar, op);
	#elif Saber_type == 2
		SABER_un_pack4bit(scale_ar, op);
	#elif Saber_type == 3
		SABER_un_pack6bit(scale_ar, op);
	#endif


	//addition of h2
	for(i=0;i<SABER_N;i++){
		message_dec_unpacked[i]= ( ( message_dec_unpacked[i] + h2 - (op[i]<<(SABER_EP-SABER_ET)) ) & (SABER_P-1) ) >> (SABER_EP-1);
	}


	POL2MSG(message_dec_unpacked, message_dec);
}

