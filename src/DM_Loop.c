#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <definitions.h> */
#include <omp.h>
#define OMP_NUM_THREADS = 16;

int main(void)
{
    #pragma omp parallel
    printf("Hello, world.\n");
    return 0;
}