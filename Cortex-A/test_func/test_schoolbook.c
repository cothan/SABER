#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../polymul/toom_cook_4/asimd_scm_neon.h"

void readFile(uint16_t *out, char *filename, size_t length)
{
    char *line = NULL;
    size_t i = 0;
    size_t len, nread;
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
        }
    }  
    // printf("i = %d, %d",i, length);
    // assert(i == length);
}

void printArray(uint16_t *array, size_t length, char *string)
{   
    printf("%s: ", string);
    for (size_t i = 0; i < length; i++)
    {
        printf("%d, ", array[i]);
    }
    printf("\n");
}

int compareArray(uint16_t *a, uint16_t *b, size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        if (a[i] != b[i])
        {
            printf("Error at %d: %d != %d\n", i, a[i], b[i]);
            i+=15;
            // return 1;
        }
    }
    return 0;
}

int test1()
{
    uint16_t a[256], b[256], c_gold[512], c_test[512];
    readFile(a, "test_schoolbook_a.txt", 256);
    readFile(b, "test_schoolbook_b.txt", 256);
    readFile(c_gold, "test_schoolbook_c.txt", 512);

    printArray(a, 256, "A:");
    printArray(b, 256, "B:");
    printArray(c_gold, 512, "C:");
    printf("========\n");
    schoolbook_neon_new(c_test, a, b);
    printArray(c_test, 512, "C_test:");
    return compareArray(c_test, c_gold, 512);
}

int main()
{
    int ret;
    ret = test1();
    if (ret != 0){
        return 1;
    }
    return ret;
}