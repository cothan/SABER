#define PERMUTE0(c, a, b, value)                                                                   \
  c->val[0] = (uint16x8_t)a.val[0];                                                                \
  c->val[1] = (uint16x8_t)b.val[0]

#define PERMUTE1(c, a, b, value)                                                                          \
  c->val[0] = (uint16x8_t)a.val[1];                                                                \
  c->val[1] = (uint16x8_t)b.val[1]

void transpose(uint16x8x2_t **M) {
  int i;
  uint16x8x2_t tL[8], tH[8];
  uint32x4x2_t bL[4], bH[4], cL[4], cH[4];
  uint64x2x2_t dL[2], dH[2], eL[2], eH[2], fL[2], fH[2], gL[2], gH[2];

  for (i = 0; i < 8; i = i + 1) {
    tL[i].val[0] = vzip2q_u16(M[2 * i]->val[0], M[2 * i + 1]->val[0]);
    tL[i].val[1] = vzip1q_u16(M[2 * i]->val[0], M[2 * i + 1]->val[0]);
    tH[i].val[0] = vzip2q_u16(M[2 * i]->val[1], M[2 * i + 1]->val[1]);
    tH[i].val[1] = vzip1q_u16(M[2 * i]->val[1], M[2 * i + 1]->val[1]);
  }
  //-----------------------------------
  for (i = 0; i < 4; i = i + 1) {
    bL[i].val[0] = vzip2q_u32((uint32x4_t)tL[2 * i].val[0], (uint32x4_t)tL[2 * i + 1].val[0]);
    bL[i].val[1] = vzip1q_u32((uint32x4_t)tL[2 * i].val[0], (uint32x4_t)tL[2 * i + 1].val[0]);

    bH[i].val[0] = vzip2q_u32((uint32x4_t)tL[2 * i].val[1], (uint32x4_t)tL[2 * i + 1].val[1]);
    bH[i].val[1] = vzip1q_u32((uint32x4_t)tL[2 * i].val[1], (uint32x4_t)tL[2 * i + 1].val[1]);
  }
  //-----------------------------------
  for (i = 0; i < 4; i = i + 1) {
    cL[i].val[0] = vzip2q_u32((uint32x4_t)tH[2 * i].val[0], (uint32x4_t)tH[2 * i + 1].val[0]);
    cL[i].val[1] = vzip1q_u32((uint32x4_t)tH[2 * i].val[0], (uint32x4_t)tH[2 * i + 1].val[0]);

    cH[i].val[0] = vzip2q_u32((uint32x4_t)tH[2 * i].val[1], (uint32x4_t)tH[2 * i + 1].val[1]);
    cH[i].val[1] = vzip1q_u32((uint32x4_t)tH[2 * i].val[1], (uint32x4_t)tH[2 * i + 1].val[1]);
  }
  //------------------------------------
  for (i = 0; i < 2; i = i + 1) {
    dL[i].val[0] = vzip2q_u64((uint64x2_t)bL[2 * i].val[0], (uint64x2_t)bL[2 * i + 1].val[0]);
    dL[i].val[1] = vzip1q_u64((uint64x2_t)bL[2 * i].val[0], (uint64x2_t)bL[2 * i + 1].val[0]);

    dH[i].val[0] = vzip2q_u64((uint64x2_t)bL[2 * i].val[1], (uint64x2_t)bL[2 * i + 1].val[1]);
    dH[i].val[1] = vzip1q_u64((uint64x2_t)bL[2 * i].val[1], (uint64x2_t)bL[2 * i + 1].val[1]);
  }
  //------------------------------------
  for (i = 0; i < 2; i = i + 1) {
    eL[i].val[0] = vzip2q_u64((uint64x2_t)bH[2 * i].val[0], (uint64x2_t)bH[2 * i + 1].val[0]);
    eL[i].val[1] = vzip1q_u64((uint64x2_t)bH[2 * i].val[0], (uint64x2_t)bH[2 * i + 1].val[0]);

    eH[i].val[0] = vzip2q_u64((uint64x2_t)bH[2 * i].val[1], (uint64x2_t)bH[2 * i + 1].val[1]);
    eH[i].val[1] = vzip1q_u64((uint64x2_t)bH[2 * i].val[1], (uint64x2_t)bH[2 * i + 1].val[1]);
  }
  //-------------------------------------
  for (i = 0; i < 2; i = i + 1) {
    fL[i].val[0] = vzip2q_u64((uint64x2_t)cL[2 * i].val[0], (uint64x2_t)cL[2 * i + 1].val[0]);
    fL[i].val[1] = vzip1q_u64((uint64x2_t)cL[2 * i].val[0], (uint64x2_t)cL[2 * i + 1].val[0]);

    fH[i].val[0] = vzip2q_u64((uint64x2_t)cL[2 * i].val[1], (uint64x2_t)cL[2 * i + 1].val[1]);
    fH[i].val[1] = vzip1q_u64((uint64x2_t)cL[2 * i].val[1], (uint64x2_t)cL[2 * i + 1].val[1]);
  }
  for (i = 0; i < 2; i = i + 1) {
    gL[i].val[0] = vzip2q_u64((uint64x2_t)cH[2 * i].val[0], (uint64x2_t)cH[2 * i + 1].val[0]);
    gL[i].val[1] = vzip1q_u64((uint64x2_t)cH[2 * i].val[0], (uint64x2_t)cH[2 * i + 1].val[0]);

    gH[i].val[0] = vzip2q_u64((uint64x2_t)cH[2 * i].val[1], (uint64x2_t)cH[2 * i + 1].val[1]);
    gH[i].val[1] = vzip1q_u64((uint64x2_t)cH[2 * i].val[1], (uint64x2_t)cH[2 * i + 1].val[1]);
  }
  PERMUTE0(M[0], dL[0], dL[1], 0x20);
  PERMUTE1(M[8], dL[0], dL[1], 0x31);
  PERMUTE0(M[1], dH[0], dH[1], 0x20);
  PERMUTE1(M[9], dH[0], dH[1], 0x31);

  PERMUTE0(M[2], eL[0], eL[1], 0x20);
  PERMUTE1(M[10], eL[0], eL[1], 0x31);
  PERMUTE0(M[3], eH[0], eH[1], 0x20);
  PERMUTE1(M[11], eH[0], eH[1], 0x31);

  PERMUTE0(M[4], fL[0], fL[1], 0x20);
  PERMUTE1(M[12], fL[0], fL[1], 0x31);
  PERMUTE0(M[5], fH[0], fH[1], 0x20);
  PERMUTE1(M[13], fH[0], fH[1], 0x31);

  PERMUTE0(M[6], gL[0], gL[1], 0x20);
  PERMUTE1(M[14], gL[0], gL[1], 0x31);
  PERMUTE0(M[7], gH[0], gH[1], 0x20);
  PERMUTE1(M[15], gH[0], gH[1], 0x31);
}
