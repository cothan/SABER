#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "../api.h"
#include "../SABER_params.h"
#include "../poly.h"
#include <papi.h>

#define NTESTS 100000

uint64_t t[NTESTS];
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
  
  PAPI_hl_region_begin("InnerProd");
  for(i=0;i<NTESTS;i++) {
    InnerProd(b, s, acc);
  }
  PAPI_hl_region_end("InnerProd");

  PAPI_hl_region_begin("MatrixVectorMul");
  for(i=0;i<NTESTS;i++) {
    MatrixVectorMul(matrix, s, b, 1);
  }
  PAPI_hl_region_end("MatrixVectorMul");


  return 0;
}
