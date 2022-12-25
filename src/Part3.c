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

/* Structures */

/* Prototypes */

/* Moved functions and structs into cleaner structure */
#include <definitions.h>
#include <read_data_n.h>
#include <read_data.h>
#include <periodic_separation.h>
#include <list_combine.h>
#include <Nodes.h>

#define MAX_SEP 5

/* main program */
int main(int argc, char *argv[]) {

    int i,j;
    int n;
    float sep, boxSize;
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
    List = (struct liststruct *) malloc(n * sizeof(struct liststruct));

    if (List == NULL)
        return -2;

    // read the data
    read_data(fp,List,n);

    /* Part 2 */
    /* get separation of each DM pair and form groups */
    /* Hint -- only need to include DM particles in these loops */

    // Timing calls
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    // I hate this section of code. ~O(n^2)
    for (i=0;i<n-1;i++) { //loop through particles
        if (List[i].type==1){ //only do more if DM
            for (j=i+1;j<n;j++) { // loop through remaining particles
                if (List[j].type==1) { //
                    sep = periodic_separation(&List[i],&List[j],boxSize,3);
                    if (sep <= MAX_SEP) {
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
    printf("The Dark Matter calculation takes %f seconds\n",cpu_time_used );

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

    /*
    // some defs
    struct kd_node_t testNode;
    struct kd_node_t *root, *found, *million;
    // create memory for binary tree
    million =(struct kd_node_t*) malloc(numDM * sizeof(struct kd_node_t));

    //allocate DM points to tree
    for (i = 0; i < numDM; i++) {
        if (List[i].type==1) {
            million[i].x[0] = List[i].p[0];
            million[i].x[1] = List[i].p[1];
            million[i].x[2] = List[i].p[2];
            million[i].group = List[i].group;
        }
    };
    // make the binary tree
    root = make_tree(million, numDM, 0, 3);
    visited=0;
    for (j=0;j<n;j++){
        double best_dist;
        // Baryonic matter point
        testNode.x[0] = List[j].p[0];
        testNode.x[1] = List[j].p[1];
        testNode.x[2] = List[j].p[2];
        found = 0;
        nearest(root, &testNode, 0, 3, &found, &best_dist);
        List[j].group = found->group;

        printf("\rsearching %d dist %g group %d",j,
        sqrt(best_dist),found->group);
        // need to get DM group an allocate to
    };
    printf("\n");
    free(million);
    */

    /* Part 3 */
    /* Add baryons to group of nearest DM part */
    /* Hint -- similar to above but one loop for baryons,
       one loop for DM.
       Hint -- when should you call list_combine now?
    */
    start = clock();
    /*
    I hate this section of code. Tried to implement kd tree above but fell back
    to this as I know it works and gives the answer.
    */
    for (i=0;i<n;i++) { // loop through all particles
        int bestindex;
        float dist[2]={boxSize,boxSize};
        if (List[i].type !=1 ) {
            for (j=0;j<n;j++) {
                if (List[j].type==1) {
                    dist[1] = periodic_separation(&List[i],&List[j],boxSize,3);
                    if (dist[1]<dist[0]){ // update closest distance index
                        bestindex = j; // get closest match index
                        dist[0] = dist[1]; // update dist
                    }
                }
            }
        list_combine(List,i,bestindex);
        }
        printf("\rComputing closest DM for particle %d of %d",i+1,n);
        fflush(stdout);
    }

    struct liststruct **Start = (struct liststruct **) malloc(n * sizeof(struct liststruct *));
    if(!Start) {
        printf("Couldn't allocate memory for Start array\n");
        return -3;
    }

    int count = 0;
    for(i=0; i<n; i++) {
        if(List[i].prev == NULL && List[i].next != NULL) {
            Start[count++] = &List[i];
        }
    }
    printf("\nThere are %d groups.\n",count);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("The Baryon calculation takes %f seconds\n",cpu_time_used );

    /* Part 4 */
    /* Do some analysis on each group */

    float Mass[count];
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

    int p;
    for(p=0;p<count;p++) {
        for(Mass[p]=0,l=Start[p]; l != NULL; l = l->next) {
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
    printf("Found %d massive groups. Largest group was %g\n",massive_count,bigMass);


    /* Part 4 */
    /* Open file and write data*/
    char outfile[30] = "analysis.csv";
    FILE *f = fopen(outfile, "w");
    fprintf(f,"Total_Mass, Gas_Mass, DM_Mass, Star_Mass, BH_Mass\n");
    for(i=0;i<count;i++) {
      if (Star[i]>=MassLimit) {
        // Write masses to file
        fprintf(f,"%g, %g, %g, %g, %g\n",log10(Mass[i]),log10(Gas[i]),log10(DM[i]),log10(Star[i]),log10(BH[i]));
        //printf("%d\n",i);
      }
    }
    fclose(f);

    //clean up
    free(Start);
    free(List);

    return 0;
};
