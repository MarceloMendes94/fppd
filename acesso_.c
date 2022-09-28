#define AUTOR "Marcelo Passamai Mendes"
#define DATA "21/10/2022"
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

typedef struct var_compatilhada{
    int valor;
    pthread_mutex_t mutex;
    int flag; //0  escrever e não ler !=0 pode ler e não pode escrever
    int parada; //if igual 1 matar o programa; em caso de semaforos
    int count;
}var_compatilhada;

void* thread_escrita(void* arg){
    var_compatilhada* pck = (var_compatilhada*) arg;
    
    do {
        pthread_mutex_lock(&pck->mutex);
        //trecho travado
        pck->valor++;
        pck->flag = 0;
        pthread_mutex_unlock(&pck->mutex);
    } while( pck->flag == 0 );

    return (NULL);
}

void* thread_leitura(void* arg){
   var_compatilhada* pck = (var_compatilhada*) arg;
    
    do {
        pthread_mutex_lock(&pck->mutex);
        //trecho travado
        pck->valor--;
        pck->flag = 1;
        pthread_mutex_unlock(&pck->mutex);
    } while( pck->flag != 0 );

    return (NULL);
}

int main(){
    printf("teste");
    pthread_mutex_t mutex;    
    pthread_mutex_init(&mutex, NULL);
    
    var_compatilhada* pkg;
    pkg = malloc(sizeof(var_compatilhada));
    pkg-> valor = 0;    
    pkg-> mutex = mutex;
    pkg-> flag = 1;
    pkg-> parada = 0;
    pkg-> count = 99;  

    pthread_t escrita, leitura;
    printf("teste");


    /*
    pthread_create(&escrita,NULL,thread_escrita,&pkg);
    pthread_create(&leitura,NULL,thread_leitura,&pkg);

    if (pthread_join(escrita,NULL) !=0 ){
        perror("Pthread JOIN falhou");
        exit(-1);
    }
    if (pthread_join(escrita,NULL) !=0 ){
        perror("Pthread JOIN falhou");
        exit(-2);
    }*/
    return 0;
}