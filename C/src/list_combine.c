#include <stdio.h>
#include <definitions.h>
#include <list_combine.h>


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

}
