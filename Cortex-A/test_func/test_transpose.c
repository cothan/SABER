#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <arm_neon.h>


#include "../polymul/toom_cook_4/asimd_matrix.c"

void readFile(uint16_t *out, char *filename, int length)
{
    char *line = NULL;
    int i = 0;
    int len, nread;
    int ret; 
    FILE *fp = fopen(filename, "r");
    char *token; 
    char *endptr; 
    
    if (fp == NULL)
    {
        perror("Error, no test file to read\n");
        exit(EXIT_FAILURE);
    }
    
    while ((nread = getline(&line, &len, fp)) != -1) {
        printf("Retrieved line of length %zu:\n", nread);
        // printf("%s\n", line);
        // TODO strtok to separate input 

        token = strtok(line, ",");
        while (token != NULL)
        {
            // printf("token %s\n", token);
            out[i++] = strtol(token, &endptr, 10);
            token = strtok(NULL, ",");
            if (i >= length){
                return ;
            }
        }
    }  
    printf("i = %d, %d", i, length);
    // assert(i == length);
}

void printArray(uint16_t *M, char *string)
{
    printf("%s: \n", string);
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++){
            printf("%5d ", M[16*i + j]);
        }
        printf("\n");
    }
}


int compareArray(uint16_t *a, uint16_t *b, size_t length)
{
    for (int i = 0; i < length; i++)
    {
        if (a[i] != b[i])
        {
            printf("Error at %d: %d != %d\n", i, a[i], b[i]);
            // return 1;
        }
    }
    return 0;
}

int test1()
{
    uint16_t m[256], M_gold[256];
    readFile(m, "test_transpose_m.txt", 256);
    readFile(M_gold, "test_transpose_M.txt", 256);

    printArray(m, "m:");
    printArray(M_gold,"M_gold:");
    printf("========\n");
    transpose(m);
    printArray(m, "M_test:");
    return compareArray(m, M_gold, 256);
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