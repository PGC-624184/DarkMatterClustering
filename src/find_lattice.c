#include <find_lattice.h>

void find_lattice(const double *pos, const uint32_t num_pos, const double inv_cell_width, const int NY, const int64_t NX, int64_t *out) {
  /*
    Find the bucket index (int)(p[0]*inv_cell_width)*nx + (int)(p[1]*inv_cell_width)*ny + (int)(p[2]*inv_cell_width)
    for every point
   */
    for (size_t i=0;i<num_pos;i++) {
        out[i] = (int64_t)(pos[i*3]*inv_cell_width)*NX + (int64_t)(pos[i*3+1]*inv_cell_width)*NY + (int64_t)(pos[i*3+2]*inv_cell_width);
    }
}