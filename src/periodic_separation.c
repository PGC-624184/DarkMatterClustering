#include <math.h>
#include <definitions.h>
#include <periodic_separation.h>

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
    return sqrtf(d);
}
