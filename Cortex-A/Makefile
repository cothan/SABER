CC 		  = clang
CFLAGS   = -Wall -Wextra -O3 -fomit-frame-pointer -march=native -lcrypto
CFLAGS2  = -Wall -Wextra -O3 -fomit-frame-pointer -march=native -std=c11 
CFLAGS3  = -Wall -Wextra -O3 -fomit-frame-pointer -march=native
CFLAGS4  = -Wall -Wextra -O3 -fomit-frame-pointer -mtune=native -mcpu=cortex-a53 -lpapi -lkeccak -g3
CFLAGS5  = -Wall -Wextra -O3 -fomit-frame-pointer -mtune=native -mcpu=cortex-a53 -lcrypto -fwrapv -Qunused-arguments -w  -lpapi -lkeccak -g3
CLANG   = clang -march=native -O3 -fomit-frame-pointer -lcrypto -fwrapv -Qunused-arguments -w  -lpapi -lkeccak -g3
RM 		  = /bin/rm


all: test/PQCgenKAT_kem \
     test/test_speed

SOURCES = pack_unpack.c poly.c verify.c fips202.c SABER_indcpa.c kem.c \
					cbd.c rng.c
#rng.o
#randombytes.c

SOURCES2 =  pack_unpack.c rng.c poly.c verify.c SABER_indcpa.c kem.c \
			cbd.c fips202.c 

HEADERS = SABER_params.h pack_unpack.h poly.h verify.h cbd.h SABER_indcpa.h kem.h rng.h fips202.h api.h randombytes.h


test/test_speed: $(SOURCES2) $(HEADERS) test/test_speed.c 
	$(CC) $(CFLAGS5) -o $@ $(SOURCES2) test/test_speed.c -lkeccak

test/PQCgenKAT_kem: $(SOURCES2) $(HEADERS) test/PQCgenKAT_kem.c 
	$(CC) $(CFLAGS5) -o $@ $(SOURCES2) test/PQCgenKAT_kem.c -lkeccak


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


