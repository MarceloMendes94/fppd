#define AUTOR "Marcelo Passamai Mendes"
#define DATA "17/10/2022"
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <semaphore.h>


typedef struct info_lab_t{
    int id;                 // identificador de cada  lab
    int* lst_contagem;      // lst com contagem de vezes que produziu algum produto
    int num_rodadas;        // numero fornceido pelo ususário com total de rodadas
    int prod_1;             // armazena tipo do produto do lab
    int prod_2;             // 1 Injecao 2 Virus 3 Secreto
    ///
    int* mesa;              // mesa com todos os produtos
    int p_prod_1;           // posicao na mesa do produto 1
    int p_prod_2;           // posicao na mesa do produto 2
    pthread_mutex_t* mutex;
    sem_t* semaf_mesa;      // controlar o acesso a mesa    
    pthread_t tid;
}info_lab_t;

typedef struct info_infec_t{
    int id;                 // identificador de cada  INFECTADO
    int* lst_contagem_inf;  //  contar quantas vezes cada infectado cosumiu
    int num_rodadas;        // numero fornceido pelo ususário com total de rodadas
    ///
    int* mesa;               // mesa com todos os produtos
    int p1_prod_1;           // primeira posicao na mesa do produto 1 a ser consumido
    int p2_prod_1;           // segunda  posicao na mesa do produto 1 a ser consumido
    int p1_prod_2;           // primeira posicao na mesa do produto 2 a ser consumido
    int p2_prod_2;           // segunda  posicao na mesa do produto 1 a ser consumido
    pthread_mutex_t* mutex;
    sem_t* semaf_mesa;       // controlar o acesso a mesa    
    pthread_t tid;
}info_infec_t;


void* thread_produtora(void* arg){
    info_lab_t* pacote = (info_lab_t*)arg;
    while(pacote->lst_contagem[0]< pacote->num_rodadas || pacote->lst_contagem[1] < pacote->num_rodadas || pacote->lst_contagem[2] < pacote->num_rodadas){
        pthread_mutex_lock(pacote->mutex);
        if(pacote->mesa[pacote->p_prod_1] == 0 && pacote->mesa[pacote->p_prod_2] == 0){     //verificando se precisa colocar/repor o produto  REMOVA-ME
            sem_post(&pacote->semaf_mesa[pacote->p_prod_1]);
            sem_post(&pacote->semaf_mesa[pacote->p_prod_2]);
            pacote->mesa[pacote->p_prod_1] = pacote->prod_1;
            pacote->mesa[pacote->p_prod_2] = pacote->prod_2;
            pacote->lst_contagem[pacote->id] = pacote->lst_contagem[pacote->id] + 1;     //contando quantas vezes o laboratório renovou estoque REMOVA-ME
        }
        pthread_mutex_unlock(pacote->mutex);
    }
    return NULL;
}  

int tem_produto(int p1,int p2,int p3, int p4, int* mesa){
    return ((mesa[p1] != 0 || mesa[p2] != 0) && (mesa[p3] != 0 || mesa[p4] != 0 ));
}

void* thread_consumidora(void* arg){      // função para os infectados pegarem os produtos na mesa
    info_infec_t* pacote = (info_infec_t*)arg;

    //verifica quantidade mínima
    while (pacote->lst_contagem_inf[0]< pacote->num_rodadas || pacote->lst_contagem_inf[1] < pacote->num_rodadas || pacote->lst_contagem_inf[2] < pacote->num_rodadas)
    {
        pthread_mutex_lock(pacote->mutex); 
        if (tem_produto( pacote->p1_prod_1, pacote->p2_prod_1, pacote->p1_prod_2 , pacote->p2_prod_2, pacote->mesa) )
        {
            if (pacote->mesa[pacote->p1_prod_1]!=0 && pacote->mesa[pacote->p1_prod_2]!=0){
                sem_wait(&pacote->semaf_mesa[pacote->p1_prod_1]);
                sem_wait(&pacote->semaf_mesa[pacote->p1_prod_2]);
                pacote->mesa[pacote->p1_prod_1] = 0;
                pacote->mesa[pacote->p1_prod_2] = 0;

            }else if (pacote->mesa[pacote->p1_prod_1]!=0 && pacote->mesa[pacote->p2_prod_2]!=0){
                sem_wait(&pacote->semaf_mesa[pacote->p1_prod_1]);
                sem_wait(&pacote->semaf_mesa[pacote->p2_prod_2]);
                pacote->mesa[pacote->p1_prod_1] = 0;
                pacote->mesa[pacote->p2_prod_2] = 0;

            }else if (pacote->mesa[pacote->p2_prod_1]!=0 && pacote->mesa[pacote->p1_prod_2]!=0){
                sem_wait(&pacote->semaf_mesa[pacote->p2_prod_1]);
                sem_wait(&pacote->semaf_mesa[pacote->p1_prod_2]);
                pacote->mesa[pacote->p2_prod_1] = 0;
                pacote->mesa[pacote->p1_prod_2] = 0;

            }else if (pacote->mesa[pacote->p2_prod_1]!=0 && pacote->mesa[pacote->p2_prod_2]!=0){
                sem_wait(&pacote->semaf_mesa[pacote->p2_prod_1]);
                sem_wait(&pacote->semaf_mesa[pacote->p2_prod_2]);
                pacote->mesa[pacote->p2_prod_1] = 0;
                pacote->mesa[pacote->p2_prod_2] = 0;
            }
            pacote->lst_contagem_inf[pacote->id] = pacote->lst_contagem_inf[pacote->id]+1; 
        }
        pthread_mutex_unlock(pacote->mutex);
    }    
    return NULL;
}



int main( int argc, char *argv[ ] ){
    if(argc!=2){
        printf("Parametros invalidos por favor tente novamente!");
    }else{
        int i = 0;
        int num_lab,num_infec = 3;
        int tam_mesa = 6;

        int* lst_contagem = (int*)malloc(num_lab*sizeof(int));                              //  3 lab
        info_lab_t* labs = (info_lab_t*)malloc(num_lab*(sizeof(info_lab_t)));               // 3 lab

        int* lst_contagem_infec = (int*)malloc(num_lab*sizeof(int));                        //3 infec
        info_infec_t* infect = (info_infec_t*)malloc(num_infec*(sizeof(info_infec_t*)));    //3 infec

        int* mesa = (int*)malloc(tam_mesa*sizeof(int));                                     // 6 espaços
        sem_t* semaf_mesa = (sem_t*)malloc(6*sizeof(sem_t));                                // 6 semaforos 1 por posicao

        pthread_mutex_t mutex;
        pthread_mutex_init(&mutex,NULL);
        int num_rodadas = atoi(argv[1]);

        // criar a mesa e semaforo de posicao 
        printf("0\n");
        for(i = 0; i< tam_mesa; i++){
            mesa[i] = 0;
            sem_init(&semaf_mesa[i],0,0);
        }
        
        printf("1\n");
        for (i=0;i<3;i++){
            lst_contagem[i] = 0;
            lst_contagem_infec[i]=0;
        }


        // criar laboratórios [Injecao, Virus, Virus, Secreto, Injecao, Secreto]
        for(i = 0; i<num_lab; i++){
            labs[i].id = 0;
            labs[i].lst_contagem = lst_contagem;
            labs[i].num_rodadas = num_rodadas;
            labs[i].mesa = mesa;
            labs[i].mutex = &mutex;
            labs[i].semaf_mesa = semaf_mesa;
            // [1 Injecao 2 Virus 3 Secreto]
            // [Injecao, Virus, Virus, Secreto, Injecao, Secreto]
            if(i==0){
                //labs[i].prod_1 = 1;
                //labs[i].prod_2 = 2;
                labs[i].p_prod_1 = 0;//injecao
                labs[i].p_prod_2 = 1; //virus
            }else if(i==1){
                //labs[i].prod_1 = 2;
                //labs[i].prod_2 = 3;
                labs[i].p_prod_1 = 2;//virus
                labs[i].p_prod_2 = 3;//secreto
            }else{
                //labs[i].prod_1 = 1;
                //labs[i].prod_2 = 3;
                labs[i].p_prod_1 = 4;//injecao
                labs[i].p_prod_2 = 5;//secreto
            }   
        }
        
        printf("2\n");
        // criar infectados
        for(i = 0; i<num_infec; i++){

            infect[i].id = 0;                
            infect[i].lst_contagem_inf = lst_contagem_infec;  
            infect[i].num_rodadas = num_rodadas;        
            infect[i].mesa = mesa; 
            infect[i].mutex = &mutex;
            infect[i].semaf_mesa = semaf_mesa;      
            if(i==0){
                // infectado 0 precisa de SECRECTO & VIRUS
                infect[i].p1_prod_1 = 5;          
                infect[i].p2_prod_1 = 3;          
                infect[i].p1_prod_2 = 1;          
                infect[i].p2_prod_2 = 2;   
            }else if(i==1){
                // infectado 1 precisa de INJECAO & VIRUS
                infect[i].p1_prod_1 = 0;          
                infect[i].p2_prod_1 = 4;          
                infect[i].p1_prod_2 = 1;          
                infect[i].p2_prod_2 = 2;   
            }else{ //infectado 2 precisa de INJECAO &SECRETO
                infect[i].p1_prod_1 = 0;          
                infect[i].p2_prod_1 = 4;          
                infect[i].p1_prod_2 = 5;          
                infect[i].p2_prod_2 = 3;     
            }      
        }


        printf("3\n");
        for(i=0; i<3; i++){                         
            //criando as threads de laboratório
            pthread_create(&labs[i].tid,NULL, thread_produtora,&(labs[i]));
        }
        
        printf("4\n");
        for(i=0; i<3; i++){                         
            //criando as threads de infectados
            pthread_create(&infect[i].tid,NULL,thread_consumidora,&(infect[i]));
        }

        printf("5\n");
        ///DANDO JOIN NAS THREADS
        for(i=0; i<3;i++){
            pthread_join(labs[i].tid,NULL);  
        }
        printf("6\n");
        for(i=0; i<3;i++){
        pthread_join(infect[i].tid,NULL); 
        }

        printf("7\n");
        //Número de vezes que cada laboratório renovou seu estoque
        printf("\nNumero de vezes que cada laboratorio renovou seu estoque... \n\n");
        for(i=0;i<3;i++){
            printf("Laboratorio %d produziu %d\n",i,labs[i].lst_contagem[i]);
        }
        printf("8\n");
        //Número de vezes que cada infectado injetou a vacina
        printf("\n\nNumero de vezes que cada infectado se injetou com a vacina... \n\n");
        for(i=0;i<3;i++){
            printf("Infectado %d se injetou %d\n",i,infect[i].lst_contagem_inf[i]);
        }
    }
    return 0;
}