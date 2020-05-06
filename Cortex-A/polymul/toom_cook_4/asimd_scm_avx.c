#include <arm_neon.h>

#define SCM_SIZE 16

uint16x8x2_t c_avx[2*SCM_SIZE]; 
uint16x8x2_t a[SCM_SIZE]; 
uint16x8x2_t b[SCM_SIZE]; 
uint16x8x2_t c_avx_extra[4];

/*
 * schoolbook_neon_new
 * Assume a, b are already loaded 
 */
void schoolbook_avx_new()
{
c_avx[0].val[0] = vmulq_u16(a[0].val[0], b[0].val[0]);
c_avx[0].val[1] = vmulq_u16(a[0].val[1], b[0].val[1]);
c_avx[1].val[0] = vmulq_u16(a[0].val[0], b[1].val[0]);
c_avx[1].val[1] = vmulq_u16(a[0].val[1], b[1].val[1]);
c_avx[1].val[0] = vmlaq_u16(c_avx[1].val[0], a[1].val[0], b[0].val[0]);
c_avx[1].val[1] = vmlaq_u16(c_avx[1].val[1], a[1].val[1], b[0].val[1]);
c_avx[2].val[0] = vmulq_u16(a[0].val[0], b[2].val[0]);
c_avx[2].val[1] = vmulq_u16(a[0].val[1], b[2].val[1]);
c_avx[2].val[0] = vmlaq_u16(c_avx[2].val[0], a[1].val[0], b[1].val[0]);
c_avx[2].val[1] = vmlaq_u16(c_avx[2].val[1], a[1].val[1], b[1].val[1]);
c_avx[2].val[0] = vmlaq_u16(c_avx[2].val[0], a[2].val[0], b[0].val[0]);
c_avx[2].val[1] = vmlaq_u16(c_avx[2].val[1], a[2].val[1], b[0].val[1]);
c_avx[3].val[0] = vmulq_u16(a[0].val[0], b[3].val[0]);
c_avx[3].val[1] = vmulq_u16(a[0].val[1], b[3].val[1]);
c_avx[3].val[0] = vmlaq_u16(c_avx[3].val[0], a[1].val[0], b[2].val[0]);
c_avx[3].val[1] = vmlaq_u16(c_avx[3].val[1], a[1].val[1], b[2].val[1]);
c_avx[3].val[0] = vmlaq_u16(c_avx[3].val[0], a[2].val[0], b[1].val[0]);
c_avx[3].val[1] = vmlaq_u16(c_avx[3].val[1], a[2].val[1], b[1].val[1]);
c_avx[3].val[0] = vmlaq_u16(c_avx[3].val[0], a[3].val[0], b[0].val[0]);
c_avx[3].val[1] = vmlaq_u16(c_avx[3].val[1], a[3].val[1], b[0].val[1]);
c_avx[4].val[0] = vmulq_u16(a[0].val[0], b[4].val[0]);
c_avx[4].val[1] = vmulq_u16(a[0].val[1], b[4].val[1]);
c_avx[4].val[0] = vmlaq_u16(c_avx[4].val[0], a[1].val[0], b[3].val[0]);
c_avx[4].val[1] = vmlaq_u16(c_avx[4].val[1], a[1].val[1], b[3].val[1]);
c_avx[4].val[0] = vmlaq_u16(c_avx[4].val[0], a[2].val[0], b[2].val[0]);
c_avx[4].val[1] = vmlaq_u16(c_avx[4].val[1], a[2].val[1], b[2].val[1]);
c_avx[4].val[0] = vmlaq_u16(c_avx[4].val[0], a[3].val[0], b[1].val[0]);
c_avx[4].val[1] = vmlaq_u16(c_avx[4].val[1], a[3].val[1], b[1].val[1]);
c_avx[4].val[0] = vmlaq_u16(c_avx[4].val[0], a[4].val[0], b[0].val[0]);
c_avx[4].val[1] = vmlaq_u16(c_avx[4].val[1], a[4].val[1], b[0].val[1]);
c_avx[5].val[0] = vmulq_u16(a[0].val[0], b[5].val[0]);
c_avx[5].val[1] = vmulq_u16(a[0].val[1], b[5].val[1]);
c_avx[5].val[0] = vmlaq_u16(c_avx[5].val[0], a[1].val[0], b[4].val[0]);
c_avx[5].val[1] = vmlaq_u16(c_avx[5].val[1], a[1].val[1], b[4].val[1]);
c_avx[5].val[0] = vmlaq_u16(c_avx[5].val[0], a[2].val[0], b[3].val[0]);
c_avx[5].val[1] = vmlaq_u16(c_avx[5].val[1], a[2].val[1], b[3].val[1]);
c_avx[5].val[0] = vmlaq_u16(c_avx[5].val[0], a[3].val[0], b[2].val[0]);
c_avx[5].val[1] = vmlaq_u16(c_avx[5].val[1], a[3].val[1], b[2].val[1]);
c_avx[5].val[0] = vmlaq_u16(c_avx[5].val[0], a[4].val[0], b[1].val[0]);
c_avx[5].val[1] = vmlaq_u16(c_avx[5].val[1], a[4].val[1], b[1].val[1]);
c_avx[5].val[0] = vmlaq_u16(c_avx[5].val[0], a[5].val[0], b[0].val[0]);
c_avx[5].val[1] = vmlaq_u16(c_avx[5].val[1], a[5].val[1], b[0].val[1]);
c_avx[6].val[0] = vmulq_u16(a[0].val[0], b[6].val[0]);
c_avx[6].val[1] = vmulq_u16(a[0].val[1], b[6].val[1]);
c_avx[6].val[0] = vmlaq_u16(c_avx[6].val[0], a[1].val[0], b[5].val[0]);
c_avx[6].val[1] = vmlaq_u16(c_avx[6].val[1], a[1].val[1], b[5].val[1]);
c_avx[6].val[0] = vmlaq_u16(c_avx[6].val[0], a[2].val[0], b[4].val[0]);
c_avx[6].val[1] = vmlaq_u16(c_avx[6].val[1], a[2].val[1], b[4].val[1]);
c_avx[6].val[0] = vmlaq_u16(c_avx[6].val[0], a[3].val[0], b[3].val[0]);
c_avx[6].val[1] = vmlaq_u16(c_avx[6].val[1], a[3].val[1], b[3].val[1]);
c_avx[6].val[0] = vmlaq_u16(c_avx[6].val[0], a[4].val[0], b[2].val[0]);
c_avx[6].val[1] = vmlaq_u16(c_avx[6].val[1], a[4].val[1], b[2].val[1]);
c_avx[6].val[0] = vmlaq_u16(c_avx[6].val[0], a[5].val[0], b[1].val[0]);
c_avx[6].val[1] = vmlaq_u16(c_avx[6].val[1], a[5].val[1], b[1].val[1]);
c_avx[6].val[0] = vmlaq_u16(c_avx[6].val[0], a[6].val[0], b[0].val[0]);
c_avx[6].val[1] = vmlaq_u16(c_avx[6].val[1], a[6].val[1], b[0].val[1]);
c_avx[7].val[0] = vmulq_u16(a[0].val[0], b[7].val[0]);
c_avx[7].val[1] = vmulq_u16(a[0].val[1], b[7].val[1]);
c_avx[7].val[0] = vmlaq_u16(c_avx[7].val[0], a[1].val[0], b[6].val[0]);
c_avx[7].val[1] = vmlaq_u16(c_avx[7].val[1], a[1].val[1], b[6].val[1]);
c_avx[7].val[0] = vmlaq_u16(c_avx[7].val[0], a[2].val[0], b[5].val[0]);
c_avx[7].val[1] = vmlaq_u16(c_avx[7].val[1], a[2].val[1], b[5].val[1]);
c_avx[7].val[0] = vmlaq_u16(c_avx[7].val[0], a[3].val[0], b[4].val[0]);
c_avx[7].val[1] = vmlaq_u16(c_avx[7].val[1], a[3].val[1], b[4].val[1]);
c_avx[7].val[0] = vmlaq_u16(c_avx[7].val[0], a[4].val[0], b[3].val[0]);
c_avx[7].val[1] = vmlaq_u16(c_avx[7].val[1], a[4].val[1], b[3].val[1]);
c_avx[7].val[0] = vmlaq_u16(c_avx[7].val[0], a[5].val[0], b[2].val[0]);
c_avx[7].val[1] = vmlaq_u16(c_avx[7].val[1], a[5].val[1], b[2].val[1]);
c_avx[7].val[0] = vmlaq_u16(c_avx[7].val[0], a[6].val[0], b[1].val[0]);
c_avx[7].val[1] = vmlaq_u16(c_avx[7].val[1], a[6].val[1], b[1].val[1]);
c_avx[7].val[0] = vmlaq_u16(c_avx[7].val[0], a[7].val[0], b[0].val[0]);
c_avx[7].val[1] = vmlaq_u16(c_avx[7].val[1], a[7].val[1], b[0].val[1]);
c_avx[8].val[0] = vmulq_u16(a[0].val[0], b[8].val[0]);
c_avx[8].val[1] = vmulq_u16(a[0].val[1], b[8].val[1]);
c_avx[8].val[0] = vmlaq_u16(c_avx[8].val[0], a[1].val[0], b[7].val[0]);
c_avx[8].val[1] = vmlaq_u16(c_avx[8].val[1], a[1].val[1], b[7].val[1]);
c_avx[8].val[0] = vmlaq_u16(c_avx[8].val[0], a[2].val[0], b[6].val[0]);
c_avx[8].val[1] = vmlaq_u16(c_avx[8].val[1], a[2].val[1], b[6].val[1]);
c_avx[8].val[0] = vmlaq_u16(c_avx[8].val[0], a[3].val[0], b[5].val[0]);
c_avx[8].val[1] = vmlaq_u16(c_avx[8].val[1], a[3].val[1], b[5].val[1]);
c_avx[8].val[0] = vmlaq_u16(c_avx[8].val[0], a[4].val[0], b[4].val[0]);
c_avx[8].val[1] = vmlaq_u16(c_avx[8].val[1], a[4].val[1], b[4].val[1]);
c_avx[8].val[0] = vmlaq_u16(c_avx[8].val[0], a[5].val[0], b[3].val[0]);
c_avx[8].val[1] = vmlaq_u16(c_avx[8].val[1], a[5].val[1], b[3].val[1]);
c_avx[8].val[0] = vmlaq_u16(c_avx[8].val[0], a[6].val[0], b[2].val[0]);
c_avx[8].val[1] = vmlaq_u16(c_avx[8].val[1], a[6].val[1], b[2].val[1]);
c_avx[8].val[0] = vmlaq_u16(c_avx[8].val[0], a[7].val[0], b[1].val[0]);
c_avx[8].val[1] = vmlaq_u16(c_avx[8].val[1], a[7].val[1], b[1].val[1]);
c_avx[8].val[0] = vmlaq_u16(c_avx[8].val[0], a[8].val[0], b[0].val[0]);
c_avx[8].val[1] = vmlaq_u16(c_avx[8].val[1], a[8].val[1], b[0].val[1]);
c_avx[9].val[0] = vmulq_u16(a[0].val[0], b[9].val[0]);
c_avx[9].val[1] = vmulq_u16(a[0].val[1], b[9].val[1]);
c_avx[9].val[0] = vmlaq_u16(c_avx[9].val[0], a[1].val[0], b[8].val[0]);
c_avx[9].val[1] = vmlaq_u16(c_avx[9].val[1], a[1].val[1], b[8].val[1]);
c_avx[9].val[0] = vmlaq_u16(c_avx[9].val[0], a[2].val[0], b[7].val[0]);
c_avx[9].val[1] = vmlaq_u16(c_avx[9].val[1], a[2].val[1], b[7].val[1]);
c_avx[9].val[0] = vmlaq_u16(c_avx[9].val[0], a[3].val[0], b[6].val[0]);
c_avx[9].val[1] = vmlaq_u16(c_avx[9].val[1], a[3].val[1], b[6].val[1]);
c_avx[9].val[0] = vmlaq_u16(c_avx[9].val[0], a[4].val[0], b[5].val[0]);
c_avx[9].val[1] = vmlaq_u16(c_avx[9].val[1], a[4].val[1], b[5].val[1]);
c_avx[9].val[0] = vmlaq_u16(c_avx[9].val[0], a[5].val[0], b[4].val[0]);
c_avx[9].val[1] = vmlaq_u16(c_avx[9].val[1], a[5].val[1], b[4].val[1]);
c_avx[9].val[0] = vmlaq_u16(c_avx[9].val[0], a[6].val[0], b[3].val[0]);
c_avx[9].val[1] = vmlaq_u16(c_avx[9].val[1], a[6].val[1], b[3].val[1]);
c_avx[9].val[0] = vmlaq_u16(c_avx[9].val[0], a[7].val[0], b[2].val[0]);
c_avx[9].val[1] = vmlaq_u16(c_avx[9].val[1], a[7].val[1], b[2].val[1]);
c_avx[9].val[0] = vmlaq_u16(c_avx[9].val[0], a[8].val[0], b[1].val[0]);
c_avx[9].val[1] = vmlaq_u16(c_avx[9].val[1], a[8].val[1], b[1].val[1]);
c_avx[9].val[0] = vmlaq_u16(c_avx[9].val[0], a[9].val[0], b[0].val[0]);
c_avx[9].val[1] = vmlaq_u16(c_avx[9].val[1], a[9].val[1], b[0].val[1]);
c_avx[10].val[0] = vmulq_u16(a[0].val[0], b[10].val[0]);
c_avx[10].val[1] = vmulq_u16(a[0].val[1], b[10].val[1]);
c_avx[10].val[0] = vmlaq_u16(c_avx[10].val[0], a[1].val[0], b[9].val[0]);
c_avx[10].val[1] = vmlaq_u16(c_avx[10].val[1], a[1].val[1], b[9].val[1]);
c_avx[10].val[0] = vmlaq_u16(c_avx[10].val[0], a[2].val[0], b[8].val[0]);
c_avx[10].val[1] = vmlaq_u16(c_avx[10].val[1], a[2].val[1], b[8].val[1]);
c_avx[10].val[0] = vmlaq_u16(c_avx[10].val[0], a[3].val[0], b[7].val[0]);
c_avx[10].val[1] = vmlaq_u16(c_avx[10].val[1], a[3].val[1], b[7].val[1]);
c_avx[10].val[0] = vmlaq_u16(c_avx[10].val[0], a[4].val[0], b[6].val[0]);
c_avx[10].val[1] = vmlaq_u16(c_avx[10].val[1], a[4].val[1], b[6].val[1]);
c_avx[10].val[0] = vmlaq_u16(c_avx[10].val[0], a[5].val[0], b[5].val[0]);
c_avx[10].val[1] = vmlaq_u16(c_avx[10].val[1], a[5].val[1], b[5].val[1]);
c_avx[10].val[0] = vmlaq_u16(c_avx[10].val[0], a[6].val[0], b[4].val[0]);
c_avx[10].val[1] = vmlaq_u16(c_avx[10].val[1], a[6].val[1], b[4].val[1]);
c_avx[10].val[0] = vmlaq_u16(c_avx[10].val[0], a[7].val[0], b[3].val[0]);
c_avx[10].val[1] = vmlaq_u16(c_avx[10].val[1], a[7].val[1], b[3].val[1]);
c_avx[10].val[0] = vmlaq_u16(c_avx[10].val[0], a[8].val[0], b[2].val[0]);
c_avx[10].val[1] = vmlaq_u16(c_avx[10].val[1], a[8].val[1], b[2].val[1]);
c_avx[10].val[0] = vmlaq_u16(c_avx[10].val[0], a[9].val[0], b[1].val[0]);
c_avx[10].val[1] = vmlaq_u16(c_avx[10].val[1], a[9].val[1], b[1].val[1]);
c_avx[10].val[0] = vmlaq_u16(c_avx[10].val[0], a[10].val[0], b[0].val[0]);
c_avx[10].val[1] = vmlaq_u16(c_avx[10].val[1], a[10].val[1], b[0].val[1]);
c_avx[11].val[0] = vmulq_u16(a[0].val[0], b[11].val[0]);
c_avx[11].val[1] = vmulq_u16(a[0].val[1], b[11].val[1]);
c_avx[11].val[0] = vmlaq_u16(c_avx[11].val[0], a[1].val[0], b[10].val[0]);
c_avx[11].val[1] = vmlaq_u16(c_avx[11].val[1], a[1].val[1], b[10].val[1]);
c_avx[11].val[0] = vmlaq_u16(c_avx[11].val[0], a[2].val[0], b[9].val[0]);
c_avx[11].val[1] = vmlaq_u16(c_avx[11].val[1], a[2].val[1], b[9].val[1]);
c_avx[11].val[0] = vmlaq_u16(c_avx[11].val[0], a[3].val[0], b[8].val[0]);
c_avx[11].val[1] = vmlaq_u16(c_avx[11].val[1], a[3].val[1], b[8].val[1]);
c_avx[11].val[0] = vmlaq_u16(c_avx[11].val[0], a[4].val[0], b[7].val[0]);
c_avx[11].val[1] = vmlaq_u16(c_avx[11].val[1], a[4].val[1], b[7].val[1]);
c_avx[11].val[0] = vmlaq_u16(c_avx[11].val[0], a[5].val[0], b[6].val[0]);
c_avx[11].val[1] = vmlaq_u16(c_avx[11].val[1], a[5].val[1], b[6].val[1]);
c_avx[11].val[0] = vmlaq_u16(c_avx[11].val[0], a[6].val[0], b[5].val[0]);
c_avx[11].val[1] = vmlaq_u16(c_avx[11].val[1], a[6].val[1], b[5].val[1]);
c_avx[11].val[0] = vmlaq_u16(c_avx[11].val[0], a[7].val[0], b[4].val[0]);
c_avx[11].val[1] = vmlaq_u16(c_avx[11].val[1], a[7].val[1], b[4].val[1]);
c_avx[11].val[0] = vmlaq_u16(c_avx[11].val[0], a[8].val[0], b[3].val[0]);
c_avx[11].val[1] = vmlaq_u16(c_avx[11].val[1], a[8].val[1], b[3].val[1]);
c_avx[11].val[0] = vmlaq_u16(c_avx[11].val[0], a[9].val[0], b[2].val[0]);
c_avx[11].val[1] = vmlaq_u16(c_avx[11].val[1], a[9].val[1], b[2].val[1]);
c_avx[11].val[0] = vmlaq_u16(c_avx[11].val[0], a[10].val[0], b[1].val[0]);
c_avx[11].val[1] = vmlaq_u16(c_avx[11].val[1], a[10].val[1], b[1].val[1]);
c_avx[11].val[0] = vmlaq_u16(c_avx[11].val[0], a[11].val[0], b[0].val[0]);
c_avx[11].val[1] = vmlaq_u16(c_avx[11].val[1], a[11].val[1], b[0].val[1]);
c_avx[12].val[0] = vmulq_u16(a[0].val[0], b[12].val[0]);
c_avx[12].val[1] = vmulq_u16(a[0].val[1], b[12].val[1]);
c_avx[12].val[0] = vmlaq_u16(c_avx[12].val[0], a[1].val[0], b[11].val[0]);
c_avx[12].val[1] = vmlaq_u16(c_avx[12].val[1], a[1].val[1], b[11].val[1]);
c_avx[12].val[0] = vmlaq_u16(c_avx[12].val[0], a[2].val[0], b[10].val[0]);
c_avx[12].val[1] = vmlaq_u16(c_avx[12].val[1], a[2].val[1], b[10].val[1]);
c_avx[12].val[0] = vmlaq_u16(c_avx[12].val[0], a[3].val[0], b[9].val[0]);
c_avx[12].val[1] = vmlaq_u16(c_avx[12].val[1], a[3].val[1], b[9].val[1]);
c_avx[12].val[0] = vmlaq_u16(c_avx[12].val[0], a[4].val[0], b[8].val[0]);
c_avx[12].val[1] = vmlaq_u16(c_avx[12].val[1], a[4].val[1], b[8].val[1]);
c_avx[12].val[0] = vmlaq_u16(c_avx[12].val[0], a[5].val[0], b[7].val[0]);
c_avx[12].val[1] = vmlaq_u16(c_avx[12].val[1], a[5].val[1], b[7].val[1]);
c_avx[12].val[0] = vmlaq_u16(c_avx[12].val[0], a[6].val[0], b[6].val[0]);
c_avx[12].val[1] = vmlaq_u16(c_avx[12].val[1], a[6].val[1], b[6].val[1]);
c_avx[12].val[0] = vmlaq_u16(c_avx[12].val[0], a[7].val[0], b[5].val[0]);
c_avx[12].val[1] = vmlaq_u16(c_avx[12].val[1], a[7].val[1], b[5].val[1]);
c_avx[12].val[0] = vmlaq_u16(c_avx[12].val[0], a[8].val[0], b[4].val[0]);
c_avx[12].val[1] = vmlaq_u16(c_avx[12].val[1], a[8].val[1], b[4].val[1]);
c_avx[12].val[0] = vmlaq_u16(c_avx[12].val[0], a[9].val[0], b[3].val[0]);
c_avx[12].val[1] = vmlaq_u16(c_avx[12].val[1], a[9].val[1], b[3].val[1]);
c_avx[12].val[0] = vmlaq_u16(c_avx[12].val[0], a[10].val[0], b[2].val[0]);
c_avx[12].val[1] = vmlaq_u16(c_avx[12].val[1], a[10].val[1], b[2].val[1]);
c_avx[12].val[0] = vmlaq_u16(c_avx[12].val[0], a[11].val[0], b[1].val[0]);
c_avx[12].val[1] = vmlaq_u16(c_avx[12].val[1], a[11].val[1], b[1].val[1]);
c_avx[12].val[0] = vmlaq_u16(c_avx[12].val[0], a[12].val[0], b[0].val[0]);
c_avx[12].val[1] = vmlaq_u16(c_avx[12].val[1], a[12].val[1], b[0].val[1]);
c_avx[13].val[0] = vmulq_u16(a[0].val[0], b[13].val[0]);
c_avx[13].val[1] = vmulq_u16(a[0].val[1], b[13].val[1]);
c_avx[13].val[0] = vmlaq_u16(c_avx[13].val[0], a[1].val[0], b[12].val[0]);
c_avx[13].val[1] = vmlaq_u16(c_avx[13].val[1], a[1].val[1], b[12].val[1]);
c_avx[13].val[0] = vmlaq_u16(c_avx[13].val[0], a[2].val[0], b[11].val[0]);
c_avx[13].val[1] = vmlaq_u16(c_avx[13].val[1], a[2].val[1], b[11].val[1]);
c_avx[13].val[0] = vmlaq_u16(c_avx[13].val[0], a[3].val[0], b[10].val[0]);
c_avx[13].val[1] = vmlaq_u16(c_avx[13].val[1], a[3].val[1], b[10].val[1]);
c_avx[13].val[0] = vmlaq_u16(c_avx[13].val[0], a[4].val[0], b[9].val[0]);
c_avx[13].val[1] = vmlaq_u16(c_avx[13].val[1], a[4].val[1], b[9].val[1]);
c_avx[13].val[0] = vmlaq_u16(c_avx[13].val[0], a[5].val[0], b[8].val[0]);
c_avx[13].val[1] = vmlaq_u16(c_avx[13].val[1], a[5].val[1], b[8].val[1]);
c_avx[13].val[0] = vmlaq_u16(c_avx[13].val[0], a[6].val[0], b[7].val[0]);
c_avx[13].val[1] = vmlaq_u16(c_avx[13].val[1], a[6].val[1], b[7].val[1]);
c_avx[13].val[0] = vmlaq_u16(c_avx[13].val[0], a[7].val[0], b[6].val[0]);
c_avx[13].val[1] = vmlaq_u16(c_avx[13].val[1], a[7].val[1], b[6].val[1]);
c_avx[13].val[0] = vmlaq_u16(c_avx[13].val[0], a[8].val[0], b[5].val[0]);
c_avx[13].val[1] = vmlaq_u16(c_avx[13].val[1], a[8].val[1], b[5].val[1]);
c_avx[13].val[0] = vmlaq_u16(c_avx[13].val[0], a[9].val[0], b[4].val[0]);
c_avx[13].val[1] = vmlaq_u16(c_avx[13].val[1], a[9].val[1], b[4].val[1]);
c_avx[13].val[0] = vmlaq_u16(c_avx[13].val[0], a[10].val[0], b[3].val[0]);
c_avx[13].val[1] = vmlaq_u16(c_avx[13].val[1], a[10].val[1], b[3].val[1]);
c_avx[13].val[0] = vmlaq_u16(c_avx[13].val[0], a[11].val[0], b[2].val[0]);
c_avx[13].val[1] = vmlaq_u16(c_avx[13].val[1], a[11].val[1], b[2].val[1]);
c_avx[13].val[0] = vmlaq_u16(c_avx[13].val[0], a[12].val[0], b[1].val[0]);
c_avx[13].val[1] = vmlaq_u16(c_avx[13].val[1], a[12].val[1], b[1].val[1]);
c_avx[13].val[0] = vmlaq_u16(c_avx[13].val[0], a[13].val[0], b[0].val[0]);
c_avx[13].val[1] = vmlaq_u16(c_avx[13].val[1], a[13].val[1], b[0].val[1]);
c_avx[14].val[0] = vmulq_u16(a[0].val[0], b[14].val[0]);
c_avx[14].val[1] = vmulq_u16(a[0].val[1], b[14].val[1]);
c_avx[14].val[0] = vmlaq_u16(c_avx[14].val[0], a[1].val[0], b[13].val[0]);
c_avx[14].val[1] = vmlaq_u16(c_avx[14].val[1], a[1].val[1], b[13].val[1]);
c_avx[14].val[0] = vmlaq_u16(c_avx[14].val[0], a[2].val[0], b[12].val[0]);
c_avx[14].val[1] = vmlaq_u16(c_avx[14].val[1], a[2].val[1], b[12].val[1]);
c_avx[14].val[0] = vmlaq_u16(c_avx[14].val[0], a[3].val[0], b[11].val[0]);
c_avx[14].val[1] = vmlaq_u16(c_avx[14].val[1], a[3].val[1], b[11].val[1]);
c_avx[14].val[0] = vmlaq_u16(c_avx[14].val[0], a[4].val[0], b[10].val[0]);
c_avx[14].val[1] = vmlaq_u16(c_avx[14].val[1], a[4].val[1], b[10].val[1]);
c_avx[14].val[0] = vmlaq_u16(c_avx[14].val[0], a[5].val[0], b[9].val[0]);
c_avx[14].val[1] = vmlaq_u16(c_avx[14].val[1], a[5].val[1], b[9].val[1]);
c_avx[14].val[0] = vmlaq_u16(c_avx[14].val[0], a[6].val[0], b[8].val[0]);
c_avx[14].val[1] = vmlaq_u16(c_avx[14].val[1], a[6].val[1], b[8].val[1]);
c_avx[14].val[0] = vmlaq_u16(c_avx[14].val[0], a[7].val[0], b[7].val[0]);
c_avx[14].val[1] = vmlaq_u16(c_avx[14].val[1], a[7].val[1], b[7].val[1]);
c_avx[14].val[0] = vmlaq_u16(c_avx[14].val[0], a[8].val[0], b[6].val[0]);
c_avx[14].val[1] = vmlaq_u16(c_avx[14].val[1], a[8].val[1], b[6].val[1]);
c_avx[14].val[0] = vmlaq_u16(c_avx[14].val[0], a[9].val[0], b[5].val[0]);
c_avx[14].val[1] = vmlaq_u16(c_avx[14].val[1], a[9].val[1], b[5].val[1]);
c_avx[14].val[0] = vmlaq_u16(c_avx[14].val[0], a[10].val[0], b[4].val[0]);
c_avx[14].val[1] = vmlaq_u16(c_avx[14].val[1], a[10].val[1], b[4].val[1]);
c_avx[14].val[0] = vmlaq_u16(c_avx[14].val[0], a[11].val[0], b[3].val[0]);
c_avx[14].val[1] = vmlaq_u16(c_avx[14].val[1], a[11].val[1], b[3].val[1]);
c_avx[14].val[0] = vmlaq_u16(c_avx[14].val[0], a[12].val[0], b[2].val[0]);
c_avx[14].val[1] = vmlaq_u16(c_avx[14].val[1], a[12].val[1], b[2].val[1]);
c_avx[14].val[0] = vmlaq_u16(c_avx[14].val[0], a[13].val[0], b[1].val[0]);
c_avx[14].val[1] = vmlaq_u16(c_avx[14].val[1], a[13].val[1], b[1].val[1]);
c_avx[14].val[0] = vmlaq_u16(c_avx[14].val[0], a[14].val[0], b[0].val[0]);
c_avx[14].val[1] = vmlaq_u16(c_avx[14].val[1], a[14].val[1], b[0].val[1]);
c_avx[15].val[0] = vmulq_u16(a[0].val[0], b[15].val[0]);
c_avx[15].val[1] = vmulq_u16(a[0].val[1], b[15].val[1]);
c_avx[15].val[0] = vmlaq_u16(c_avx[15].val[0], a[1].val[0], b[14].val[0]);
c_avx[15].val[1] = vmlaq_u16(c_avx[15].val[1], a[1].val[1], b[14].val[1]);
c_avx[15].val[0] = vmlaq_u16(c_avx[15].val[0], a[2].val[0], b[13].val[0]);
c_avx[15].val[1] = vmlaq_u16(c_avx[15].val[1], a[2].val[1], b[13].val[1]);
c_avx[15].val[0] = vmlaq_u16(c_avx[15].val[0], a[3].val[0], b[12].val[0]);
c_avx[15].val[1] = vmlaq_u16(c_avx[15].val[1], a[3].val[1], b[12].val[1]);
c_avx[15].val[0] = vmlaq_u16(c_avx[15].val[0], a[4].val[0], b[11].val[0]);
c_avx[15].val[1] = vmlaq_u16(c_avx[15].val[1], a[4].val[1], b[11].val[1]);
c_avx[15].val[0] = vmlaq_u16(c_avx[15].val[0], a[5].val[0], b[10].val[0]);
c_avx[15].val[1] = vmlaq_u16(c_avx[15].val[1], a[5].val[1], b[10].val[1]);
c_avx[15].val[0] = vmlaq_u16(c_avx[15].val[0], a[6].val[0], b[9].val[0]);
c_avx[15].val[1] = vmlaq_u16(c_avx[15].val[1], a[6].val[1], b[9].val[1]);
c_avx[15].val[0] = vmlaq_u16(c_avx[15].val[0], a[7].val[0], b[8].val[0]);
c_avx[15].val[1] = vmlaq_u16(c_avx[15].val[1], a[7].val[1], b[8].val[1]);
c_avx[15].val[0] = vmlaq_u16(c_avx[15].val[0], a[8].val[0], b[7].val[0]);
c_avx[15].val[1] = vmlaq_u16(c_avx[15].val[1], a[8].val[1], b[7].val[1]);
c_avx[15].val[0] = vmlaq_u16(c_avx[15].val[0], a[9].val[0], b[6].val[0]);
c_avx[15].val[1] = vmlaq_u16(c_avx[15].val[1], a[9].val[1], b[6].val[1]);
c_avx[15].val[0] = vmlaq_u16(c_avx[15].val[0], a[10].val[0], b[5].val[0]);
c_avx[15].val[1] = vmlaq_u16(c_avx[15].val[1], a[10].val[1], b[5].val[1]);
c_avx[15].val[0] = vmlaq_u16(c_avx[15].val[0], a[11].val[0], b[4].val[0]);
c_avx[15].val[1] = vmlaq_u16(c_avx[15].val[1], a[11].val[1], b[4].val[1]);
c_avx[15].val[0] = vmlaq_u16(c_avx[15].val[0], a[12].val[0], b[3].val[0]);
c_avx[15].val[1] = vmlaq_u16(c_avx[15].val[1], a[12].val[1], b[3].val[1]);
c_avx[15].val[0] = vmlaq_u16(c_avx[15].val[0], a[13].val[0], b[2].val[0]);
c_avx[15].val[1] = vmlaq_u16(c_avx[15].val[1], a[13].val[1], b[2].val[1]);
c_avx[15].val[0] = vmlaq_u16(c_avx[15].val[0], a[14].val[0], b[1].val[0]);
c_avx[15].val[1] = vmlaq_u16(c_avx[15].val[1], a[14].val[1], b[1].val[1]);
c_avx[15].val[0] = vmlaq_u16(c_avx[15].val[0], a[15].val[0], b[0].val[0]);
c_avx[15].val[1] = vmlaq_u16(c_avx[15].val[1], a[15].val[1], b[0].val[1]);
c_avx[16].val[0] = vmulq_u16(a[1].val[0], b[15].val[0]);
c_avx[16].val[1] = vmulq_u16(a[1].val[1], b[15].val[1]);
c_avx[16].val[0] = vmlaq_u16(c_avx[16].val[0], a[2].val[0], b[14].val[0]);
c_avx[16].val[1] = vmlaq_u16(c_avx[16].val[1], a[2].val[1], b[14].val[1]);
c_avx[16].val[0] = vmlaq_u16(c_avx[16].val[0], a[3].val[0], b[13].val[0]);
c_avx[16].val[1] = vmlaq_u16(c_avx[16].val[1], a[3].val[1], b[13].val[1]);
c_avx[16].val[0] = vmlaq_u16(c_avx[16].val[0], a[4].val[0], b[12].val[0]);
c_avx[16].val[1] = vmlaq_u16(c_avx[16].val[1], a[4].val[1], b[12].val[1]);
c_avx[16].val[0] = vmlaq_u16(c_avx[16].val[0], a[5].val[0], b[11].val[0]);
c_avx[16].val[1] = vmlaq_u16(c_avx[16].val[1], a[5].val[1], b[11].val[1]);
c_avx[16].val[0] = vmlaq_u16(c_avx[16].val[0], a[6].val[0], b[10].val[0]);
c_avx[16].val[1] = vmlaq_u16(c_avx[16].val[1], a[6].val[1], b[10].val[1]);
c_avx[16].val[0] = vmlaq_u16(c_avx[16].val[0], a[7].val[0], b[9].val[0]);
c_avx[16].val[1] = vmlaq_u16(c_avx[16].val[1], a[7].val[1], b[9].val[1]);
c_avx[16].val[0] = vmlaq_u16(c_avx[16].val[0], a[8].val[0], b[8].val[0]);
c_avx[16].val[1] = vmlaq_u16(c_avx[16].val[1], a[8].val[1], b[8].val[1]);
c_avx[16].val[0] = vmlaq_u16(c_avx[16].val[0], a[9].val[0], b[7].val[0]);
c_avx[16].val[1] = vmlaq_u16(c_avx[16].val[1], a[9].val[1], b[7].val[1]);
c_avx[16].val[0] = vmlaq_u16(c_avx[16].val[0], a[10].val[0], b[6].val[0]);
c_avx[16].val[1] = vmlaq_u16(c_avx[16].val[1], a[10].val[1], b[6].val[1]);
c_avx[16].val[0] = vmlaq_u16(c_avx[16].val[0], a[11].val[0], b[5].val[0]);
c_avx[16].val[1] = vmlaq_u16(c_avx[16].val[1], a[11].val[1], b[5].val[1]);
c_avx[16].val[0] = vmlaq_u16(c_avx[16].val[0], a[12].val[0], b[4].val[0]);
c_avx[16].val[1] = vmlaq_u16(c_avx[16].val[1], a[12].val[1], b[4].val[1]);
c_avx[16].val[0] = vmlaq_u16(c_avx[16].val[0], a[13].val[0], b[3].val[0]);
c_avx[16].val[1] = vmlaq_u16(c_avx[16].val[1], a[13].val[1], b[3].val[1]);
c_avx[16].val[0] = vmlaq_u16(c_avx[16].val[0], a[14].val[0], b[2].val[0]);
c_avx[16].val[1] = vmlaq_u16(c_avx[16].val[1], a[14].val[1], b[2].val[1]);
c_avx[16].val[0] = vmlaq_u16(c_avx[16].val[0], a[15].val[0], b[1].val[0]);
c_avx[16].val[1] = vmlaq_u16(c_avx[16].val[1], a[15].val[1], b[1].val[1]);
c_avx[17].val[0] = vmulq_u16(a[2].val[0], b[15].val[0]);
c_avx[17].val[1] = vmulq_u16(a[2].val[1], b[15].val[1]);
c_avx[17].val[0] = vmlaq_u16(c_avx[17].val[0], a[3].val[0], b[14].val[0]);
c_avx[17].val[1] = vmlaq_u16(c_avx[17].val[1], a[3].val[1], b[14].val[1]);
c_avx[17].val[0] = vmlaq_u16(c_avx[17].val[0], a[4].val[0], b[13].val[0]);
c_avx[17].val[1] = vmlaq_u16(c_avx[17].val[1], a[4].val[1], b[13].val[1]);
c_avx[17].val[0] = vmlaq_u16(c_avx[17].val[0], a[5].val[0], b[12].val[0]);
c_avx[17].val[1] = vmlaq_u16(c_avx[17].val[1], a[5].val[1], b[12].val[1]);
c_avx[17].val[0] = vmlaq_u16(c_avx[17].val[0], a[6].val[0], b[11].val[0]);
c_avx[17].val[1] = vmlaq_u16(c_avx[17].val[1], a[6].val[1], b[11].val[1]);
c_avx[17].val[0] = vmlaq_u16(c_avx[17].val[0], a[7].val[0], b[10].val[0]);
c_avx[17].val[1] = vmlaq_u16(c_avx[17].val[1], a[7].val[1], b[10].val[1]);
c_avx[17].val[0] = vmlaq_u16(c_avx[17].val[0], a[8].val[0], b[9].val[0]);
c_avx[17].val[1] = vmlaq_u16(c_avx[17].val[1], a[8].val[1], b[9].val[1]);
c_avx[17].val[0] = vmlaq_u16(c_avx[17].val[0], a[9].val[0], b[8].val[0]);
c_avx[17].val[1] = vmlaq_u16(c_avx[17].val[1], a[9].val[1], b[8].val[1]);
c_avx[17].val[0] = vmlaq_u16(c_avx[17].val[0], a[10].val[0], b[7].val[0]);
c_avx[17].val[1] = vmlaq_u16(c_avx[17].val[1], a[10].val[1], b[7].val[1]);
c_avx[17].val[0] = vmlaq_u16(c_avx[17].val[0], a[11].val[0], b[6].val[0]);
c_avx[17].val[1] = vmlaq_u16(c_avx[17].val[1], a[11].val[1], b[6].val[1]);
c_avx[17].val[0] = vmlaq_u16(c_avx[17].val[0], a[12].val[0], b[5].val[0]);
c_avx[17].val[1] = vmlaq_u16(c_avx[17].val[1], a[12].val[1], b[5].val[1]);
c_avx[17].val[0] = vmlaq_u16(c_avx[17].val[0], a[13].val[0], b[4].val[0]);
c_avx[17].val[1] = vmlaq_u16(c_avx[17].val[1], a[13].val[1], b[4].val[1]);
c_avx[17].val[0] = vmlaq_u16(c_avx[17].val[0], a[14].val[0], b[3].val[0]);
c_avx[17].val[1] = vmlaq_u16(c_avx[17].val[1], a[14].val[1], b[3].val[1]);
c_avx[17].val[0] = vmlaq_u16(c_avx[17].val[0], a[15].val[0], b[2].val[0]);
c_avx[17].val[1] = vmlaq_u16(c_avx[17].val[1], a[15].val[1], b[2].val[1]);
c_avx[18].val[0] = vmulq_u16(a[3].val[0], b[15].val[0]);
c_avx[18].val[1] = vmulq_u16(a[3].val[1], b[15].val[1]);
c_avx[18].val[0] = vmlaq_u16(c_avx[18].val[0], a[4].val[0], b[14].val[0]);
c_avx[18].val[1] = vmlaq_u16(c_avx[18].val[1], a[4].val[1], b[14].val[1]);
c_avx[18].val[0] = vmlaq_u16(c_avx[18].val[0], a[5].val[0], b[13].val[0]);
c_avx[18].val[1] = vmlaq_u16(c_avx[18].val[1], a[5].val[1], b[13].val[1]);
c_avx[18].val[0] = vmlaq_u16(c_avx[18].val[0], a[6].val[0], b[12].val[0]);
c_avx[18].val[1] = vmlaq_u16(c_avx[18].val[1], a[6].val[1], b[12].val[1]);
c_avx[18].val[0] = vmlaq_u16(c_avx[18].val[0], a[7].val[0], b[11].val[0]);
c_avx[18].val[1] = vmlaq_u16(c_avx[18].val[1], a[7].val[1], b[11].val[1]);
c_avx[18].val[0] = vmlaq_u16(c_avx[18].val[0], a[8].val[0], b[10].val[0]);
c_avx[18].val[1] = vmlaq_u16(c_avx[18].val[1], a[8].val[1], b[10].val[1]);
c_avx[18].val[0] = vmlaq_u16(c_avx[18].val[0], a[9].val[0], b[9].val[0]);
c_avx[18].val[1] = vmlaq_u16(c_avx[18].val[1], a[9].val[1], b[9].val[1]);
c_avx[18].val[0] = vmlaq_u16(c_avx[18].val[0], a[10].val[0], b[8].val[0]);
c_avx[18].val[1] = vmlaq_u16(c_avx[18].val[1], a[10].val[1], b[8].val[1]);
c_avx[18].val[0] = vmlaq_u16(c_avx[18].val[0], a[11].val[0], b[7].val[0]);
c_avx[18].val[1] = vmlaq_u16(c_avx[18].val[1], a[11].val[1], b[7].val[1]);
c_avx[18].val[0] = vmlaq_u16(c_avx[18].val[0], a[12].val[0], b[6].val[0]);
c_avx[18].val[1] = vmlaq_u16(c_avx[18].val[1], a[12].val[1], b[6].val[1]);
c_avx[18].val[0] = vmlaq_u16(c_avx[18].val[0], a[13].val[0], b[5].val[0]);
c_avx[18].val[1] = vmlaq_u16(c_avx[18].val[1], a[13].val[1], b[5].val[1]);
c_avx[18].val[0] = vmlaq_u16(c_avx[18].val[0], a[14].val[0], b[4].val[0]);
c_avx[18].val[1] = vmlaq_u16(c_avx[18].val[1], a[14].val[1], b[4].val[1]);
c_avx[18].val[0] = vmlaq_u16(c_avx[18].val[0], a[15].val[0], b[3].val[0]);
c_avx[18].val[1] = vmlaq_u16(c_avx[18].val[1], a[15].val[1], b[3].val[1]);
c_avx[19].val[0] = vmulq_u16(a[4].val[0], b[15].val[0]);
c_avx[19].val[1] = vmulq_u16(a[4].val[1], b[15].val[1]);
c_avx[19].val[0] = vmlaq_u16(c_avx[19].val[0], a[5].val[0], b[14].val[0]);
c_avx[19].val[1] = vmlaq_u16(c_avx[19].val[1], a[5].val[1], b[14].val[1]);
c_avx[19].val[0] = vmlaq_u16(c_avx[19].val[0], a[6].val[0], b[13].val[0]);
c_avx[19].val[1] = vmlaq_u16(c_avx[19].val[1], a[6].val[1], b[13].val[1]);
c_avx[19].val[0] = vmlaq_u16(c_avx[19].val[0], a[7].val[0], b[12].val[0]);
c_avx[19].val[1] = vmlaq_u16(c_avx[19].val[1], a[7].val[1], b[12].val[1]);
c_avx[19].val[0] = vmlaq_u16(c_avx[19].val[0], a[8].val[0], b[11].val[0]);
c_avx[19].val[1] = vmlaq_u16(c_avx[19].val[1], a[8].val[1], b[11].val[1]);
c_avx[19].val[0] = vmlaq_u16(c_avx[19].val[0], a[9].val[0], b[10].val[0]);
c_avx[19].val[1] = vmlaq_u16(c_avx[19].val[1], a[9].val[1], b[10].val[1]);
c_avx[19].val[0] = vmlaq_u16(c_avx[19].val[0], a[10].val[0], b[9].val[0]);
c_avx[19].val[1] = vmlaq_u16(c_avx[19].val[1], a[10].val[1], b[9].val[1]);
c_avx[19].val[0] = vmlaq_u16(c_avx[19].val[0], a[11].val[0], b[8].val[0]);
c_avx[19].val[1] = vmlaq_u16(c_avx[19].val[1], a[11].val[1], b[8].val[1]);
c_avx[19].val[0] = vmlaq_u16(c_avx[19].val[0], a[12].val[0], b[7].val[0]);
c_avx[19].val[1] = vmlaq_u16(c_avx[19].val[1], a[12].val[1], b[7].val[1]);
c_avx[19].val[0] = vmlaq_u16(c_avx[19].val[0], a[13].val[0], b[6].val[0]);
c_avx[19].val[1] = vmlaq_u16(c_avx[19].val[1], a[13].val[1], b[6].val[1]);
c_avx[19].val[0] = vmlaq_u16(c_avx[19].val[0], a[14].val[0], b[5].val[0]);
c_avx[19].val[1] = vmlaq_u16(c_avx[19].val[1], a[14].val[1], b[5].val[1]);
c_avx[19].val[0] = vmlaq_u16(c_avx[19].val[0], a[15].val[0], b[4].val[0]);
c_avx[19].val[1] = vmlaq_u16(c_avx[19].val[1], a[15].val[1], b[4].val[1]);
c_avx[20].val[0] = vmulq_u16(a[5].val[0], b[15].val[0]);
c_avx[20].val[1] = vmulq_u16(a[5].val[1], b[15].val[1]);
c_avx[20].val[0] = vmlaq_u16(c_avx[20].val[0], a[6].val[0], b[14].val[0]);
c_avx[20].val[1] = vmlaq_u16(c_avx[20].val[1], a[6].val[1], b[14].val[1]);
c_avx[20].val[0] = vmlaq_u16(c_avx[20].val[0], a[7].val[0], b[13].val[0]);
c_avx[20].val[1] = vmlaq_u16(c_avx[20].val[1], a[7].val[1], b[13].val[1]);
c_avx[20].val[0] = vmlaq_u16(c_avx[20].val[0], a[8].val[0], b[12].val[0]);
c_avx[20].val[1] = vmlaq_u16(c_avx[20].val[1], a[8].val[1], b[12].val[1]);
c_avx[20].val[0] = vmlaq_u16(c_avx[20].val[0], a[9].val[0], b[11].val[0]);
c_avx[20].val[1] = vmlaq_u16(c_avx[20].val[1], a[9].val[1], b[11].val[1]);
c_avx[20].val[0] = vmlaq_u16(c_avx[20].val[0], a[10].val[0], b[10].val[0]);
c_avx[20].val[1] = vmlaq_u16(c_avx[20].val[1], a[10].val[1], b[10].val[1]);
c_avx[20].val[0] = vmlaq_u16(c_avx[20].val[0], a[11].val[0], b[9].val[0]);
c_avx[20].val[1] = vmlaq_u16(c_avx[20].val[1], a[11].val[1], b[9].val[1]);
c_avx[20].val[0] = vmlaq_u16(c_avx[20].val[0], a[12].val[0], b[8].val[0]);
c_avx[20].val[1] = vmlaq_u16(c_avx[20].val[1], a[12].val[1], b[8].val[1]);
c_avx[20].val[0] = vmlaq_u16(c_avx[20].val[0], a[13].val[0], b[7].val[0]);
c_avx[20].val[1] = vmlaq_u16(c_avx[20].val[1], a[13].val[1], b[7].val[1]);
c_avx[20].val[0] = vmlaq_u16(c_avx[20].val[0], a[14].val[0], b[6].val[0]);
c_avx[20].val[1] = vmlaq_u16(c_avx[20].val[1], a[14].val[1], b[6].val[1]);
c_avx[20].val[0] = vmlaq_u16(c_avx[20].val[0], a[15].val[0], b[5].val[0]);
c_avx[20].val[1] = vmlaq_u16(c_avx[20].val[1], a[15].val[1], b[5].val[1]);
c_avx[21].val[0] = vmulq_u16(a[6].val[0], b[15].val[0]);
c_avx[21].val[1] = vmulq_u16(a[6].val[1], b[15].val[1]);
c_avx[21].val[0] = vmlaq_u16(c_avx[21].val[0], a[7].val[0], b[14].val[0]);
c_avx[21].val[1] = vmlaq_u16(c_avx[21].val[1], a[7].val[1], b[14].val[1]);
c_avx[21].val[0] = vmlaq_u16(c_avx[21].val[0], a[8].val[0], b[13].val[0]);
c_avx[21].val[1] = vmlaq_u16(c_avx[21].val[1], a[8].val[1], b[13].val[1]);
c_avx[21].val[0] = vmlaq_u16(c_avx[21].val[0], a[9].val[0], b[12].val[0]);
c_avx[21].val[1] = vmlaq_u16(c_avx[21].val[1], a[9].val[1], b[12].val[1]);
c_avx[21].val[0] = vmlaq_u16(c_avx[21].val[0], a[10].val[0], b[11].val[0]);
c_avx[21].val[1] = vmlaq_u16(c_avx[21].val[1], a[10].val[1], b[11].val[1]);
c_avx[21].val[0] = vmlaq_u16(c_avx[21].val[0], a[11].val[0], b[10].val[0]);
c_avx[21].val[1] = vmlaq_u16(c_avx[21].val[1], a[11].val[1], b[10].val[1]);
c_avx[21].val[0] = vmlaq_u16(c_avx[21].val[0], a[12].val[0], b[9].val[0]);
c_avx[21].val[1] = vmlaq_u16(c_avx[21].val[1], a[12].val[1], b[9].val[1]);
c_avx[21].val[0] = vmlaq_u16(c_avx[21].val[0], a[13].val[0], b[8].val[0]);
c_avx[21].val[1] = vmlaq_u16(c_avx[21].val[1], a[13].val[1], b[8].val[1]);
c_avx[21].val[0] = vmlaq_u16(c_avx[21].val[0], a[14].val[0], b[7].val[0]);
c_avx[21].val[1] = vmlaq_u16(c_avx[21].val[1], a[14].val[1], b[7].val[1]);
c_avx[21].val[0] = vmlaq_u16(c_avx[21].val[0], a[15].val[0], b[6].val[0]);
c_avx[21].val[1] = vmlaq_u16(c_avx[21].val[1], a[15].val[1], b[6].val[1]);
c_avx[22].val[0] = vmulq_u16(a[7].val[0], b[15].val[0]);
c_avx[22].val[1] = vmulq_u16(a[7].val[1], b[15].val[1]);
c_avx[22].val[0] = vmlaq_u16(c_avx[22].val[0], a[8].val[0], b[14].val[0]);
c_avx[22].val[1] = vmlaq_u16(c_avx[22].val[1], a[8].val[1], b[14].val[1]);
c_avx[22].val[0] = vmlaq_u16(c_avx[22].val[0], a[9].val[0], b[13].val[0]);
c_avx[22].val[1] = vmlaq_u16(c_avx[22].val[1], a[9].val[1], b[13].val[1]);
c_avx[22].val[0] = vmlaq_u16(c_avx[22].val[0], a[10].val[0], b[12].val[0]);
c_avx[22].val[1] = vmlaq_u16(c_avx[22].val[1], a[10].val[1], b[12].val[1]);
c_avx[22].val[0] = vmlaq_u16(c_avx[22].val[0], a[11].val[0], b[11].val[0]);
c_avx[22].val[1] = vmlaq_u16(c_avx[22].val[1], a[11].val[1], b[11].val[1]);
c_avx[22].val[0] = vmlaq_u16(c_avx[22].val[0], a[12].val[0], b[10].val[0]);
c_avx[22].val[1] = vmlaq_u16(c_avx[22].val[1], a[12].val[1], b[10].val[1]);
c_avx[22].val[0] = vmlaq_u16(c_avx[22].val[0], a[13].val[0], b[9].val[0]);
c_avx[22].val[1] = vmlaq_u16(c_avx[22].val[1], a[13].val[1], b[9].val[1]);
c_avx[22].val[0] = vmlaq_u16(c_avx[22].val[0], a[14].val[0], b[8].val[0]);
c_avx[22].val[1] = vmlaq_u16(c_avx[22].val[1], a[14].val[1], b[8].val[1]);
c_avx[22].val[0] = vmlaq_u16(c_avx[22].val[0], a[15].val[0], b[7].val[0]);
c_avx[22].val[1] = vmlaq_u16(c_avx[22].val[1], a[15].val[1], b[7].val[1]);
c_avx[23].val[0] = vmulq_u16(a[8].val[0], b[15].val[0]);
c_avx[23].val[1] = vmulq_u16(a[8].val[1], b[15].val[1]);
c_avx[23].val[0] = vmlaq_u16(c_avx[23].val[0], a[9].val[0], b[14].val[0]);
c_avx[23].val[1] = vmlaq_u16(c_avx[23].val[1], a[9].val[1], b[14].val[1]);
c_avx[23].val[0] = vmlaq_u16(c_avx[23].val[0], a[10].val[0], b[13].val[0]);
c_avx[23].val[1] = vmlaq_u16(c_avx[23].val[1], a[10].val[1], b[13].val[1]);
c_avx[23].val[0] = vmlaq_u16(c_avx[23].val[0], a[11].val[0], b[12].val[0]);
c_avx[23].val[1] = vmlaq_u16(c_avx[23].val[1], a[11].val[1], b[12].val[1]);
c_avx[23].val[0] = vmlaq_u16(c_avx[23].val[0], a[12].val[0], b[11].val[0]);
c_avx[23].val[1] = vmlaq_u16(c_avx[23].val[1], a[12].val[1], b[11].val[1]);
c_avx[23].val[0] = vmlaq_u16(c_avx[23].val[0], a[13].val[0], b[10].val[0]);
c_avx[23].val[1] = vmlaq_u16(c_avx[23].val[1], a[13].val[1], b[10].val[1]);
c_avx[23].val[0] = vmlaq_u16(c_avx[23].val[0], a[14].val[0], b[9].val[0]);
c_avx[23].val[1] = vmlaq_u16(c_avx[23].val[1], a[14].val[1], b[9].val[1]);
c_avx[23].val[0] = vmlaq_u16(c_avx[23].val[0], a[15].val[0], b[8].val[0]);
c_avx[23].val[1] = vmlaq_u16(c_avx[23].val[1], a[15].val[1], b[8].val[1]);
c_avx[24].val[0] = vmulq_u16(a[9].val[0], b[15].val[0]);
c_avx[24].val[1] = vmulq_u16(a[9].val[1], b[15].val[1]);
c_avx[24].val[0] = vmlaq_u16(c_avx[24].val[0], a[10].val[0], b[14].val[0]);
c_avx[24].val[1] = vmlaq_u16(c_avx[24].val[1], a[10].val[1], b[14].val[1]);
c_avx[24].val[0] = vmlaq_u16(c_avx[24].val[0], a[11].val[0], b[13].val[0]);
c_avx[24].val[1] = vmlaq_u16(c_avx[24].val[1], a[11].val[1], b[13].val[1]);
c_avx[24].val[0] = vmlaq_u16(c_avx[24].val[0], a[12].val[0], b[12].val[0]);
c_avx[24].val[1] = vmlaq_u16(c_avx[24].val[1], a[12].val[1], b[12].val[1]);
c_avx[24].val[0] = vmlaq_u16(c_avx[24].val[0], a[13].val[0], b[11].val[0]);
c_avx[24].val[1] = vmlaq_u16(c_avx[24].val[1], a[13].val[1], b[11].val[1]);
c_avx[24].val[0] = vmlaq_u16(c_avx[24].val[0], a[14].val[0], b[10].val[0]);
c_avx[24].val[1] = vmlaq_u16(c_avx[24].val[1], a[14].val[1], b[10].val[1]);
c_avx[24].val[0] = vmlaq_u16(c_avx[24].val[0], a[15].val[0], b[9].val[0]);
c_avx[24].val[1] = vmlaq_u16(c_avx[24].val[1], a[15].val[1], b[9].val[1]);
c_avx[25].val[0] = vmulq_u16(a[10].val[0], b[15].val[0]);
c_avx[25].val[1] = vmulq_u16(a[10].val[1], b[15].val[1]);
c_avx[25].val[0] = vmlaq_u16(c_avx[25].val[0], a[11].val[0], b[14].val[0]);
c_avx[25].val[1] = vmlaq_u16(c_avx[25].val[1], a[11].val[1], b[14].val[1]);
c_avx[25].val[0] = vmlaq_u16(c_avx[25].val[0], a[12].val[0], b[13].val[0]);
c_avx[25].val[1] = vmlaq_u16(c_avx[25].val[1], a[12].val[1], b[13].val[1]);
c_avx[25].val[0] = vmlaq_u16(c_avx[25].val[0], a[13].val[0], b[12].val[0]);
c_avx[25].val[1] = vmlaq_u16(c_avx[25].val[1], a[13].val[1], b[12].val[1]);
c_avx[25].val[0] = vmlaq_u16(c_avx[25].val[0], a[14].val[0], b[11].val[0]);
c_avx[25].val[1] = vmlaq_u16(c_avx[25].val[1], a[14].val[1], b[11].val[1]);
c_avx[25].val[0] = vmlaq_u16(c_avx[25].val[0], a[15].val[0], b[10].val[0]);
c_avx[25].val[1] = vmlaq_u16(c_avx[25].val[1], a[15].val[1], b[10].val[1]);
c_avx[26].val[0] = vmulq_u16(a[11].val[0], b[15].val[0]);
c_avx[26].val[1] = vmulq_u16(a[11].val[1], b[15].val[1]);
c_avx[26].val[0] = vmlaq_u16(c_avx[26].val[0], a[12].val[0], b[14].val[0]);
c_avx[26].val[1] = vmlaq_u16(c_avx[26].val[1], a[12].val[1], b[14].val[1]);
c_avx[26].val[0] = vmlaq_u16(c_avx[26].val[0], a[13].val[0], b[13].val[0]);
c_avx[26].val[1] = vmlaq_u16(c_avx[26].val[1], a[13].val[1], b[13].val[1]);
c_avx[26].val[0] = vmlaq_u16(c_avx[26].val[0], a[14].val[0], b[12].val[0]);
c_avx[26].val[1] = vmlaq_u16(c_avx[26].val[1], a[14].val[1], b[12].val[1]);
c_avx[26].val[0] = vmlaq_u16(c_avx[26].val[0], a[15].val[0], b[11].val[0]);
c_avx[26].val[1] = vmlaq_u16(c_avx[26].val[1], a[15].val[1], b[11].val[1]);
c_avx[27].val[0] = vmulq_u16(a[12].val[0], b[15].val[0]);
c_avx[27].val[1] = vmulq_u16(a[12].val[1], b[15].val[1]);
c_avx[27].val[0] = vmlaq_u16(c_avx[27].val[0], a[13].val[0], b[14].val[0]);
c_avx[27].val[1] = vmlaq_u16(c_avx[27].val[1], a[13].val[1], b[14].val[1]);
c_avx[27].val[0] = vmlaq_u16(c_avx[27].val[0], a[14].val[0], b[13].val[0]);
c_avx[27].val[1] = vmlaq_u16(c_avx[27].val[1], a[14].val[1], b[13].val[1]);
c_avx[27].val[0] = vmlaq_u16(c_avx[27].val[0], a[15].val[0], b[12].val[0]);
c_avx[27].val[1] = vmlaq_u16(c_avx[27].val[1], a[15].val[1], b[12].val[1]);
c_avx[28].val[0] = vmulq_u16(a[13].val[0], b[15].val[0]);
c_avx[28].val[1] = vmulq_u16(a[13].val[1], b[15].val[1]);
c_avx[28].val[0] = vmlaq_u16(c_avx[28].val[0], a[14].val[0], b[14].val[0]);
c_avx[28].val[1] = vmlaq_u16(c_avx[28].val[1], a[14].val[1], b[14].val[1]);
c_avx[28].val[0] = vmlaq_u16(c_avx[28].val[0], a[15].val[0], b[13].val[0]);
c_avx[28].val[1] = vmlaq_u16(c_avx[28].val[1], a[15].val[1], b[13].val[1]);
c_avx[29].val[0] = vmulq_u16(a[14].val[0], b[15].val[0]);
c_avx[29].val[1] = vmulq_u16(a[14].val[1], b[15].val[1]);
c_avx[29].val[0] = vmlaq_u16(c_avx[29].val[0], a[15].val[0], b[14].val[0]);
c_avx[29].val[1] = vmlaq_u16(c_avx[29].val[1], a[15].val[1], b[14].val[1]);
c_avx[30].val[0] = vmulq_u16(a[15].val[0], b[15].val[0]);
c_avx[30].val[1] = vmulq_u16(a[15].val[1], b[15].val[1]);
c_avx[31].val[0] = vdupq_n_u16(0);
c_avx[31].val[1] = vdupq_n_u16(0);
}
