#define author 	"Marcelo Passamai Mendes"
#define date 	"11/09/2022"
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define SIZE 100000
#define N_THREADS 4

typedef struct 
{
    int id;
    long int soma;
    int* p_vetor;
}dataframe_t;


void* funcao_thread (void* args){
    dataframe_t *df_t = ((dataframe_t *) args);
    printf("%d\n",df_t->id);
}


int main(){
    int vetor[SIZE];
    long int soma = 0;
    for(int i=0 ; i<SIZE; i++){
        vetor[i]=i+1;         
    }

    clock_t m_1 = clock();
    for(int i=0 ; i<SIZE; i++){
        soma = soma + vetor[i];         
    }
    clock_t m_2 = clock();
    double Tempo = (m_2 - m_1) * 1000.0 / CLOCKS_PER_SEC;
    printf("Soma iterativa %ld\n", soma);
    printf("Tempo gasto: %g ms.", Tempo);
    
    // Com Threads
    pthread_t tid;
    dataframe_t df;
    df.id=1;
    df.soma=0;
    df.p_vetor = vetor; 

    pthread_create(&tid,NULL, funcao_thread, &df);
    pthread_join(tid, NULL);   

    system("read -p 'Press Enter to continue...' var");
    
    
    return 0;
}
