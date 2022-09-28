#define AUTOR "Marcelo Passamai Mendes"
#define DATA "20/10/2022"
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

typedef struct mesa_t{
    int mesa[6];
    pthread_mutex_t mutex;
}

void* frabrica_insumos(void* arg);
void* consumo_vacina(void* arg);

int main( int argc, char *argv[ ] ){
    if(argc!=2){
        printf("Parametros invalidos por favor tente novamente!");
    }else{
        int rodadas = atoi(argv[1]);
        int i;
        // [vm, vm, ij, ij, is, is]
        int mesa[6];
        for(i=0;i<6;i++){
            mesa[i]=0;
        }
        pthread_mutex_t mutex;
        pthread_mutex_init(&mutex, NULL);
  
    
    }
    return 0;
}