p = print
SCM_SIZE = 16

c_avx = [None]*SCM_SIZE
a = [None]*SCM_SIZE
b = [None]*SCM_SIZE


def vmul(ci, ai, bi):
    # ci = ai * b*
    # p("c_avx[{}] = vmulq_u16(a[{}], b[{}]);".format(ci, ai, bi))

    p("c_avx[{}]->val[0] = vmulq_u16(a[{}]->val[0], b[{}]->val[0]);".format(ci, ai, bi))
    p("c_avx[{}]->val[1] = vmulq_u16(a[{}]->val[1], b[{}]->val[1]);".format(ci, ai, bi))


def vmla(ci, ai, bi):
    #  ci = ci + ai*bi
    # p("c_avx[{}] = vmlaq_u16(c_avx[{}], a[{}], b[{}]);".format(ci, ci, ai, bi))

    p("c_avx[{}]->val[0] = vmlaq_u16(c_avx[{}]->val[0], a[{}]->val[0], b[{}]->val[0]);".format(ci, ci, ai, bi))
    p("c_avx[{}]->val[1] = vmlaq_u16(c_avx[{}]->val[1], a[{}]->val[1], b[{}]->val[1]);".format(ci, ci, ai, bi))


def vconst(ci, value):
    # c = value
    # p("c_avx[{}] = vdupq_n_u16({});".format(ci, value))

    p("c_avx[{}]->val[0] = vdupq_n_u16({});".format(ci, value))
    p("c_avx[{}]->val[1] = vdupq_n_u16({});".format(ci, value))


def schoolbook_avx_new():
    #  the first triangle
    for i in range(SCM_SIZE):
        vmul(ci=i, ai=0, bi=i)
        for j in range(1, i+1):
            vmla(ci=i, ai=j, bi=i-j)

    scm_size_1 = SCM_SIZE - 1

    # the second triangle
    for i in range(1, SCM_SIZE):
        vmul(ci=scm_size_1 + i, ai=i, bi=scm_size_1)
        vv = scm_size_1 + i
        for j in range(1, SCM_SIZE - i):
            vmla(ci=vv, ai=i+j, bi=scm_size_1-j)

    vconst(ci=2*SCM_SIZE-1, value=0)


if __name__ == "__main__":
    p("""#define SCM_SIZE 16

uint16x8x2_t c_avx[2*SCM_SIZE]; 
uint16x8x2_t a[SCM_SIZE+2]; 
uint16x8x2_t b[SCM_SIZE+2]; 

/*
 * schoolbook_neon_new
 * Assume a, b are already loaded 
 */
void schoolbook_avx_new()
{""")
    schoolbook_avx_new()
    p("}")