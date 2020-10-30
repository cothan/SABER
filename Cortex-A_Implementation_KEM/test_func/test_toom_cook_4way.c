#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <arm_neon.h>

#include "../rq_mul/neon_poly_rq_mul.h"

void readFile(uint16_t *out, char *filename, int length)
{
    size_t len;
    char *token, *endptr, *line = NULL; 
    int i, nread, col = 0;
    
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror("Error, no test file to read\n");
        exit(EXIT_FAILURE);
    }

    while ((nread = getline(&line, &len, fp)) != -1) {
        // printf("Retrieved line of length %zu:\n", nread);
        // printf("%s\n", line);
        // TODO strtok to separate input 

        token = strtok(line, ",");
        i = 0;
        while (token != NULL && i < length)
        {
            nread = strtoul(token, &endptr, 10);
            out[i++] = nread;
            // printf("%s -- %d -- %d -- %d, ", token, nread, col,  out[col][i-1]);
            token = strtok(NULL, ",");
        }
        col++;
        // printf("newline\n");
    }  
    // printf("i = %d, %d", i, length);
    // assert(i == length);
}

void printArray(uint16_t *M, int size,  char *string)
{
  printf("%s: \n", string);
  for (int i = 0; i < size / 16; i++) {
    for (int j = 0; j < 16; j++) {
      printf("%5d ", M[16 * i + j]);
    }
    printf("\n");
  }
  printf("---------\n");
}

int compareArray(uint16_t *a, uint16_t *b, size_t length) {
  for (size_t i = 0; i < length; i++) {
    if (a[i] != b[i]) {
      printf("Error at [%zu]: %d != %d\n", i, a[i], b[i]);
      return 1;
    }
  }

  return 0;
}

int test1() {
    // TODO: change the input size to 256 rather than 512
  uint16_t res_test[256] = {0};
  uint16_t a[256], b[256], res_gold[256];
  readFile(a, "test_toom_a.txt", 256);
  readFile(b, "test_toom_b.txt", 256);
  readFile(res_gold, "test_toom_res.txt", 256);

  printArray(a, 256, "a_avx:");
  printf("========\n");
  printArray(b, 256, "b_avx:");
  printf("========\n");
  printArray(res_gold, 256, "res_gold:");
  printf("========\n");

  poly_mul_neon(res_test, a, b);

  printArray(res_test, 256, "res_test:");
  printf("========\n");
  //   printArray(res_gold, 512, "res_gold:");
  int ret = compareArray(res_test, res_gold, 256);
  return ret;
}

int main() {
  int ret;
  ret = test1();
  printf("Finish test\n");
  if (ret != 0) {
    return 1;
  }
  return ret;
}