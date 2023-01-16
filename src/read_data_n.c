#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <read_data_n.h>

int read_data_n(FILE *fp, float *boxSize) {

    char buf[1024];
    int n;
    
    if (fgets(buf, sizeof buf, fp) == NULL) {
        printf("Error: There was a problem reading the file.");
        return -1;
    }
    (void)*fgets(buf, sizeof buf, fp); // Cast to void to avoid return value warning
    sscanf(buf,"%d %g",&n, boxSize);

    return n;
}
