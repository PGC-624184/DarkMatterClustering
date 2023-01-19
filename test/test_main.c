// ----------------------------------------------------------------------------
// Date Created:          15/10/2021
// Last Edited:
// Version:               Linux Mint 20.1
// Description:
//
// Outputs:
// ---------------------------------------------------------------------------

// Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <omp.h>

/* Structures */
struct liststruct {
    float p[3];
    float m;
    int type;
    int group;
    struct liststruct *next;
    struct liststruct *prev;
} *List;

/* Prototypes */
int read_data_n(FILE *fp, float *boxSize); 
void read_data(FILE *fp, struct liststruct *L, int n);
float periodic_separation(struct liststruct *a, struct liststruct *b, float boxSize, int dim);
void list_combine(struct liststruct *L, long i, long j);

/* Moved functions and structs into cleaner structure */
//#include <definitions.h>
//#include <read_data_n.h>
//#include <read_data.h>
//#include <periodic_separation.h>
//#include <list_combine.h>


#define MAX_SEP 5

/* main program */
int main(int argc, char *argv[]) {

    int i,j;
    int n;
    float boxSize;
    float sep;
    char fname[1024];
    FILE *fp;
    
    /* parse arguments */
    if (argc != 2)
        return -1;
    // fname is defined as argv[1]
    strcpy(fname,argv[1]);

    // print file location
    printf("The file location is %s\n",fname);

    /* Read in some data */
    /* Part 1 */
    if(!(fp = fopen(fname,"r")))
        return -1;

    // Find the number of particles
    n = read_data_n(fp,&boxSize);

    printf("The number of particles is %d\n", n);
    printf("The box size is is %f\n", boxSize);

    // Allocate for data
    List = (struct liststruct *) malloc(n * sizeof(*List));

    
    // closing files and freeing List
    if (List == NULL) {
        printf("Error: ould not creat List. Exiting.");
        close(fp);
        free(List);
        return -2;
    }

    // read the data
    read_data(fp,List,n);
    
    /* Part 2 */
    /* get separation of each DM pair and form groups */
    /* Hint -- only need to include DM particles in these loops */

    // Timing calls
    
    clock_t start, end;
    double cpu_time_used;
    float MAX_SEP2 = MAX_SEP*MAX_SEP;

    start = clock();
    
    // I hate this section of code. ~O(n^2)
    
    //#pragma omp parallel for
    for (i=0;i<n;i++) { //loop through particles
        if (List[i].type==1){ //only do more if DM
            //#pragma omp parallel for // Run parallel loop here bc previous loop is just a select loop.
            for (j=i+1;j<n;j++) { // loop through remaining particles
                if (List[j].type==1) { //
                    sep = periodic_separation(&List[i],&List[j],boxSize,3);
                    if (sep <= MAX_SEP2) {
                        list_combine(List,i,j);
                    }
                }
            }
        }
        printf("\rComputing Dark Matter groups for particle %d of %d",i+1,n);
        fflush(stdout);
    }

    printf("\n");
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("The Dark Matter calculation takes %0.2f seconds\n",cpu_time_used );
    
    int numDM = 0;
    int numGas = 0;
    int numBH = 0;
    int numStar = 0;

    for (i=0;i<n;i++) {
      if (List[i].type==1) {
        numDM +=1;
      }
      if (List[i].type==0) {
        numGas +=1;
      }
      if (List[i].type==4) {
        numStar +=1;
      }
      if (List[i].type==5) {
        numBH +=1;
      }
    }
    printf("There are %d Gas Particles\n",numGas);
    printf("There are %d Dark Matter Particles\n",numDM);
    printf("There are %d Star Particles\n",numStar);
    printf("There are %d Black Hole Particles\n",numBH);

    /* Part 3 */
    /* Add baryons to group of nearest DM part */
    /* Hint -- similar to above but one loop for baryons,
       one loop for DM.
       Hint -- when should you call list_combine now?
    */
    start = clock();
    /*
    This is another candidate area to implement a Ball Tree  - which will deal with periodic boundary conditions
    */
   
    for (i=0;i<n;i++) { // loop through all particles
        int bestindex = 0;
        float dist[2]={boxSize,boxSize};
        if (List[i].type !=1 ) { // baryons only in calc
            for (j=0;j<n;j++) { // loop through all particles
                if (List[j].type==1) { // select DM particles
                    dist[1] = periodic_separation(&List[i],&List[j],boxSize,3); // calc dist
                    if (dist[1]<dist[0]){ // update closest distance index
                        bestindex = j; // get closest match index
                        dist[0] = dist[1]; // update dist
                    }
                }
            }
        list_combine(List,i,bestindex); // combine to group
        }
        printf("\rComputing closest DM for particle %d of %d",i+1,n);
        fflush(stdout);
    }

    struct liststruct **Start = (struct liststruct **) malloc(n * sizeof(struct liststruct *));
    if(!Start) {
        printf("Couldn't allocate memory for Start array\n");
        return -3;
    }
    // Calculate the number of groups
    int count = 0;
    for(i=0; i<n; i++) {
        if(List[i].prev == NULL && List[i].next != NULL) {
            Start[count++] = &List[i];
        }
    }
    printf("\nThere are %d groups.\n",count);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("The Baryon calculation takes %0.2f seconds\n",cpu_time_used );

    /* Part 4 */
    /* Do some analysis on each group */

    printf("The below arrays have size %d", count);    
    // Below arrays do not have an initialisation value, giving error in Infer
    float *Mass[count] = (float *)malloc(count * sizeof(float));
    // All Gas and DM particles have the same mass
    float Gas[count];
    float DM[count];
    // stars, BH particles have different masses
    float Star[count];
    float BH[count];


    // allocate for largest mass
    float bigMass=0.0;
    struct liststruct *l;
    float MassLimit = pow(10,9);
    int massive_count=0;

    // Error from uninitialised values above
    int p;
    for(p=0;p<count;p++) { // loop over unique groups
        for(Mass[p]=0.0,l=Start[p]; l != NULL; l = l->next) {
            Mass[p] += l->m;
            if (l->type==0) {
                Gas[p]+=l->m;
            };
            if (l->type==1) {
                DM[p]+=l->m;
            };
            if (l->type==4) {
                Star[p]+=l->m;
            };
            if (l->type==5) {
                BH[p]+=l->m;
            };
        }
        if (Mass[p] > bigMass) {
            bigMass = Mass[p];
        }
        if (Star[p]>MassLimit) {
            massive_count+=1;
        }
    }
    // on screen display of results of massive groups
    printf("Found %d massive groups. Largest group was %g\n",massive_count,bigMass);
    

    /* Open file and write data*/
    char outfile[30] = "analysis.csv";
    FILE *f = fopen(outfile, "w");

    // check if file ptr exists
    if (f == NULL) {
        printf("Error: Failed to open file");
        return -4;
    }

    // file header
    fprintf(f,"Total_Mass, Gas_Mass, DM_Mass, Star_Mass, BH_Mass\n");
    
    // write results to file 
    for(i=0;i<count;i++) {
      if (Star[i]>=MassLimit) {
        // Write masses to file
        fprintf(f,"%g, %g, %g, %g, %g\n",log10(Mass[i]),log10(Gas[i]),log10(DM[i]),log10(Star[i]),log10(BH[i]));
        //printf("%d\n",i);
      }
    }
    fclose(f);

    //clean up memory
    free(Start);
    free(List);

    return 0;
};


int read_data_n(FILE *fp, float *boxSize) {

    char buf[1024];
    int n;
    
    (void)*fgets(buf, sizeof buf, fp); // Cast to void to avoid return value warning
    sscanf(buf,"%d %g",&n, boxSize);
    return n;
};


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
      (void)*fgets(buf,sizeof buf,fp);
      sscanf(buf,"%g %g %g %g %d",&L[i].p[0],&L[i].p[1],&L[i].p[2],&L[i].m,&L[i].type);
      L[i].next = NULL;
      L[i].prev = NULL;
    }

    /* close the file when finished */
    fclose(fp);

};

float periodic_separation(struct liststruct *a, struct liststruct *b, float boxSize, int dim) {
    float t,d = 0;
    float boxHalf = boxSize / 2.;
    while (dim--) {
        t = b->p[dim] - a->p[dim];
        if (t > boxHalf) {
            t -= boxSize;
        }
        if (t < -boxHalf) {
            t += boxSize;
        }
        d += t*t;
    }
    //return sqrtf(d);
    return d; // returning the square to avoid the sqrt calculation.
};

void list_combine(struct liststruct *L, long i, long j) {

    /* Parts 2 & 3 */
    /* Hint -- this function should not need to know what types
       of particle are being linked */

    struct liststruct *starti, *startj;

    /* find the start of both lists */
    starti = &L[i];
    while (starti -> prev != NULL) {
        starti = starti->prev;
    }
    startj = &L[j];
    while (startj->prev !=NULL) {
        startj = startj->prev;
    }


    /* Both particles in same group already? */
    if (starti == startj) {
        return;
    }
    else {
        /* Go to end of i and join to start of j */
        while (starti->next != NULL) {
            starti = starti->next;
        }
        starti->next = startj;
        startj->prev = starti;
    }

};
