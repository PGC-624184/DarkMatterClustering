#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <definitions.h>
#include <omp.h>
#include <DM_loop.h>

void DM_Loop(struct liststruct *a, int n, float boxSize, int MAX_SEP) {
    int j;
    int i;
    float sep;
    #pragma omp parallel for
    for (i=0;i<n-1;i++) { //loop through particles
        if (a[i].type==1){ //only do more if DM
            for (j=i+1;j<n;j++) { // loop through remaining particles
                if (a[j].type==1) { //
                    sep = periodic_separation(&a[i],&a[j],boxSize,3);
                    if (sep <= MAX_SEP) {
                        list_combine(a,i,j);
                    }
                }
            }
        }
        printf("\rComputing Dark Matter groups for particle %d of %d",i+1,n);
        fflush(stdout);
    }
}