CC 		  = cc
CFLAGS4  = -Wall -Wextra -O3 -fomit-frame-pointer -mtune=native -mcpu=cortex-a53 -lpapi -g3
CFLAGS5  = -Wall -Wextra -O3 -fomit-frame-pointer -mtune=native -mcpu=native -lcrypto -fwrapv -w  -g3 -I/usr/include -I/usr/local/include -lpapi
CFLAGS6  = -Wall -Wextra -O3 -fomit-frame-pointer -mtune=native -mcpu=native -lcrypto -fwrapv -w  -lpapi -g3 
CFLAGSDEBUG  = -Wall -Wextra -O0 -fomit-frame-pointer -mtune=native -mcpu=native -march=native -lcrypto -fwrapv -w  -lpapi -g3
CLANG   = -Wall -Wextra -O2 -fomit-frame-pointer -mtune=native -mcpu=cortex-a53 -lcrypto -fwrapv -Qunused-arguments -w  -lpapi -g3
RM 		  = /bin/rm


all: test/PQCgenKAT_kem \
     test/test_speed \
	 test/test_speed2

SOURCES = pack_unpack.c poly.c verify.c fips202.c SABER_indcpa.c kem.c cbd.c rng.c

SOURCES2 =  pack_unpack.c \
			rng.c \
			poly.c \
			verify.c \
			SABER_indcpa.c \
			kem.c \
			cbd.c \
			fips202.c

HEADERS = 	SABER_params.h  \
			pack_unpack.h  \
			poly.h  \
			verify.h  \
			cbd.h  \
			SABER_indcpa.h  \
			kem.h  \
			rng.h  \
			fips202.h  \
			api.h  \
			randombytes.h \


test/test_speed: $(SOURCES2) $(HEADERS) test/test_speed.c 
	$(CC) $(CFLAGS6) -o $@ $(SOURCES2) test/test_speed.c 

test/test_speed2: $(SOURCES2) $(HEADERS) test/test_speed2.c 
	$(CC) $(CFLAGS6) -o $@ $(SOURCES2) test/test_speed2.c 

test/PQCgenKAT_kem: $(SOURCES2) $(HEADERS) test/PQCgenKAT_kem.c 
	$(CC) $(CFLAGS6) -o $@ $(SOURCES2) test/PQCgenKAT_kem.c 


rng.o: rng.c
	$(CC) $(CFLAGS2) -c rng.c -lcrypto -o $@ 

fips202.o: fips202.c
	$(CLANG) -c $^ -o $@

.PHONY: clean test

test:
	./test/test_speed
	./test/test_speed2

clean:
	-$(RM) -r test/test_speed
	-$(RM) -r test/test_speed2
	-$(RM) -r test/PQCgenKAT_kem
	-$(RM) -r test/*.req
	-$(RM) -r test/*.rsp
