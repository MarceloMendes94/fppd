#define AUTOR "Marcelo Passamai Mendes"
#define DATA "28/10/2022"
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

typedef struct data_t{
    int id;
}data_t;

void* funcao_thread(void* arg){
    data_t* pkg = (data_t*)arg;
    printf("%d",pkg->id);
    return NULL;
} 

int main(){
    data_t* pkg;
    pkg=malloc(sizeof(data_t));
    pkg->id=1;
    pthread_t th;
    pthread_create(&th,NULL,funcao_thread,(void*)pkg);
    pthread_join(th,NULL);
    return 0;
}