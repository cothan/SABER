#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <papi.h>
#include "../api.h"
#include "../SABER_params.h"
#include "../poly.h"
#include "../neon_poly_rq_mul.h"

#define NTESTS 100000

#define START(funcname) PAPI_hl_region_begin(funcname);
#define END(funcname) PAPI_hl_region_end(funcname);


uint8_t seed[SABER_SEEDBYTES] = {0};

int main()
{
  unsigned int i;
  unsigned char pk[CRYPTO_PUBLICKEYBYTES] = {0};
  unsigned char sk[CRYPTO_SECRETKEYBYTES] = {0};
  unsigned char ct[CRYPTO_CIPHERTEXTBYTES] = {0};
  unsigned char key[CRYPTO_BYTES] = {0};
  long long start, stop;
  long long ns;
  
  uint16_t matrix[SABER_L][SABER_L][SABER_N] = {0};
  uint16_t s[SABER_L][SABER_N] = {0};
  uint16_t a[SABER_L][SABER_N] = {0};
  uint16_t b[SABER_L][SABER_N] = {0};
  uint16_t acc[2*SABER_N] = {0};

  uint16_t *w[7];
  uint16_t tmp[7 * SABER_N/2] = {0};

  w[0] = &tmp[0 * SABER_N/2];
  w[1] = &tmp[1 * SABER_N/2];
  w[2] = &tmp[2 * SABER_N/2];
  w[3] = &tmp[3 * SABER_N/2];
  w[4] = &tmp[4 * SABER_N/2];
  w[5] = &tmp[5 * SABER_N/2];
  w[6] = &tmp[6 * SABER_N/2];




  START("GenMatrix");
  for(i=0;i<NTESTS;i++) {
    GenMatrix(matrix, seed);
  }
  END("GenMatrix");

  START("GenSecret");
  for(i=0;i<NTESTS;i++) {
    GenSecret(s, seed);
  }
  END("GenSecret");

  START("crypto_kem_keypair");
  for(i=0;i<NTESTS;i++) {
    crypto_kem_keypair(pk, sk);
  }
  END("crypto_kem_keypair");

  START("crypto_kem_enc");
  for(i=0;i<NTESTS;i++) {
    crypto_kem_enc(ct, key, pk);
  }
  END("crypto_kem_enc");

  START("crypto_kem_dec");
  for(i=0;i<NTESTS;i++) {
    crypto_kem_dec(key, ct, sk);
  }
  END("crypto_kem_dec");

  START("neonInnerProd");
  for(i=0;i<NTESTS;i++) {
    neonInnerProd(acc, a, b, s[0], 0);
  }
  END("neonInnerProd");

  START("neonMatrixVectorMul");
  for(i=0;i<NTESTS;i++) {
    neonMatrixVectorMul(b, matrix, s, 1);
  }
  END("neonMatrixVectorMul");

  START("tc4_evaluate_neon_SB1");
  for(i=0;i<NTESTS;i++) {
    tc4_evaluate_neon_SB1(w, acc);
  }
  END("tc4_evaluate_neon_SB1");
  
  START("tc4_interpolate_neon_SB1_reduce");
  for(i=0;i<NTESTS;i++) {
    tc4_interpolate_neon_SB1(acc, w);
    neon_poly_neon_reduction(s[0], acc);
  }
  END("tc4_interpolate_neon_SB1_reduce");
  
  START("tc4_interpolate_neon_SB1");
  for(i=0;i<NTESTS;i++) {
    tc4_interpolate_neon_SB1(acc, w);
  }
  END("tc4_interpolate_neon_SB1");
  

  return 0;
}
