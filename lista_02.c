#define AUTOR "Marcelo Passamai Mendes"
#define DATA "16/11/2022"
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define vetor_size 10000
#define num_threads 10


typedef struct data_t{
    int id;
    int qte_somada;
    int soma_parcial;
    pthread_mutex_t* mutex;
    int* pos;
    int* vetor;
}data_t;



void* thread_func_2(void* arg){
    data_t* pkg = (data_t*)arg;
    int* local_pos;
    do {
        pthread_mutex_lock(pkg->mutex);
        local_pos = pkg->pos;
        (*pkg->pos)++;    
        //pthread_mutex_unlock(pkg->mutex);
        
        if((*local_pos) <= vetor_size){
           // pthread_mutex_lock(pkg->mutex);
            pkg->soma_parcial = pkg->soma_parcial+ pkg->vetor[*local_pos];
            pkg->qte_somada++;
            pthread_mutex_unlock(pkg->mutex);
        }       
        
    } while((*local_pos) <=vetor_size);    
}




int main(){
    int i;
    
    int vetor[vetor_size+1];
    for( i=0 ; i<vetor_size+1 ; i++){
        vetor[i] = i;
    }

    pthread_t* lst_thread = (pthread_t*)malloc(num_threads*sizeof(pthread_t));
    data_t* lst_data = (data_t*)malloc(num_threads * sizeof(data_t));
    
    pthread_mutex_t* lst_mutex = (pthread_mutex_t*)malloc(num_threads * sizeof(pthread_mutex_t));
    // iniciando os mutexes
    for(i=0; i<num_threads ;i++){
        pthread_mutex_init(&lst_mutex[i],NULL);
    } 

    int* pos = (int*) malloc(1*sizeof(int)); 
    *pos = 0;

    //prenchendo array de dataframe
    for(i=0 ; i<num_threads ; i++){
        lst_data[i].id = i;
        lst_data[i].qte_somada = 0;
        lst_data[i].soma_parcial = 0;
        lst_data[i].pos = pos;
        lst_data[i].vetor = vetor;
        lst_data[i].mutex = &lst_mutex[i];
    }

 
    // cria as thread
    for(i=0; i<num_threads ;i++){
        pthread_create(&lst_thread[i] , NULL , thread_func_2 , &lst_data[i]);
    }
    //junta os resultados
    for(i=0; i<num_threads ;i++){
        pthread_join(lst_thread[i] , NULL);
    }
    
    for(i=0 ; i<num_threads ; i++){
        printf("id:%d| qte:%d | soma:%d\n",lst_data[i].id,lst_data[i].qte_somada , lst_data[i].soma_parcial);
    }
    int total=0;
    for(i=0 ; i<num_threads ; i++){
        total = total+ lst_data[i].qte_somada;
        
    }
    printf("---------\ntotal:%d\n",total); 

    free(lst_thread);
    free(lst_data);
    free(lst_mutex);
    free(pos);

    return 0;
}