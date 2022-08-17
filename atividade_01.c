#define author 	"Marcelo Passamai Mendes"
#define date 	"11/09/2022"
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define SIZE 900000
#define N_THREADS 2

typedef struct 
{
    int id;
    long int soma;
    int* p_vetor;
}dataframe_t;


void* funcao_thread (void* args){
    dataframe_t *df_t = ((dataframe_t *) args);
    printf("Thread em execução(%d)...\n",df_t->id);
    int inicio = (SIZE/N_THREADS)* (df_t->id-1);
    int fim = (SIZE/N_THREADS)* (df_t->id);
    for(int i=inicio;i<fim;i++){
        df_t->soma = df_t->soma + df_t->p_vetor[i];
    }
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
    printf("Tempo gasto: %g ms\n", Tempo);
    
    // Com Threads
    pthread_t lst_thread[N_THREADS];
    dataframe_t lst_df[N_THREADS];

    clock_t m_3 = clock();  
    for(int i=0;i<N_THREADS;i++){
        lst_df[i].id = i+1;
        lst_df[i].soma = 0;
        lst_df[i].p_vetor = vetor;
        pthread_create(&lst_thread[i],NULL, funcao_thread, &lst_df[i]);
    }
    long int soma_multi = 0;
    for(int i=0;i<N_THREADS;i++){
        pthread_join(lst_thread[i], NULL); 
        soma_multi = soma_multi + lst_df[i].soma;
    }

    printf("soma multhread some é:%ld\n",soma_multi);
    clock_t m_4 = clock();
    double Tempo_2 = (m_4 - m_3) * 1000.0 / CLOCKS_PER_SEC;
    printf("Tempo gasto: %g ms\n", Tempo_2);
    system("read -p 'Press Enter to continue...' var");
    return 0;
}
