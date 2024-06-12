#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ExternalSorting.h"

int writeArray(int v[], int s, FILE *out){
    int i = 0;
    while(i < s){
        fprintf(out, "%d\n", v[i]);
        i++;
    }
    return 1;
}

int checkSortedFile(FILE *in){
    int a, b;
    if(fscanf(in, "%d", &a) == EOF) return 1;
    while(fscanf(in, "%d", &b) != EOF){
        if(a > b) return 0;
        a = b;
    }
    return 1;
}

void randomizeArray(int v[], int l, int r){
    int i = l, aux, ran;
    while(i < (r+1)){
        ran = rand()%(r+1-l);
        aux = v[i];
        v[i] = v[l+ran];
        v[l+ran] = aux;
        i++;
    }
}

int partitionArray(int v[], int l, int r){
    int i = l, j = l+1, aux;
    while(j < (r+1)){
        if(v[j] <= v[l]){
            i++;
            aux = v[i];
            v[i] = v[j];
            v[j] = aux;
        }
        j++;
    }
    aux = v[l];
    v[l] = v[i];
    v[i] = aux;

    return i;
}

void randomQuickSort(int v[], int l, int r){
    if(l < r){
        randomizeArray(v, l, r);
        int ip = partitionArray(v, l, r);
        randomQuickSort(v, l, ip-1);
        randomQuickSort(v, ip+1, r);
    }
}

void sort(int *v, int s){
    randomQuickSort(v, 0, s-1);
}

int main(int argc, char *argv[]){
    int i, *v, t;
    if(argc < 4){
        printf("Argumentos insuficientes.\nModo de uso: BucketShellSort {num_elementos} {arq_input} {arq_output} {--no-cleanup}\n");
        return -1;
    }
    t = (int)atoi(argv[1]);
    FILE *in = fopen(argv[2], "r");
    FILE *out = fopen(argv[3], "w");
    if(t <= SIZE_INTERNAL){
        v = (int*)malloc(t * sizeof(int));
        printf("Vetor de tamanho %d criado\n", t);
        for(i = 0; i < t; i++){
            if(fscanf(in, "%d", &v[i]) == EOF){
                printf("Arquivo chegou ao final antes do vetor ser preenchido.\n");
                return -1;
            }
        }
        printf("Vetor preenchido\n");
        sort(v, t);
        writeArray(v, t, out);
    } else {
        int k = createRuns(t, SIZE_INTERNAL, sort, in);
        if(k > 500){
            printf("Numero de elementos muito alto\n");
            return -2;
        }
        printf("%d runs criadas com tamanho <= %d\n", k, SIZE_INTERNAL);
        mergeRuns(k, t, out);
        printf("Runs mescladas\n");
        if(argc < 5 || strcmp(argv[4], "--no-cleanup")) cleanup(k);
    }
    printf("Vetor salvo em %s\n", argv[3]);
    fclose(in);
    fclose(out);
    /* checa se arquivo esta ordenado */
    in = fopen(argv[3], "r");
    if(checkSortedFile(in)) printf("Vetor ordenado\n");
    else printf("Vetor nao ordenado\n");
    fclose(in);
    return 0;
}
