/*faca um codigo simples no VS que crie 5 threads e execute todas elas imprimindo seu proprio ID de forma a visualizar a desordem das impressoes fazendo com q o codigo principal espere ate que todas as 5 threads terminem.
ps: somente cria UMA funcao thread, nao vale se usar 5 funcoes de thread.
SOMENTE O .C DEVE SER ENVIADO.*/
#define AUTOR "Marcelo Passamai Mendes"
#define DATA "16/11/2022"
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#define tam 5

typedef struct data_t{
    int id;
}data_t;


void * thread_func(void* arg){
    data_t* pkg = (data_t*)arg;
    printf("%d\n", pkg->id);
}

int main(){
    int i;
    pthread_t* lst_thread;
    data_t* lst_data;

    lst_data = (data_t*)malloc(tam*sizeof(data_t)) ;
    lst_thread = (pthread_t*)malloc(tam*sizeof(pthread_t)) ;

    printf("Inicio do Programa \n"); 
    for(i=0;i<tam;i++){
        lst_data[i].id = i+1;
    }    
    //criando as threads
    for(i=0;i<tam;i++){
        pthread_create(&lst_thread[i], NULL, thread_func, &lst_data[i]);
    }
    //criando as threads
    for(i=0;i<tam;i++){
        pthread_join(lst_thread[i], NULL);
    }
    printf("Fim do Programa \n"); 
    free(lst_data);
    free(lst_thread);
    return 1;
}