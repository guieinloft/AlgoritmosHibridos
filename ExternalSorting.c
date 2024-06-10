#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "ExternalSorting.h"

Heap *createHeap(int size){
    Heap *h = (Heap*)malloc(sizeof(Heap));
    h->vet = (Element*)malloc(sizeof(Element) * size);
    h->size = size;
    h->capacity = size;
    return h;
}

void openTemps(Heap *h){
    char str[11];
    int i = 0;
    while(i < h->size){
        sprintf(str, "temp%02d.txt", i);
        h->vet[i].file = fopen(str, "r");
        if(h->vet[i].file == NULL) printf("ops\n");
        fscanf(h->vet[i].file, "%d", &(h->vet[i].v));
        h->vet[i].index = i;
        h->vet[i].num = 1;
        i++;
    }
}

int left(int i){
    return 2*i+1;
}

int right(int i){
    return 2*i+2;
}

void swap(Element *a, Element *b){
    Element temp = *a;
    *a = *b;
    *b = temp;
}


/* heapify assume que elementos acima de i ja estejam de acordo com o heap */
void heapify(Heap *h, int i){
    int smallest = i;
    int l = left(i);
    int r = right(i);
    if(l < h->size && h->vet[smallest].v > h->vet[l].v) smallest = l;
    if(r < h->size && h->vet[smallest].v > h->vet[r].v) smallest = r;

    if(smallest != i){
        swap(&(h->vet[smallest]), &(h->vet[i]));
        heapify(h, smallest);
    }
}

void buildHeap(Heap *h){
    int i = (h->size)-1;
    while(i > -1){
        heapify(h, i);
        i--;
    }
}

int createRuns(int nelements, int sizeruns, void (*sort)(int*, int), FILE *input){
    int size = nelements/sizeruns + ((float)nelements/sizeruns > nelements/sizeruns);
    int i = 0, j, k, aux;
    int *v = (int*)malloc(sizeof(int) * sizeruns);
    char str[11];
    FILE *temp;
    while(i < size){
        sprintf(str, "temp%02d.txt", i);
        j = 0;
        k = 0;
        while(j < sizeruns && fscanf(input, "%d", &aux) != EOF){
            v[j] = aux;
            j++;
        }
        sort(v, j);
        temp = fopen(str, "w");
        while(k < j){
            fprintf(temp, "%d\n", v[k]);
            k++;
        }
        fclose(temp);
        i++;
    }
    free(v);
    return size;
}

void freeHeap(Heap *h){
    int i = 0;
    while(i < h->capacity){
        fclose(h->vet[i].file);
        i++;
    }
    free(h->vet);
    free(h);
}

void cleanup(int size){
    int i = 0;
    char str[11];
    while(i < size){
        sprintf(str, "temp%02d.txt", i);
        remove(str);
        i++;
    }
}

void mergeRuns(int size, int nelements, FILE *output){
    int i = 0, aux;
    Heap *h = createHeap(size);
    openTemps(h);
    buildHeap(h);
    while(i < nelements && h->size > 0){
        fprintf(output, "%d\n", h->vet[0].v);
        if(fscanf(h->vet[0].file, "%d", &aux) != EOF){
            h->vet[0].v = aux;
            h->vet[0].num++;
        } else {
            (h->size)--;
            swap(&(h->vet[0]), &(h->vet[h->size]));
        }
        heapify(h, 0);
    }
    freeHeap(h);
}
