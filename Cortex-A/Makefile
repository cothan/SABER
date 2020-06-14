CC 		  = clang
CFLAGS   = -Wall -Wextra -O3 -fomit-frame-pointer -march=native -lcrypto
CFLAGS2  = -Wall -Wextra -O3 -fomit-frame-pointer -march=native -std=c11 
CFLAGS3  = -Wall -Wextra -O3 -fomit-frame-pointer -march=native
CFLAGS4  = -Wall -Wextra -O3 -fomit-frame-pointer -mtune=native -mcpu=cortex-a53 -lpapi -g3
CFLAGS5  = -Wall -Wextra -O3 -fomit-frame-pointer -mtune=native -mcpu=cortex-a53 -lcrypto -fwrapv -w  -lpapi -g3 
CFLAGS6  = -Wall -Wextra -O3 -fomit-frame-pointer -lcrypto -fwrapv -w  -lpapi -g3 
CFLAGSDEBUG  = -Wall -Wextra -O0 -fomit-frame-pointer -mtune=native -mcpu=native -march=native -lcrypto -fwrapv -w  -lpapi -g3
CLANG   = -Wall -Wextra -O3 -fomit-frame-pointer -mtune=native -mcpu=cortex-a53 -lcrypto -fwrapv -Qunused-arguments -w  -lpapi -g3
RM 		  = /bin/rm


all: test/PQCgenKAT_kem \
     test/test_speed

SOURCES = pack_unpack.c poly.c verify.c fips202.c SABER_indcpa.c kem.c \
					cbd.c rng.c
#rng.o
#randombytes.c

SOURCES2 =  pack_unpack.c \
			rng.c \
			poly.c \
			verify.c \
			polymul/toom_cook_4/asimd_scm_neon.c \
			polymul/toom_cook_4/batch_64coefficient_multiplications.c \
			polymul/toom_cook_4/asimd_toom_cook_4way_neon.c \
			SABER_indcpa.c \
			kem.c \
			cbd.c \
			fips202.c
			
			

HEADERS = 	SABER_params.h  \
			pack_unpack.h  \
			poly.h  \
			verify.h  \
			polymul/toom_cook_4/asimd_scm_neon.h \
			polymul/toom_cook_4/batch_64coefficient_multiplications.h \
			polymul/toom_cook_4/asimd_scm_neon.h \
			cbd.h  \
			SABER_indcpa.h  \
			kem.h  \
			rng.h  \
			fips202.h  \
			api.h  \
			randombytes.h \


test/test_speed: $(SOURCES2) $(HEADERS) test/test_speed.c 
	$(CC) $(CFLAGS5) -o $@ $(SOURCES2) test/test_speed.c -lpapi

test/PQCgenKAT_kem: $(SOURCES2) $(HEADERS) test/PQCgenKAT_kem.c 
	$(CC) $(CFLAGS6) -o $@ $(SOURCES2) test/PQCgenKAT_kem.c 


rng.o: rng.c
	$(CC) $(CFLAGS2) -c rng.c -lcrypto -o $@ 

fips202.o: fips202.c
	$(CLANG) -c $^ -o $@

.PHONY: clean test

test:
	./test/test_speed
	./test/PQCgenKAT_kem


clean:
	-$(RM) -r test/test_speed
	-$(RM) -r test/PQCgenKAT_kem
	-$(RM) -r test/*.req
	-$(RM) -r test/*.rsp


