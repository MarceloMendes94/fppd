#define AUTOR "Marcelo Passamai Mendes"
#define DATE "20/10/2022"
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

// MUTEXES é uma esturutura de thread que bloqueia acesso a um determinado momento.
#define TAM 1000
#define NTHREADS 10

int a[TAM];
int global_index = 0;
int sum = 0;
pthread_mutex_t mutex1;

void* slave(void* nenhum){
    int local_index, partial_sum = 0 ;
    
    do {
        pthread_mutex_lock(&mutex1);
        local_index = global_index;
        global_index ++;
        pthread_mutex_unlock(&mutex1);
        if (local_index< TAM){
            partial_sum += a[local_index]; 
        }
    } while(local_index< TAM);

    pthread_mutex_lock(&mutex1);
    sum += partial_sum;
    pthread_mutex_unlock(&mutex1);

    return (NULL);
}



int main(){
    int i;
    pthread_t thread[NTHREADS];

    pthread_mutex_init(&mutex1, NULL);
    
    for(i=0; i<TAM; i++)
    {
        a[i] = i+1;
    }
    // CREATE
    for(i=0; i<NTHREADS; i++)
    {
        if (pthread_create(&thread[i],NULL,slave,NULL) !=0 ){
            perror("Pthread CREATE falhou");
            exit(-1);
        }
    }
    // JOIN
    for(i=0; i<NTHREADS; i++)
    {
        if (pthread_join(thread[i],NULL) !=0 ){
            perror("Pthread JOIN falhou");
            exit(-2);
        }
    }
    printf("A soma eh: %d\n",sum);
    /*system("pause");*/
    return 0;
}