#include <stdint.h>

#include "../kem.h"
#include "../api.h"
#include "../rng.h"
#include "../SABER_params.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <papi.h>

extern int crypto_kem_keypair(unsigned char *pk, unsigned char *sk);
extern int crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk);
extern int crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);

int test_kem_cca()
{

    uint8_t pk[SABER_PUBLICKEYBYTES];
    uint8_t sk[SABER_SECRETKEYBYTES];
    uint8_t c[SABER_BYTES_CCA_DEC];
    uint8_t k_a[SABER_KEYBYTES], k_b[SABER_KEYBYTES];

    int retval;

    unsigned char entropy_input[48];

    uint64_t i, repeat;
    repeat = 10000;
    //repeat = 1;

    time_t t;
    // Intializes random number generator
    srand((unsigned)time(&t));

    for (i = 0; i < 48; i++)
    {
        entropy_input[i] = i;
        //entropy_input[i] = rand()%256;
    }
    randombytes_init(entropy_input, NULL, 256);
    /* =================================== */
    retval = PAPI_hl_region_begin("keypair");
    for (i = 0; i < repeat; i++)
    {
        crypto_kem_keypair(pk, sk);
    }
    retval = PAPI_hl_region_end("keypair");
    if (retval != PAPI_OK)
    {
        return 1;
    }
    /* =================================== */
    retval = PAPI_hl_region_begin("encaps");
    for (i = 0; i < repeat; i++)
    {
        crypto_kem_enc(c, k_a, pk);
    }
    retval = PAPI_hl_region_end("encaps");
    if (retval != PAPI_OK)
    {
        return 1;
    }
    /* =================================== */
    retval = PAPI_hl_region_begin("decaps");
    for (i = 0; i < repeat; i++)
    {
        crypto_kem_dec(k_b, c, sk);
    }
    retval = PAPI_hl_region_end("decaps");
    if (retval != PAPI_OK)
    {
        return 1;
    }
    /* =================================== */
    return 0;
}

int main()
{
    test_kem_cca();
    return 0;
}
