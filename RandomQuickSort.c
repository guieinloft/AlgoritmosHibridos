#include <stdio.h>
#include <stdlib.h>

void imprimevet(int v[], int t){
    int i = 0;
    while(i < t){
        printf("%d ", v[i]);
        i++;
    }
    putchar('\n');
}

int ordenado(int v[], int l, int r){
    int i = l;
    while(i < r){
        if(v[i] > v[i+1]) return 0;
        i++;
    }
    return 1;
}

void aleatoriza(int v[], int l, int r){
    int i = l, aux, ran;
    while(i < (r+1)){
        ran = rand()%(r+1-l);
        aux = v[i];
        v[i] = v[l+ran];
        v[l+ran] = aux;
        i++;
    }
}

int particiona(int v[], int l, int r){
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

void rquick(int v[], int l, int r){
    if(l < r){
        aleatoriza(v, l, r);
        int ip = particiona(v, l, r);
        rquick(v, l, ip-1);
        rquick(v, ip+1, r);
    }
}

int main(int argc, char *argv[]){
    int i, *v, aux, t;
    t = (int)atoi(argv[1]);
    v = (int*)malloc(t * sizeof(int));
    printf("Vetor de tamanho %d criado\n", t);
    FILE *arq = fopen(argv[2], "r");
    for(i = 0; i < t; i++){
        if(fscanf(arq, "%d", &v[i]) == EOF){
            printf("Arquivo chegou ao final antes do vetor ser preenchido.\n");
        }
    }
    printf("Vetor preenchido\n");
    rquick(v, 0, t-1);
    if(ordenado(v, 0, t-1)) printf("Vetor ordenado\n");
    else printf("Vetor nao ordenado\n");
    return 0;
}
