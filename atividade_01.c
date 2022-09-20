#define author 	"Marcelo Passamai Mendes"
#define date 	"11/09/2022"
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define SIZE 10000000 
#define N_THREADS 2

typedef struct 
{
    int id;
    long int soma;
    int* p_vetor;
    int* size;
    int n_threads;/// pelamorrr de deus não vamos usar mais de 1 milhão de threaads
}dataframe_t;


void* funcao_thread (void* args){
    dataframe_t *df_t = ((dataframe_t *) args);
    printf("Thread em execução(%d)...\n",df_t->id);
    int * inicio = malloc(sizeof(int)* ( (*df_t->size)/df_t->n_threads));
    int * fim    = malloc(sizeof(int)* ( (*df_t->size)/df_t->n_threads));

    *inicio = ( (*df_t->size)/df_t->n_threads) * (df_t->id-1);
    *fim = ( (*df_t->size)/df_t->n_threads) * (df_t->id);

    //for(int i=*inicio ; i<*fim ; i++){
    //    df_t->soma = df_t->soma + df_t->p_vetor[i]);
    //}
    free(inicio);
    free(fim);

}


int main(int argc,char *argv[]){
    if (argc!=3){
        printf("modo de usar programa + tamanho do vetor + numero de threads");
    }
    else{
        int * size = malloc(sizeof(int) * atoi(argv[1]) );
        * size = atoi(argv[1]);
        //printf("%d",*size);
        
        int *vetor = malloc(sizeof(int) * atoi(argv[1]) );
        int *i = malloc(sizeof(int) * atoi(argv[1]) );//contador
        long int soma = 0;

        for( *i = 0 ; *i < (*size); (*i)++){
            vetor[ *i ] = *i;         
        }

        
        clock_t m_1 = clock();
        for( *i = 0 ; *i< (*size); (*i)++){
            soma = soma + vetor[*i];         
        }
        clock_t m_2 = clock();
        double Tempo = (m_2 - m_1) * 1000.0 / CLOCKS_PER_SEC;
        printf("Soma iterativa %ld\n", soma);
        printf("Tempo gasto: %g ms\n", Tempo);
        
        // Com Threads
        int n_threads = atoi(argv[2]);
        pthread_t lst_thread[n_threads];
        dataframe_t lst_df[n_threads];
        
        
        clock_t m_3 = clock();  
        for(int t=0 ; t < n_threads ; t++){
            lst_df[t].id = t+1;
            lst_df[t].soma = 0;
            lst_df[t].p_vetor = vetor;
            lst_df[t].size = size;
            lst_df[t].n_threads = n_threads;
            //pthread_create(&lst_thread[t],NULL, funcao_thread, &lst_df[t]);
        }
        /*
        long int soma_multi = 0;
        for(int t=0 ; t < n_threads ; t++){
            pthread_join(lst_thread[t], NULL); 
            soma_multi = soma_multi + lst_df[t].soma;
        }
        */
        
        //printf("soma multhread some é:%ld\n",soma_multi);
        clock_t m_4 = clock();
        double Tempo_2 = (m_4 - m_3) * 1000.0 / CLOCKS_PER_SEC;
        printf("Tempo gasto: %g ms\n", Tempo_2);
        
        free(i);
        free(vetor);
        free(size);
        
    }
    
    system("read -p 'Press Enter to continue...' var");
    return 0;
}
