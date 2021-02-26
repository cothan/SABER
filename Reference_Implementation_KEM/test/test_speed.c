#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "../api.h"
#include "../SABER_params.h"
#include "../poly.h"
// #include "cpucycles.h"
// #include "speed_print.h"
#include <time.h>
#include "../print.h"

#define NTESTS 1000000

#define TIME(s) clock_gettime(CLOCK_MONOTONIC_RAW, &s);
// Result is nanosecond per call 
#define  CALC(start, stop) \
  ((double) ((stop.tv_sec - start.tv_sec) * 1000000000 + (stop.tv_nsec - start.tv_nsec))) / NTESTS;

uint8_t seed[SABER_SEEDBYTES] = {0};

int main()
{
  unsigned int i;
  unsigned char pk[CRYPTO_PUBLICKEYBYTES] = {0};
  unsigned char sk[CRYPTO_SECRETKEYBYTES] = {0};
  unsigned char ct[CRYPTO_CIPHERTEXTBYTES] = {0};
  unsigned char key[CRYPTO_BYTES] = {0};
  
  uint16_t matrix[SABER_L][SABER_L][SABER_N] = {0};
  uint16_t s[SABER_L][SABER_N] = {0};
  uint16_t a[SABER_L][SABER_N] = {0};
  uint16_t b[SABER_L][SABER_N] = {0};
  uint16_t acc[SABER_N] = {0};
  struct timespec start, stop;
  long ns;
  

  
  TIME(start);
  for(i=0;i<NTESTS;i++) {
    GenMatrix(matrix, seed);
  }
  TIME(stop);
  ns = CALC(start, stop);
  print("GenMatrix:", ns);
  
  TIME(start);
  for(i=0;i<NTESTS;i++) {
    GenSecret(s, seed);
  }
  TIME(stop);
  ns = CALC(start, stop);
  print("GenSecret:", ns);
  
  TIME(start);
  for(i=0;i<NTESTS;i++) {
    crypto_kem_keypair(pk, sk);
  }
  TIME(stop);
  ns = CALC(start, stop);
  print("crypto_kem_keypair:", ns);
  
  TIME(start);
  for(i=0;i<NTESTS;i++) {
    crypto_kem_enc(ct, key, pk);
  }
  TIME(stop);
  ns = CALC(start, stop);
  print("crypto_kem_enc:", ns);
  
  TIME(start);
  for(i=0;i<NTESTS;i++) {
    crypto_kem_dec(key, ct, sk);
  }
  TIME(stop);
  ns = CALC(start, stop);
  print("crypto_kem_dec:", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    InnerProd(a, b, acc);
  }
  TIME(stop);
  ns = CALC(start, stop);
  print("InnerProd:", ns);

  TIME(start);
  for(i=0;i<NTESTS;i++) {
    MatrixVectorMul(matrix, s, b, 1);
  }
  TIME(stop);
  ns = CALC(start, stop);
  print("MatrixVectorMul:", ns);

  print("CLOCKS_PER_SEC:", CLOCKS_PER_SEC);

  return 0;
}
