#define AUTOR "Marcelo Passamai Mendes"
#define DATA "16/11/2022"
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define vetor_size 32
#define num_threads 4


typedef struct data_t{
    int id;
    int qte_somada;
    int soma_parcial;
    pthread_mutex_t* mutex;
    int* pos;
    int* vetor;
}data_t;

void* thread_func(void* arg){
    data_t* pkg = (data_t*)arg;
    int local_index;
    do {
        pthread_mutex_lock(pkg->mutex);
        printf("mutou\n");
        local_index = *pkg->pos;
        pkg->pos++;
        pthread_mutex_unlock(pkg->mutex);
        if (local_index< vetor_size){
            pkg->soma_parcial += pkg->vetor[local_index]; 
        }
    } while(local_index< vetor_size);

    //pthread_mutex_lock(&mutex1);
    //sum += partial_sum;
    //pthread_mutex_unlock(&mutex1);
}

int main(){
    int i;
    int vetor[vetor_size];
    int* pos = (int*) malloc(1*sizeof(int)); 
    pthread_t* lst_thread = (pthread_t*)malloc(num_threads*sizeof(pthread_t));
    data_t* lst_data = (data_t*)malloc(num_threads * sizeof(data_t));
    pthread_mutex_t* lst_mutex = (pthread_mutex_t*)malloc(num_threads * sizeof(pthread_mutex_t));

    *pos = 0;

    for( i=0 ; i<vetor_size ; i++){
        vetor[i] = i+1;
    }
    // iniciando os mutexes
    for(i=0; i<num_threads ;i++){
        pthread_mutex_init(&lst_mutex[i],NULL);
    }

    //prenchendo array de dataframe
    for(i=0 ; i<num_threads ; i++){
        lst_data[i].id = 0;
        lst_data[i].qte_somada = 0;
        lst_data[i].soma_parcial = 0;
        lst_data[i].pos = pos;
        lst_data[i].vetor = vetor;
        lst_data[i].mutex = &lst_mutex[i];
    }

    for(i=0; i<num_threads ;i++){
        pthread_create(&lst_thread[i] , NULL , thread_func , &lst_data[i]);
    }

    for(i=0; i<num_threads ;i++){
        pthread_join(lst_thread[i] , NULL);
    }
    
    int total = 0;
    for(i<0;i<num_threads;i++){
       printf("%d",lst_data[i].soma_parcial);
    }
}