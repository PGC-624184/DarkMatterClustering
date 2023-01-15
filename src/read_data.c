#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <definitions.h>
#include <read_data.h>

void read_data(FILE *fp, struct liststruct *L, int n) {

    int i;
    char buf[1024];

    /* Part 1 */
    /* Read data and populate List */
    if(fp == NULL) {
      perror("Error opening file");
      return;
    }

    for(i=0;i<n;i++) {
      fgets(buf,sizeof buf,fp);
      sscanf(buf,"%g %g %g %g %d",&L[i].p[0],&L[i].p[1],&L[i].p[2],&L[i].m,&L[i].type);
      L[i].next = NULL;
      L[i].prev = NULL;
    }

    /* close the file when finished */
    fclose(fp);

}
