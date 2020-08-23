
/*=============================================================================
 * Copyright (c) 2020 by Cryptographic Engineering Research Group (CERG)
 * ECE Department, George Mason University
 * Fairfax, VA, U.S.A.
 * Author: Duc Tri Nguyen

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

=============================================================================*/
#include <arm_neon.h>

// c = aa * bb
#define sb_vmul(c, aa, bb) c = vmulq_u16(aa, bb);

// c += aa*bb
#define sb_vmla(c, aa, bb) c = vmlaq_u16(c, aa, bb);

// load c <= a
#define sb_vload(c, a) c = vld1q_u16(a);

// store c <= a
#define sb_vstore(c, a) vst1q_u16(c, a);

// c = aa ^ bb
#define sb_vxor(c, aa, bb) c = veorq_u16(aa, bb);

#define SB_ITER 6

/*
 * schoolbook_neon_new
 * Assume aa, bb are already loaded
 */
void schoolbook_neon_new(uint16_t *restrict c_mem, uint16_t *restrict a_mem, uint16_t *restrict b_mem) {
  uint16x8_t tmp[8], aa[8], bb[8];
  uint16_t i;
  uint16_t pad = 0;
  uint16_t section = 0;
  for (i = 0; i < SB_ITER; i++) {

    sb_vload(aa[0], &a_mem[0 * 48 + pad]);
    sb_vload(ab[0], &b_mem[0 * 48 + pad]);

    sb_vmul(tmp[0], aa[0], bb[0]);
    sb_vstore(&c_mem[0 + pad], tmp[0]);
    //----
    sb_vload(aa[1], &a_mem[1 * 48 + pad]);
    sb_vload(ab[1], &b_mem[1 * 48 + pad]);

    sb_vmul(tmp[1], aa[0], bb[1]);
    sb_vmla(tmp[1], aa[1], bb[0]);
    sb_vstore(&c_mem[48 + pad], tmp[1]);
    //----
    sb_vload(aa[2], &a_mem[2 * 48 + pad]);
    sb_vload(ab[2], &b_mem[2 * 48 + pad]);

    sb_vmul(tmp[2], aa[0], bb[2]);
    sb_vmla(tmp[2], aa[1], bb[1]);
    sb_vmla(tmp[2], aa[2], bb[0]);
    sb_vstore(&c_mem[96 + pad], tmp[2]);
    //----
    sb_vload(aa[3], &a_mem[3 * 48 + pad]);
    sb_vload(ab[3], &b_mem[3 * 48 + pad]);

    sb_vmul(tmp[3], aa[0], bb[3]);
    sb_vmla(tmp[3], aa[1], bb[2]);
    sb_vmla(tmp[3], aa[2], bb[1]);
    sb_vmla(tmp[3], aa[3], bb[0]);
    sb_vstore(&c_mem[144 + pad], tmp[3]);
    //----
    sb_vload(aa[4], &a_mem[4 * 48 + pad]);
    sb_vload(ab[4], &b_mem[4 * 48 + pad]);

    sb_vmul(tmp[4], aa[0], bb[4]);
    sb_vmla(tmp[4], aa[1], bb[3]);
    sb_vmla(tmp[4], aa[2], bb[2]);
    sb_vmla(tmp[4], aa[3], bb[1]);
    sb_vmla(tmp[4], aa[4], bb[0]);
    sb_vstore(&c_mem[192 + pad], tmp[4]);
    //----
    sb_vload(aa[5], &a_mem[5 * 48 + pad]);
    sb_vload(ab[5], &b_mem[5 * 48 + pad]);

    sb_vmul(tmp[5], aa[0], bb[5]);
    sb_vmla(tmp[5], aa[1], bb[4]);
    sb_vmla(tmp[5], aa[2], bb[3]);
    sb_vmla(tmp[5], aa[3], bb[2]);
    sb_vmla(tmp[5], aa[4], bb[1]);
    sb_vmla(tmp[5], aa[5], bb[0]);
    sb_vstore(&c_mem[240 + pad], tmp[5]);
    //----
    sb_vload(aa[6], &a_mem[6 * 48 + pad]);
    sb_vload(ab[6], &b_mem[6 * 48 + pad]);

    sb_vmul(tmp[6], aa[0], bb[6]);
    sb_vmla(tmp[6], aa[1], bb[5]);
    sb_vmla(tmp[6], aa[2], bb[4]);
    sb_vmla(tmp[6], aa[3], bb[3]);
    sb_vmla(tmp[6], aa[4], bb[2]);
    sb_vmla(tmp[6], aa[5], bb[1]);
    sb_vmla(tmp[6], aa[6], bb[0]);
    sb_vstore(&c_mem[288 + pad], tmp[6]);
    //----
    sb_vload(aa[7], &a_mem[7 * 48 + pad]);
    sb_vload(ab[7], &b_mem[7 * 48 + pad]);

    sb_vmul(tmp[7], aa[0], bb[7]);
    sb_vmla(tmp[7], aa[1], bb[6]);
    sb_vmla(tmp[7], aa[2], bb[5]);
    sb_vmla(tmp[7], aa[3], bb[4]);
    sb_vmla(tmp[7], aa[4], bb[3]);
    sb_vmla(tmp[7], aa[5], bb[2]);
    sb_vmla(tmp[7], aa[6], bb[1]);
    sb_vmla(tmp[7], aa[7], bb[0]);
    sb_vstore(&c_mem[336 + pad], tmp[7]);
    //----
    //=======
    // ci =  {0, 1, 2, 3, 4, 5, 6, 7} 8
    // ai =  {0, 1, 2, 3, 4, 5, 6, 7} 8
    // bi =  {0, 1, 2, 3, 4, 5, 6, 7} 8
    // ----------------PART 2----------------
    sb_vmul(tmp[8], aa[1], bb[7]);
    sb_vmla(tmp[8], aa[2], bb[6]);
    sb_vmla(tmp[8], aa[3], bb[5]);
    sb_vmla(tmp[8], aa[4], bb[4]);
    sb_vmla(tmp[8], aa[5], bb[3]);
    sb_vmla(tmp[8], aa[6], bb[2]);
    sb_vmla(tmp[8], aa[7], bb[1]);
    sb_vstore(&c_mem[384 + pad], tmp);
    //-----
    sb_vmul(tmp[9], aa[2], bb[7]);
    sb_vmla(tmp[9], aa[3], bb[6]);
    sb_vmla(tmp[9], aa[4], bb[5]);
    sb_vmla(tmp[9], aa[5], bb[4]);
    sb_vmla(tmp[9], aa[6], bb[3]);
    sb_vmla(tmp[9], aa[7], bb[2]);
    sb_vstore(&c_mem[432 + pad], tmp);
    //-----
    sb_vmul(tmp[10], aa[3], bb[7]);
    sb_vmla(tmp[10], aa[4], bb[6]);
    sb_vmla(tmp[10], aa[5], bb[5]);
    sb_vmla(tmp[10], aa[6], bb[4]);
    sb_vmla(tmp[10], aa[7], bb[3]);
    sb_vstore(&c_mem[480 + pad], tmp);
    //-----
    sb_vmul(tmp[11], aa[4], bb[7]);
    sb_vmla(tmp[11], aa[5], bb[6]);
    sb_vmla(tmp[11], aa[6], bb[5]);
    sb_vmla(tmp[11], aa[7], bb[4]);
    sb_vstore(&c_mem[528 + pad], tmp);
    //-----
    sb_vmul(tmp[12], aa[5], bb[7]);
    sb_vmla(tmp[12], aa[6], bb[6]);
    sb_vmla(tmp[12], aa[7], bb[5]);
    sb_vstore(&c_mem[576 + pad], tmp);
    //-----
    sb_vmul(tmp[13], aa[6], bb[7]);
    sb_vmla(tmp[13], aa[7], bb[6]);
    sb_vstore(&c_mem[624 + pad], tmp);
    //-----
    sb_vmul(tmp[14], aa[7], bb[7]);
    sb_vstore(&c_mem[672 + pad], tmp);
    //-----

    pad += 8;
  }

  sb_vxor(tmp[0], tmp[0], tmp[0]);
  sb_vstore(&c_mem[15 * 48], tmp[0]);
  sb_vstore(&c_mem[15 * 48 + 8 * 1], tmp[0]);
  sb_vstore(&c_mem[15 * 48 + 8 * 2], tmp[0]);
  sb_vstore(&c_mem[15 * 48 + 8 * 3], tmp[0]);
  sb_vstore(&c_mem[15 * 48 + 8 * 4], tmp[0]);
  sb_vstore(&c_mem[15 * 48 + 8 * 5], tmp[0]);
}
