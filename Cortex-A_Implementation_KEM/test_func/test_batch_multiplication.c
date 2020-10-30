#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <arm_neon.h>


#include "../polymul/toom_cook_4/batch_64coefficient_multiplications.h"

void readFile(uint16_t out[7][128], char *filename, int length)
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
            out[col][i++] = nread;
            // printf("%s -- %d -- %d -- %d, ", token, nread, col,  out[col][i-1]);
            token = strtok(NULL, ",");
        }
        col++;
        // printf("newline\n");
    }  
    // printf("i = %d, %d", i, length);
    // assert(i == length);
}

void printArray(uint16_t M[7][128], int size,  char *string)
{
    for (int k = 0; k < 7; k++)
    {
        printf("%d-%s: \n", k, string);
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < 16; j++){
                printf("%5d ", M[k][16*i + j]);
            }
            printf("\n");
        }
        printf("---------%d\n", k);
    }
}


int compareArray(uint16_t a[7][128], uint16_t b[7][128])
{
    for (int k = 0; k < 7; k++)
    {    
        for (size_t i = 0; i < 64; i++)
        {
            if (a[k][i] != b[k][i])
            {
                printf("Error at [%d][%zu]: %d != %d\n", k, i, a[k][i], b[k][i]);
                return 1;
            }
        }
        printf("-----%d\n", k);
    }
    return 0;
}

int test1()
{
    // TODO: Fix overflow 
    uint16_t w_test[7][128];
    uint16_t a[7][128], b[7][128], w_gold[7][128];
    readFile(a, "test_batch_a1.txt", 64);
    readFile(b, "test_batch_b1.txt", 64);
    readFile(w_gold, "test_batch_w7.txt", 128);

    printArray(a, 4, "a_ph_avx:");
    printf("========\n");
    printArray(b, 4, "b_ph_avx:");
    printf("========\n");
    printArray(w_gold, 8, "w_avx_gold:");
    printf("========\n");
    batch_64coefficient_multiplications(
        a[0], b[0], w_test[0],
        a[1], b[1], w_test[1],
        a[2], b[2], w_test[2],
        a[3], b[3], w_test[3],
        a[4], b[4], w_test[4],
        a[5], b[5], w_test[5],
        a[6], b[6], w_test[6]);

    printArray(w_test, 8, "w_avx_test:");
    printf("========\n");
    printArray(w_gold, 8, "w_avx_gold:");
    int ret = compareArray(w_test, w_gold);
    return ret;
}

int main()
{
    int ret;
    ret = test1();
    printf("Finish test\n");
    if (ret != 0){
        return 1;
    }
    return ret;
}