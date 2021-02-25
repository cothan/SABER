#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "../api.h"
#include "../SABER_params.h"
#include "../poly.h"
#include "../SABER_indcpa.h"
#include <time.h>
#include "../print.h"

#define NTESTS 1000000

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
  clock_t start, end;

  
  start = clock();
  for(i=0;i<NTESTS;i++) {
    // t[i] = cpucycles();
    GenMatrix(matrix, seed);
  }
  end = clock() - start;
  print("GenMatrix:", ((double) end)/NTESTS);
  // print_results("GenMatrix: ", t, NTESTS);
  
  start = clock();
  for(i=0;i<NTESTS;i++) {
    // t[i] = cpucycles();
    GenSecret(s, seed);
  }
  end = clock() - start;
  print("GenSecret:", ((double) end)/NTESTS);
  // print_results("GenSecret: ", t, NTESTS);
  
  start = clock();
  for(i=0;i<NTESTS;i++) {
    // t[i] = cpucycles();
    crypto_kem_keypair(pk, sk);
  }
  end = clock() - start;
  print("crypto_kem_keypair:", ((double) end)/NTESTS);
  // print_results("saber_keypair: ", t, NTESTS);
  
  start = clock();
  for(i=0;i<NTESTS;i++) {
    // t[i] = cpucycles();
    crypto_kem_enc(ct, key, pk);
  }
  end = clock() - start;
  print("crypto_kem_enc:", ((double) end)/NTESTS);
  // print_results("saber_encaps: ", t, NTESTS);
  
  start = clock();
  for(i=0;i<NTESTS;i++) {
    // t[i] = cpucycles();
    crypto_kem_dec(key, ct, sk);
  }
  end = clock() - start;
  print("crypto_kem_dec:", ((double) end)/NTESTS);
  // print_results("saber_decaps: ", t, NTESTS);

  start = clock();
  for(i=0;i<NTESTS;i++) {
    InnerProd(a, b, acc);
  }
  end = clock() - start;
  print("InnerProd:", ((double) end)/NTESTS);

  start = clock();
  for(i=0;i<NTESTS;i++) {
    MatrixVectorMul(matrix, s, b, 1);
  }
  end = clock() - start;
  print("MatrixVectorMul:", ((double) end)/NTESTS);



  return 0;
}
