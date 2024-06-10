#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ExternalSorting.h"

#define SIZE_INTERNAL 1000

int gaps[] = {1, 4, 10, 23, 57, 132, 301, 701, 1636, 3657, 8172, 18235, 40764, 91064, 203519, 454741, 1016156, 2270499, 5073398, 11335582, 25328324, 56518561, 126451290, 282544198, 631315018};

int ordenado(int v[], int i, int s){
    while(i < s){
        if(v[i] > v[i+1]) return 0;
        i++;
    }
    return 1;
}

int initgap(int size){
    int i = 1;
    while(gaps[i] < size && i < 25) i++;
    return i-1;
}

int lookup(int n){
    return gaps[n];
}

void shell(int v[], int size){
    int h, i, j, gap, temp;
    for(h = initgap(size); h > -1; h--){
        gap = lookup(h);
        for(i = gap; i < size; i++){
            temp = v[i];
            for(j = i; j >= gap && temp < v[j-gap]; j-=gap){
                v[j] = v[j-gap];
            }
            v[j] = temp;
        }
    }
}

struct bucket{
    int size;
    int max;
    int *bucket;
};
typedef struct bucket Bucket;

Bucket *createBuckets(int size, int bnum){
    int bsize = (size / bnum) + 1;
    Bucket *barray = (Bucket*)malloc(sizeof(Bucket) * bnum);
    int i = 0;
    while(i < bnum){
        barray[i].bucket = (int*)malloc(sizeof(int) * bsize);
        barray[i].size = 0;
        barray[i].max = bsize;
        i++;
    }
    return barray;
}

void resizeBucket(Bucket *b, int i){
    b[i].max *= 2;
    b[i].bucket = (int*)realloc(b[i].bucket, sizeof(int) * b[i].max);
}

void fillBuckets(int v[], int size, Bucket *b, int largest, int bnum){
    int i = 0;
    int bindex;
    while(i < size){
        bindex = (v[i])*bnum/(largest+1);
        if(b[bindex].size == b[bindex].max) resizeBucket(b, bindex);
        b[bindex].bucket[b[bindex].size] = v[i];
        b[bindex].size++;
        i++;
    }
}

void sortBuckets(Bucket *b, int bnum){
    int i = 0;
    while(i < bnum){       
        shell(b[i].bucket, b[i].size);
        i++;
    }
}

void returnBuckets(int v[], int size, Bucket *b, int bnum){
    int i = 0, j, k = 0;
    while(i < bnum){
        j = 0;
        while(j < b[i].size){
            v[k] = b[i].bucket[j];
            j++;
            k++;
        }
        i++;
    }
    for(i = 0; i < bnum; i++){
        free(b[i].bucket);
    }
    free(b);
}

int findLargest(int v[], int size){
    int i = 0, largest = 0;
    while(i < size){
        if(largest < v[i]) largest = v[i];
        i++;
    }
    return largest;
}

int findNumBuckets(int size){
    if(size < 100) return 1;
    return (size/100);
}

void sort(int *v, int t){
    int bnum, largest;
    bnum = findNumBuckets(t);
    Bucket *b = createBuckets(t, bnum);
    printf("%d buckets criados com tamanho %d\n", bnum, b[1].max);
    largest = findLargest(v, t);
    printf("Maior: %d\n", largest);
    fillBuckets(v, t, b, largest, bnum);
    printf("Buckets preenchidos\n");
    sortBuckets(b, bnum);
    printf("Buckets ordenados\n");
    returnBuckets(v, t, b, bnum);
    printf("Buckets retornados\n");
}

int main(int argc, char *argv[]){
    int i, *v, t;
    t = (int)atoi(argv[1]);
    FILE *arq = fopen(argv[2], "r");
    if(t <= SIZE_INTERNAL){
        v = (int*)malloc(t * sizeof(int));
        printf("Vetor de tamanho %d criado\n", t);
        for(i = 0; i < t; i++){
            if(fscanf(arq, "%d", &v[i]) == EOF){
                printf("Arquivo chegou ao final antes do vetor ser preenchido.\n");
            }
        }
        printf("Vetor preenchido\n");
        sort(v, t);
        if(ordenado(v, 0, t-1)) printf("Vetor ordenado\n");
        else printf("Vetor nao ordenado\n");
    } else {
        int k = createRuns(t, SIZE_INTERNAL, sort, arq);
        printf("%d\n", k);
        FILE *out = fopen("./output.txt", "w");
        mergeRuns(k, t, out);
    }
    return 0;
}
