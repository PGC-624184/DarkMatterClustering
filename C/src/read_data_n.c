#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <read_data_n.h>

int read_data_n(FILE *fp, float *boxSize) {

    char buf[1024];
    int n;
    
    fgets(buf, sizeof buf, fp);
    sscanf(buf,"%d %g",&n, boxSize);

    return n;
}
