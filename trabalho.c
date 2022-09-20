#define AUTOR "Marcelo Passamai Mendes"
#define DATA "20/10/2022"
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

typedef struct construtor_t{
    int id;
    int* mesa;  //[vm,vm,ij,ij,is,is]   
    int valor_nao_insert;
    pthread_t thread;
    pthread_mutex_t mutex;
} construtor_t;

typedef struct consumidor_t{
    int id;
    int* mesa; //[vm,vm,ij,ij,is,is]
    int cond_parada;
    int insumo_proprio; //012
    pthread_t thread;
    pthread_mutex_t mutex;
}consumidor_t;

void* frabrica_insumos(void* arg);
void* consumo_vacina(void* arg);

int main( int argc, char *argv[ ] ){
    if(argc!=2){
        printf("Parametros invalidos por favor tente novamente!");
    }else{
        int rodadas = atoi(argv[1]);
        int i;
        int N_CONSTRUTOR,N_CONSUMIDOR = 3;
        int mesa[6]; //[vm,vm,ij,ij,is,is]
        for(i=0;i<6;i++){
            mesa[i]=0;
        }
        pthread_mutex_t mutex;
        pthread_mutex_init(&mutex, NULL);
        //---------------------------------------
        pthread_t thread_construtor[N_CONSTRUTOR];
        construtor_t laboratorio[N_CONSTRUTOR];
        for(i=0;i<N_CONSTRUTOR;i++){
            construtor_t* lab;
            lab = malloc(sizeof(construtor_t));
            lab->id = i;
            lab->mesa = mesa;
            lab->valor_nao_insert =i;
            lab->thread = thread_construtor[i];
            lab->mutex = mutex;
        }
        //-----------------------------------
        
        for(i=0;i<N_CONSUMIDOR;i++){

        }
        //--------------logica---------


  
    
    }
    return 0;
}

        /*
        pthread_mutex_t mutex;
        pthread_mutex_init(&mutex, NULL);
        pthread_t thread;
        construtor_t* pkg;
        pkg = malloc(sizeof(construtor_t));
        pkg->id=0;
        pkg->mesa = mesa;
        pkg->valor_nao_insert = 0;
        pkg->thread = thread;
        pkg->mutex = mutex;
        */