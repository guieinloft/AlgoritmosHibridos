#include <stdlib.h>
#include <stdio.h>

struct element{
    int v;
    FILE *file;
    int index;
    int num;
};
typedef struct element Element;

struct heap{
    Element *vet;
    int size;
    int capacity;
};
typedef struct heap Heap;


Heap *createHeap(int k);

void openTemps(Heap *h);

int createRuns(int nelements, int sizeruns, void (*sort)(int*, int), FILE *input);

void mergeRuns(int size, int nelements, FILE *output);

void freeHeap(Heap *heap);
