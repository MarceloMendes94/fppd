#define AUTOR "Marcelo Passamai Mendes"
#define DATE "20/10/2022"
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

typedef struct thread_info_sem{
    pthread_t   thread_id;
    int         thread_num;
    long        lretorno;
    sem_t       *semaph_count;
} thread_info_sem;

void* thrdSem(void* arg){
    long i = -1;
    thread_info_sem* pacote = (thread_info_sem*) arg;
    i = ((pacote->thread_num)+1)*10;    
    sem_wait(pacote->semaph_count);
    pacote->lretorno = i;
    return 0;
}

int main(int argc, char** arg)
{
    unsigned int nThreads;
    unsigned long int i,j;
    nThreads = 24;

    sem_t semaph_counter;
    sem_init(&semaph_counter,0,0);

    thread_info_sem* pacote_thread;
    pacote_thread =  malloc(sizeof(thread_info_sem) * nThreads);
    
    if(pacote_thread==NULL){
        return -1;
    }
    for (i=0; i<nThreads; i++ ){
        (pacote_thread + i)->thread_num = i;
        (pacote_thread + i)->lretorno = -1;
        (pacote_thread + i)->semaph_count = &semaph_counter;
    }
    for (i=0; i<nThreads; i++){
        pthread_create(&((pacote_thread+i)->thread_id) , NULL, thrdSem, pacote_thread+i);
    }
    for(i=0; i<nThreads; i++){
        sem_post(&semaph_counter);
        for(j=0; j<nThreads ;j++){
            if(pacote_thread[j].lretorno != -1){//gambiarra
                printf("thread %d terminou com valor %ld \n",pacote_thread[j].thread_num,pacote_thread[j].lretorno);
                pacote_thread[j].lretorno = -1;
                break;
            }
        }
    }
    free(pacote_thread);
    sem_destroy(&semaph_counter);
    return 0;
}