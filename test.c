#include <stdio.h>
#include <stdlib.h>

int main(){
    char str[11];
    for(int i = 0; i < 50; i++){
        sprintf(str, "temp%02d.txt", i);
        FILE *arq = fopen(str, "r");
        printf("%X\n", arq);
        int aux;
        while(fscanf(arq, "%d", &aux) != EOF) printf("%d\n", aux);
    }
    return 0;
}
