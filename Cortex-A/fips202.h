#ifndef FIPS202_H
#define FIPS202_H

#include <stdint.h>

#define SHAKE128_RATE 168
#define SHAKE256_RATE 136
#define SHA3_256_RATE 136
#define SHA3_512_RATE 72

#if __aarch64__
#include <libkeccak.a.headers/SimpleFIPS202.h>
#else
#error "Install SIMD Keccak"
#endif

#define shake128 SHAKE128
#define sha3_256 SHA3_256
#define sha3_512 SHA3_512


// void shake128(unsigned char *output, unsigned long long outlen, const unsigned char *input,  unsigned long long inlen);

// //void shake128(unsigned char *output, unsigned long long outlen, const unsigned char *input,  unsigned long long inlen);
// void sha3_256(unsigned char *output, const unsigned char *input,  unsigned long long inlen);
// void sha3_512(unsigned char *output, const unsigned char *input,  unsigned long long inlen);

void cshake128_simple(unsigned char *output, unsigned long long outlen, 
                      uint16_t cstm, // 2-byte domain separator
                      const unsigned char *in, unsigned long long inlen);

#endif
