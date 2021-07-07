#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "../api.h"
#include "../SABER_params.h"
#include "../poly.h"
#include "../neon_poly_rq_mul.h"
#include "../m1cycles.h"

#define NTESTS 1000000

#define TIME(s) s = rdtsc();
// Result is clock cycles 
#define  CALC(start, stop) (stop - start) / NTESTS;


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
  uint16_t out[32*64], in1[16*64], in2[16*64];

  w[0] = &tmp[0 * SABER_N/2];
  w[1] = &tmp[1 * SABER_N/2];
  w[2] = &tmp[2 * SABER_N/2];
  w[3] = &tmp[3 * SABER_N/2];
  w[4] = &tmp[4 * SABER_N/2];
  w[5] = &tmp[5 * SABER_N/2];
  w[6] = &tmp[6 * SABER_N/2];


  // setup cycles
  setup_rdtsc();


  TIME(start);
  for(i=0;i<NTESTS;i++) {
    GenMatrix(matrix, seed);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("GenMatrix: %lld\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    GenSecret(s, seed);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("GenSecret: %lld\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    crypto_kem_keypair(pk, sk);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("crypto_kem_keypair: %lld\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    crypto_kem_enc(ct, key, pk);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("crypto_kem_enc: %lld\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    crypto_kem_dec(key, ct, sk);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("crypto_kem_dec: %lld\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    neonInnerProd(acc, a, b, s[0], 0);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("neonInnerProd: %lld\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    neonMatrixVectorMul(b, matrix, s, 1);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("neonMatrixVectorMul: %lld\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    tc4_evaluate_neon_SB1(w, acc);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("tc4_evaluate_neon_SB1: %lld\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    tc4_interpolate_neon_SB1(acc, w);
    neon_poly_neon_reduction(s[0], acc);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("tc4_interpolate_neon_SB1: %lld\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    tc4_interpolate_neon_SB1(acc, w);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("tc4_interpolate_neon_SB1 without ring reduction: %lld\n", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    schoolbook16x16(out, in1, in2, 8);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("schoolbook16x16 8 iterations: %lld\n", ns);
  
  TIME(start);
  for(i=0;i<NTESTS;i++) {
    schoolbook16x16(out, in1, in2, 6);
  }
  TIME(stop);
  ns = CALC(start, stop);
  printf("schoolbook16x16 6 iterations: %lld\n", ns);

  return 0;
}
