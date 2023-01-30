#include <connected_pwise.h>

static int connected_pwise( const int cur_size, 
                            const int adj_size,
			                const double b2,
			                const int64_t* restrict cur_idx,
			                const int64_t *restrict adj_idx,
			                const double *restrict xyz) {
    /*
        Check pairwise for any connections, i.e. any point p1 in my
        cell within b of any point p2 in adj cell

        Returns 1 if connected, 0 otherwise
    */

    for (int my_k=cur_size;my_k;--my_k, cur_idx++) {
        const double *xyz1 = &xyz[(*cur_idx)*3];
        const int64_t* p2 = adj_idx;      
        for (int adj_k=adj_size; adj_k; --adj_k, p2++) {
	        const double *xyz2 = &xyz[(*p2)*3];
	        const double dx = xyz2[0]-xyz1[0],
	        dy = xyz2[1]-xyz1[1],
	        dz = xyz2[2]-xyz1[2];
            #ifdef DEBUG		      
	            pt_cmp++; // Count comparisons
            #endif
	        if ((dx*dx + dy*dy + dz*dz) > b2)
	            continue;
            return 1;
	    }
    }
    return 0;
};