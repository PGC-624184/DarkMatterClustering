#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <read_data_n.h>

int read_data_n(FILE *fp, float *boxSize) {

    char buf[1024];
    int n;
    
    if (fgets(buf, sizeof buf, fp) == NULL) {
        printf("Error: There was an error opening the file.");
        return -1;
    }
    
    if(fgets(buf, sizeof buf, fp)); // means we use the result of fgets without doing do
    sscanf(buf,"%d %g",&n, boxSize);
    return n;
}
