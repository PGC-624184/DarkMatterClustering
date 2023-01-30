/*
  Friends of Friends via hash grid
 */

#include "fof.h"
#include <math.h>
#include <stdlib.h>


#ifdef DEBUG
  #include <stdio.h>
static int max_stack_usage, pt_cmp, fill_collisions, search_collisions, ngb_found;
#endif

// A walk over the 58 cells within sqrt(3) cell widths, and having idx<me, 
// where idx(i,j,k)=Mi+Nj+k
#define WALK_NGB_SIZE 58
#define WALK_NGB(M,N) {\
    M, 1, N, M+N, M-N, M-1, M+1, N+1, N-1, M-N+1, M+N+1, M-N-1, M+N-1, 2*M, 2*N,\
      2, 2*M+N, M+2*N, 2*M-N, M-2*N, N+2, N-2, M+2, 2*M+1, M-2, 2*N-1, 2*N+1, \
      2*M-1, 2*M-N-1, 2*M+N-1, M+2*N-1, M-2*N+1, M+N-2, 2*M-N+1, 2*M+N+1, \
      M+2*N+1, M-N+2, M+N+2, M-N-2, M-2*N-1, 2*M+2*N, 2*N-2, 2*M-2*N, 2*M-2, \
      2*M+2, 2*N+2, 2*M+N-2, 2*M-N-2, M+2*N+2, 2*M-2*N-1, 2*M+2*N-1, 2*M-2*N+1, \
      2*M+2*N+1, M-2*N+2, 2*M-N+2, 2*M+N+2, M+2*N-2, M-2*N-2}

// hashed position of the neighbour (i.e. (ngb*prime) mod N)
#define HASH_NGB(A,P,M) {\
    A[0]*P&M, A[1]*P&M, A[2]*P&M, A[3]*P&M, A[4]*P&M, A[5]*P&M, A[6]*P&M, \
      A[7]*P&M, A[8]*P&M, A[9]*P&M, A[10]*P&M, A[11]*P&M, A[12]*P&M, A[13]*P&M,\
      A[14]*P&M, A[15]*P&M, A[16]*P&M, A[17]*P&M, A[18]*P&M, A[19]*P&M, \
      A[20]*P&M, A[21]*P&M, A[22]*P&M, A[23]*P&M, A[24]*P&M, A[25]*P&M, \
      A[26]*P&M, A[27]*P&M, A[28]*P&M, A[29]*P&M, A[30]*P&M, A[31]*P&M, \
      A[32]*P&M, A[33]*P&M, A[34]*P&M, A[35]*P&M, A[36]*P&M, A[37]*P&M, \
      A[38]*P&M, A[39]*P&M, A[40]*P&M, A[41]*P&M, A[42]*P&M, A[43]*P&M, \
      A[44]*P&M, A[45]*P&M, A[46]*P&M, A[47]*P&M, A[48]*P&M, A[49]*P&M, \
      A[50]*P&M, A[51]*P&M, A[52]*P&M, A[53]*P&M, A[54]*P&M, A[55]*P&M, \
      A[56]*P&M, A[57]*P&M}

// Magic number that specifies how full the hash-table should be to be most efficient
// Hash tables perform very badly beyond about 0.7
// We use a lower load since spatial hashing uses lots of speculative searches
#define DESIRED_LOAD 0.3 


typedef struct {
  uint32_t parent, start; // Parent (disjoint set) and start point per cell
} CellParentStart;

typedef struct {
  int64_t cell; // ID of cell
  uint32_t ancestor, // Hint for root
    idx; // index of cell (when ordered)
} HashCell;







