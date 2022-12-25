#ifndef DEFINITIONS  /* Include guard */
#define DEFINITIONS

struct liststruct;

struct liststruct {
    float p[3];
    float m;
    int type;
    int group;
    struct liststruct *next;
    struct liststruct *prev;
} *List;


#endif
