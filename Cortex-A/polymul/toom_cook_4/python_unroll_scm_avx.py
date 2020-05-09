p = print
SCM_SIZE = 16

c_avx = [None]*SCM_SIZE
a = [None]*SCM_SIZE
b = [None]*SCM_SIZE


def vmul(ci, ai, bi):
    # ci = ai * b*
    p("vmul(tmp, a[{}], b[{}]);".format(ai, bi))


def vmla(ci, ai, bi):
    #  ci = ci + ai*bi
    p("vmla(tmp, a[{}], b[{}]);".format(ai, bi))


def vconst(ci, value):
    # c = value
    p("vconst(c_avx[{}], {});".format(ci, value))

def vstore(ci, tmp):
    p("vst1q_u16(&c[{} + pad], {});".format(ci*16, tmp))

def schoolbook_avx_new():
    #  the first triangle
    al, bl, cl  = [], [], []
    for i in range(0, SCM_SIZE):
        vmul(ci=i, ai=0, bi=i)
        cl.append(i)
        al.append(0)
        bl.append(i)
        for j in range(1, i+1):
            vmla(ci=i, ai=j, bi=i-j)
            cl.append(i)
            al.append(j)
            bl.append(i-j)
            if len(set(al)) + len(set(bl)) > 32: 
                # p("// {} {} BREAK HERE".format(i, j))
                p("//=======")
                p("// ci = ", set(cl), len(set(cl)) )
                p("// ai = ", set(al), len(set(al)) )
                p("// bi = ", set(bl), len(set(bl)) )
                cl = [] 
                bl = []
                al = []
        vstore(ci=i, tmp="tmp")
        p("//----")
        
    p("//=======")
    p("// ci = ", set(cl), len(set(cl)) )
    p("// ai = ", set(al), len(set(al)) )
    p("// bi = ", set(bl), len(set(bl)) )
    scm_size_1 = SCM_SIZE - 1
    p("// ----------------PART 2----------------")

    # the second triangle
    for i in range(1, SCM_SIZE):
        vmul(ci=scm_size_1 + i, ai=i, bi=scm_size_1)
        vv = scm_size_1 + i
        for j in range(1, SCM_SIZE - i):
            vmla(ci=vv, ai=i+j, bi=scm_size_1-j)
        
        vstore(ci=scm_size_1 + i, tmp="tmp")
        print("//-----")

    vconst(ci=2*SCM_SIZE-1, value=0)
    # vstore(ci=2*SCM_SIZE-1, )


if __name__ == "__main__":
    p("""#define SCM_SIZE 16

uint16_t c_avx[16*2*SCM_SIZE]; 
uint16_t a[16*SCM_SIZE]; 
uint16_t b[16*SCM_SIZE]; 
uint16_t c_avx_extra[16*4];


/*
 * schoolbook_neon_new
 * Assume a, b are already loaded 
 */
void schoolbook_neon_new(uint16_t *c_mem, uint16_t *a_mem, uint16_t *b_mem)
{
    uint16x8_t tmp, a[16], b[16];
    unsigned int i;
    unsigned int pad = 0;
    for (i = 0; i < 2; i++)
    {
""")
    # TODO: for loop vload(a_mem), vload(b_mem)
    schoolbook_avx_new()

    p("""
    pad = 8;
    }
  vxor(tmp, tmp, tmp);
  vstore(&c[31*16], tmp);
  vstore(&c[31*16 + 8], tmp);
  """)
    p("}")