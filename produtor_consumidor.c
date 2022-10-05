#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int counter;
pthread_t tid;

void *producer(void *param);
void *consumer(void *param);

void initializeData(){
    counter = 0;
}



int insert_item(int item){
        if( counter < BUFFER_SIZE ){
        buffer[counter] = item;
        counter++;
        return 0;
    }else{
        return 1;
    }
}

int remove_item(int *item){
    if ( counter > 0 ){
        *item = buffer[(counter-1)];
        counter--;
        return 0;
    }else{
        return 1;
    }
}

void *producer(void *param){
    int item;
    while(1){
        sleep(1);
        item = rand()/ 10000000;
        if(!insert_item(item)){
            printf("produced: %d\n",item);
        }else{
            fprintf(stderr, "Erro ao produzir.\n");
        }

    }
}

void *consumer(void *param){
    int item;
    while(1){
        sleep(1);
        if(!remove_item(&item)){
            printf("consumed: %d\n",item);
        }else{
            fprintf(stderr, "Erro ao Consumir.\n");
        }

    }
}



int main(){
    srand((unsigned int) time(NULL));
    
    int mainSleepTime = 5;
    int numProd = 3;
    int numCons = 3;

    initializeData();

    for(int i=0; i<numProd;i++){
        pthread_create(&tid,NULL,producer,NULL);
    }

    for(int i=0; i<numCons;i++){
        pthread_create(&tid,NULL,consumer,NULL);
    }
    
    sleep(mainSleepTime);
    
    return 0;
}