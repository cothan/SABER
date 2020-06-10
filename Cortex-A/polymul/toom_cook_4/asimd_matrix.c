// #define permute0(c, a, b, value) \
//   vst1q_u16( c,     (uint16x8_t) a.val[0]); \
//   vst1q_u16( c + 8, (uint16x8_t) b.val[0]);

// #define permute1(c, a, b, value)  \
//   vst1q_u16(c,     (uint16x8_t) a.val[1]); \
//   vst1q_u16(c + 8, (uint16x8_t) b.val[1]);

// /*
//  * tranpose
//  * Input: Memory uint16_t *M
//  * Output: Memory uint16_t *M
//  */
// void transpose_old(uint16_t *M) {
//   uint16_t i;
//   uint16x8x2_t tL[8], tH[8];
//   uint32x4x2_t bL[4], bH[4], cL[4], cH[4];
//   uint64x2x2_t dL[2], dH[2], eL[2], eH[2], fL[2], fH[2], gL[2], gH[2];

//   uint16x8x2_t aa, bb;

//   for (i = 0; i < 8; i = i + 1) {
//     aa = vld2q_u16(&M[(2 * i) * 16]);
//     bb = vld2q_u16(&M[(2 * i + 1) * 16]);

//     tL[i].val[0] = vzip2q_u16(aa.val[0], bb.val[0]);
//     tL[i].val[1] = vzip1q_u16(aa.val[0], bb.val[0]);

//     tH[i].val[0] = vzip2q_u16(aa.val[1], bb.val[1]);
//     tH[i].val[1] = vzip1q_u16(aa.val[1], bb.val[1]);
//   }
// //-----------------------------------
//   for (i = 0; i < 4; i = i + 1) {
//     bL[i].val[0] = vzip2q_u32((uint32x4_t)tL[2 * i].val[0], (uint32x4_t)tL[2 * i + 1].val[0]);
//     bL[i].val[1] = vzip1q_u32((uint32x4_t)tL[2 * i].val[0], (uint32x4_t)tL[2 * i + 1].val[0]);

//     bH[i].val[0] = vzip2q_u32((uint32x4_t)tL[2 * i].val[1], (uint32x4_t)tL[2 * i + 1].val[1]);
//     bH[i].val[1] = vzip1q_u32((uint32x4_t)tL[2 * i].val[1], (uint32x4_t)tL[2 * i + 1].val[1]);
//   }
// //-----------------------------------
//   for (i = 0; i < 4; i = i + 1) {
//     cL[i].val[0] = vzip2q_u32((uint32x4_t)tH[2 * i].val[0], (uint32x4_t)tH[2 * i + 1].val[0]);
//     cL[i].val[1] = vzip1q_u32((uint32x4_t)tH[2 * i].val[0], (uint32x4_t)tH[2 * i + 1].val[0]);

//     cH[i].val[0] = vzip2q_u32((uint32x4_t)tH[2 * i].val[1], (uint32x4_t)tH[2 * i + 1].val[1]);
//     cH[i].val[1] = vzip1q_u32((uint32x4_t)tH[2 * i].val[1], (uint32x4_t)tH[2 * i + 1].val[1]);
//   }
// //------------------------------------
//   for (i = 0; i < 2; i = i + 1) {
//     dL[i].val[0] = vzip2q_u64((uint64x2_t)bL[2 * i].val[0], (uint64x2_t)bL[2 * i + 1].val[0]);
//     dL[i].val[1] = vzip1q_u64((uint64x2_t)bL[2 * i].val[0], (uint64x2_t)bL[2 * i + 1].val[0]);

//     dH[i].val[0] = vzip2q_u64((uint64x2_t)bL[2 * i].val[1], (uint64x2_t)bL[2 * i + 1].val[1]);
//     dH[i].val[1] = vzip1q_u64((uint64x2_t)bL[2 * i].val[1], (uint64x2_t)bL[2 * i + 1].val[1]);
//   }
//   permute0(&M[0*16], dL[0], dL[1], 0x20);
//   permute1(&M[8*16], dL[0], dL[1], 0x31);
//   permute0(&M[1*16], dH[0], dH[1], 0x20);
//   permute1(&M[9*16], dH[0], dH[1], 0x31);
// //------------------------------------
//   for (i = 0; i < 2; i = i + 1) {
//     eL[i].val[0] = vzip2q_u64((uint64x2_t)bH[2 * i].val[0], (uint64x2_t)bH[2 * i + 1].val[0]);
//     eL[i].val[1] = vzip1q_u64((uint64x2_t)bH[2 * i].val[0], (uint64x2_t)bH[2 * i + 1].val[0]);

//     eH[i].val[0] = vzip2q_u64((uint64x2_t)bH[2 * i].val[1], (uint64x2_t)bH[2 * i + 1].val[1]);
//     eH[i].val[1] = vzip1q_u64((uint64x2_t)bH[2 * i].val[1], (uint64x2_t)bH[2 * i + 1].val[1]);
//   }
//   permute0(&M[16*2], eL[0], eL[1], 0x20);
//   permute1(&M[16*10], eL[0], eL[1], 0x31);
//   permute0(&M[16*3], eH[0], eH[1], 0x20);
//   permute1(&M[16*11], eH[0], eH[1], 0x31);
// //-------------------------------------
//   for (i = 0; i < 2; i = i + 1) {
//     fL[i].val[0] = vzip2q_u64((uint64x2_t)cL[2 * i].val[0], (uint64x2_t)cL[2 * i + 1].val[0]);
//     fL[i].val[1] = vzip1q_u64((uint64x2_t)cL[2 * i].val[0], (uint64x2_t)cL[2 * i + 1].val[0]);

//     fH[i].val[0] = vzip2q_u64((uint64x2_t)cL[2 * i].val[1], (uint64x2_t)cL[2 * i + 1].val[1]);
//     fH[i].val[1] = vzip1q_u64((uint64x2_t)cL[2 * i].val[1], (uint64x2_t)cL[2 * i + 1].val[1]);
//   }
//   permute0(&M[16*4], fL[0], fL[1], 0x20);
//   permute1(&M[16*12], fL[0], fL[1], 0x31);
//   permute0(&M[16*5], fH[0], fH[1], 0x20);
//   permute1(&M[16*13], fH[0], fH[1], 0x31);
//   //-------------------------------------
//   for (i = 0; i < 2; i = i + 1) {
//     gL[i].val[0] = vzip2q_u64((uint64x2_t)cH[2 * i].val[0], (uint64x2_t)cH[2 * i + 1].val[0]);
//     gL[i].val[1] = vzip1q_u64((uint64x2_t)cH[2 * i].val[0], (uint64x2_t)cH[2 * i + 1].val[0]);

//     gH[i].val[0] = vzip2q_u64((uint64x2_t)cH[2 * i].val[1], (uint64x2_t)cH[2 * i + 1].val[1]);
//     gH[i].val[1] = vzip1q_u64((uint64x2_t)cH[2 * i].val[1], (uint64x2_t)cH[2 * i + 1].val[1]);
//   }
//   permute0(&M[16*6], gL[0], gL[1], 0x20);
//   permute1(&M[16*14], gL[0], gL[1], 0x31);
//   permute0(&M[16*7], gH[0], gH[1], 0x20);
//   permute1(&M[16*15], gH[0], gH[1], 0x31);

// }

/*
 * Improved in-place tranpose, minimal spill to memory.
 * Input: Memory uint16_t *M
 * Output: Memory uint16_t *M
 */
void transpose(uint16_t *M) {

  uint16x8_t y0, y1, y2, y3, y4, y5, y6, y7, y8;
  uint16x8_t y9, y10, y11, y12, y13, y14, y15, y16, y17;
  uint16x8_t y18, y19, y20, y21, y22, y23, y24, y25, y26;
  uint16x8_t y27, y28, y29, y30, y31, y32, y33, y34, y35;
  // 16x16: LD A1
  y0 = vld1q_u16(M + 0);
  y1 = vld1q_u16(M + 16);
  y2 = vld1q_u16(M + 32);
  y3 = vld1q_u16(M + 48);
  y4 = vld1q_u16(M + 64);
  y5 = vld1q_u16(M + 80);
  y6 = vld1q_u16(M + 96);
  y7 = vld1q_u16(M + 112);
  // Transpose 8x8
  y16 = vtrn1q_u16(y0, y1);
  y17 = vtrn2q_u16(y0, y1);
  y18 = vtrn1q_u16(y2, y3);
  y19 = vtrn2q_u16(y2, y3);
  y24 = vtrn1q_u32(y16, y17);
  y25 = vtrn2q_u32(y16, y17);
  y26 = vtrn1q_u32(y18, y19);
  y27 = vtrn2q_u32(y18, y19);
  y8 = vtrn1q_u32(y24, y26);
  y10 = vtrn2q_u32(y24, y26);
  y9 = vtrn1q_u32(y25, y27);
  y11 = vtrn2q_u32(y25, y27);
  y16 = vtrn1q_u16(y4, y5);
  y17 = vtrn2q_u16(y4, y5);
  y18 = vtrn1q_u16(y6, y7);
  y19 = vtrn2q_u16(y6, y7);
  y24 = vtrn1q_u32(y16, y17);
  y25 = vtrn2q_u32(y16, y17);
  y26 = vtrn1q_u32(y18, y19);
  y27 = vtrn2q_u32(y18, y19);
  y12 = vtrn1q_u32(y24, y26);
  y14 = vtrn2q_u32(y24, y26);
  y13 = vtrn1q_u32(y25, y27);
  y15 = vtrn2q_u32(y25, y27);
  y16 = vtrn1q_u64(y8, y12);
  y17 = vtrn2q_u64(y8, y12);
  y18 = vtrn1q_u64(y10, y14);
  y19 = vtrn2q_u64(y10, y14);
  y20 = vtrn1q_u64(y9, y13);
  y21 = vtrn2q_u64(y9, y13);
  y22 = vtrn1q_u64(y11, y15);
  y23 = vtrn2q_u64(y11, y15);
  // 16x16: STR A1
  vst1q_u16(M + 0, y16);
  vst1q_u16(M + 16, y18);
  vst1q_u16(M + 32, y20);
  vst1q_u16(M + 48, y22);
  vst1q_u16(M + 64, y17);
  vst1q_u16(M + 80, y19);
  vst1q_u16(M + 96, y21);
  vst1q_u16(M + 112, y23);
  // 16x16: LD A4
  y0 = vld1q_u16(M + 136);
  y1 = vld1q_u16(M + 152);
  y2 = vld1q_u16(M + 168);
  y3 = vld1q_u16(M + 184);
  y4 = vld1q_u16(M + 200);
  y5 = vld1q_u16(M + 216);
  y6 = vld1q_u16(M + 232);
  y7 = vld1q_u16(M + 248);
  // Transpose 8x8
  y16 = vtrn1q_u16(y0, y1);
  y18 = vtrn2q_u16(y0, y1);
  y20 = vtrn1q_u16(y2, y3);
  y22 = vtrn2q_u16(y2, y3);
  y24 = vtrn1q_u32(y16, y18);
  y25 = vtrn2q_u32(y16, y18);
  y26 = vtrn1q_u32(y20, y22);
  y27 = vtrn2q_u32(y20, y22);
  y8 = vtrn1q_u32(y24, y26);
  y10 = vtrn2q_u32(y24, y26);
  y9 = vtrn1q_u32(y25, y27);
  y11 = vtrn2q_u32(y25, y27);
  y16 = vtrn1q_u16(y4, y5);
  y18 = vtrn2q_u16(y4, y5);
  y20 = vtrn1q_u16(y6, y7);
  y22 = vtrn2q_u16(y6, y7);
  y24 = vtrn1q_u32(y16, y18);
  y25 = vtrn2q_u32(y16, y18);
  y26 = vtrn1q_u32(y20, y22);
  y27 = vtrn2q_u32(y20, y22);
  y12 = vtrn1q_u32(y24, y26);
  y14 = vtrn2q_u32(y24, y26);
  y13 = vtrn1q_u32(y25, y27);
  y15 = vtrn2q_u32(y25, y27);
  y16 = vtrn1q_u64(y8, y12);
  y18 = vtrn2q_u64(y8, y12);
  y20 = vtrn1q_u64(y10, y14);
  y22 = vtrn2q_u64(y10, y14);
  y17 = vtrn1q_u64(y9, y13);
  y19 = vtrn2q_u64(y9, y13);
  y21 = vtrn1q_u64(y11, y15);
  y23 = vtrn2q_u64(y11, y15);
  // 16x16: STR A4
  vst1q_u16(M + 136, y16);
  vst1q_u16(M + 152, y20);
  vst1q_u16(M + 168, y17);
  vst1q_u16(M + 184, y21);
  vst1q_u16(M + 200, y18);
  vst1q_u16(M + 216, y22);
  vst1q_u16(M + 232, y19);
  vst1q_u16(M + 248, y23);
  // 16x16: LD A2
  y0 = vld1q_u16(M + 8);
  y1 = vld1q_u16(M + 24);
  y2 = vld1q_u16(M + 40);
  y3 = vld1q_u16(M + 56);
  y4 = vld1q_u16(M + 72);
  y5 = vld1q_u16(M + 88);
  y6 = vld1q_u16(M + 104);
  y7 = vld1q_u16(M + 120);
  // Transpose 8x8
  y16 = vtrn1q_u16(y0, y1);
  y20 = vtrn2q_u16(y0, y1);
  y17 = vtrn1q_u16(y2, y3);
  y21 = vtrn2q_u16(y2, y3);
  y24 = vtrn1q_u32(y16, y20);
  y25 = vtrn2q_u32(y16, y20);
  y26 = vtrn1q_u32(y17, y21);
  y27 = vtrn2q_u32(y17, y21);
  y8 = vtrn1q_u32(y24, y26);
  y10 = vtrn2q_u32(y24, y26);
  y9 = vtrn1q_u32(y25, y27);
  y11 = vtrn2q_u32(y25, y27);
  y16 = vtrn1q_u16(y4, y5);
  y20 = vtrn2q_u16(y4, y5);
  y17 = vtrn1q_u16(y6, y7);
  y21 = vtrn2q_u16(y6, y7);
  y24 = vtrn1q_u32(y16, y20);
  y25 = vtrn2q_u32(y16, y20);
  y26 = vtrn1q_u32(y17, y21);
  y27 = vtrn2q_u32(y17, y21);
  y12 = vtrn1q_u32(y24, y26);
  y14 = vtrn2q_u32(y24, y26);
  y13 = vtrn1q_u32(y25, y27);
  y15 = vtrn2q_u32(y25, y27);
  y16 = vtrn1q_u64(y8, y12);
  y20 = vtrn2q_u64(y8, y12);
  y17 = vtrn1q_u64(y10, y14);
  y21 = vtrn2q_u64(y10, y14);
  y18 = vtrn1q_u64(y9, y13);
  y22 = vtrn2q_u64(y9, y13);
  y19 = vtrn1q_u64(y11, y15);
  y23 = vtrn2q_u64(y11, y15);
  // 16x16: LD A3
  y0 = vld1q_u16(M + 128);
  y1 = vld1q_u16(M + 144);
  y2 = vld1q_u16(M + 160);
  y3 = vld1q_u16(M + 176);
  y4 = vld1q_u16(M + 192);
  y5 = vld1q_u16(M + 208);
  y6 = vld1q_u16(M + 224);
  y7 = vld1q_u16(M + 240);
  // Transpose 8x8
  y28 = vtrn1q_u16(y0, y1);
  y29 = vtrn2q_u16(y0, y1);
  y30 = vtrn1q_u16(y2, y3);
  y31 = vtrn2q_u16(y2, y3);
  y24 = vtrn1q_u32(y28, y29);
  y25 = vtrn2q_u32(y28, y29);
  y26 = vtrn1q_u32(y30, y31);
  y27 = vtrn2q_u32(y30, y31);
  y8 = vtrn1q_u32(y24, y26);
  y10 = vtrn2q_u32(y24, y26);
  y9 = vtrn1q_u32(y25, y27);
  y11 = vtrn2q_u32(y25, y27);
  y28 = vtrn1q_u16(y4, y5);
  y29 = vtrn2q_u16(y4, y5);
  y30 = vtrn1q_u16(y6, y7);
  y31 = vtrn2q_u16(y6, y7);
  y24 = vtrn1q_u32(y28, y29);
  y25 = vtrn2q_u32(y28, y29);
  y26 = vtrn1q_u32(y30, y31);
  y27 = vtrn2q_u32(y30, y31);
  y12 = vtrn1q_u32(y24, y26);
  y14 = vtrn2q_u32(y24, y26);
  y13 = vtrn1q_u32(y25, y27);
  y15 = vtrn2q_u32(y25, y27);
  y28 = vtrn1q_u64(y8, y12);
  y29 = vtrn2q_u64(y8, y12);
  y30 = vtrn1q_u64(y10, y14);
  y31 = vtrn2q_u64(y10, y14);
  y32 = vtrn1q_u64(y9, y13);
  y33 = vtrn2q_u64(y9, y13);
  y34 = vtrn1q_u64(y11, y15);
  y35 = vtrn2q_u64(y11, y15);
  // 16x16: STR A2<-A3
  vst1q_u16(M + 8, y28);
  vst1q_u16(M + 24, y30);
  vst1q_u16(M + 40, y32);
  vst1q_u16(M + 56, y34);
  vst1q_u16(M + 72, y29);
  vst1q_u16(M + 88, y31);
  vst1q_u16(M + 104, y33);
  vst1q_u16(M + 120, y35);
  // 16x16: STR A3<-A2
  vst1q_u16(M + 128, y16);
  vst1q_u16(M + 144, y17);
  vst1q_u16(M + 160, y18);
  vst1q_u16(M + 176, y19);
  vst1q_u16(M + 192, y20);
  vst1q_u16(M + 208, y21);
  vst1q_u16(M + 224, y22);
  vst1q_u16(M + 240, y23);
}
