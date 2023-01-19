#ifndef DEFINITIONS  /* Include guard */
#define DEFINITIONS

//extern struct liststruct;

extern struct liststruct {
    float p[3];
    float m;
    int type;
    int group;
    struct liststruct *next;
    struct liststruct *prev;
} *List;

#endif