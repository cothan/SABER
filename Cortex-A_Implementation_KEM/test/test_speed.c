#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "../api.h"
#include "../SABER_params.h"
#include "../poly.h"
#include "../neon_poly_rq_mul.h"
#include <papi.h>

#define NTESTS 100000

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

  PAPI_hl_region_begin("GenMatrix");
  for(i=0;i<NTESTS;i++) {
    GenMatrix(matrix, seed);
  }
  PAPI_hl_region_end("GenMatrix");

  PAPI_hl_region_begin("GenSecret");
  for(i=0;i<NTESTS;i++) {
    GenSecret(s, seed);
  }
  PAPI_hl_region_end("GenSecret");

  PAPI_hl_region_begin("crypto_kem_keypair");
  for(i=0;i<NTESTS;i++) {
    crypto_kem_keypair(pk, sk);
  }
  PAPI_hl_region_end("crypto_kem_keypair");

  PAPI_hl_region_begin("crypto_kem_enc");
  for(i=0;i<NTESTS;i++) {
    crypto_kem_enc(ct, key, pk);
  }
  PAPI_hl_region_end("crypto_kem_enc");

  PAPI_hl_region_begin("crypto_kem_dec");
  for(i=0;i<NTESTS;i++) {
    crypto_kem_dec(key, ct, sk);
  }
  PAPI_hl_region_end("crypto_kem_dec");

  PAPI_hl_region_begin("neonInnerProd");
  for(i=0;i<NTESTS;i++) {
    neonInnerProd(acc, a, b, s[0], 0);
  }
  PAPI_hl_region_end("neonInnerProd");

  PAPI_hl_region_begin("neonMatrixVectorMul");
  for(i=0;i<NTESTS;i++) {
    neonMatrixVectorMul(b, matrix, s, 1);
  }
  PAPI_hl_region_end("neonMatrixVectorMul");



  return 0;
}
