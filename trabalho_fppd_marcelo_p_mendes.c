#define AUTOR "Marcelo Passamai Mendes"
#define DATA "17/10/2022" //ultima modificação
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <semaphore.h>

typedef struct info_lab_t
{
    int id;
    pthread_t tid;
    pthread_mutex_t mutex;
    sem_t* lst_sem;
    int  tam_controlador;
    int* controlador_labs;
    int num_rodadas;
    int* mesa;
    int pos_prod_1;
    int pos_prod_2;
}info_lab_t;

typedef struct info_infec_t
{
    int id;
    pthread_t tid;
    pthread_mutex_t mutex;
    sem_t* lst_sem;
    int tam_controlador;
    int* controlador_vacina;
    int  num_rodadas;
    int* mesa;
    int  pos1_insumo_1;
    int  pos1_insumo_2;
    int  pos2_insumo_1;
    int  pos2_insumo_2;
}info_infec_t;

void* thread_produtora(void* arg);
void* thread_consumidora(void* arg);

int main(int argc, char *argv[ ] ){

    if(argc!=2){
        printf("Parametros invalidos por favor tente novamente!");
    }else{
        int i = 0;
        int num_rodadas = atoi(argv[1]);;
        int tam_mesa = 6;
        int labs_infec = 3; 
        
        // estrutura compartilhada
        int* mesa = (int*)malloc(tam_mesa*sizeof(int)) ;
        sem_t* lst_sem = (sem_t*)malloc(tam_mesa*sizeof(sem_t));
        pthread_mutex_t mutex;
        pthread_mutex_init(&mutex,NULL);

        int* controlador_labs = (int*)malloc(labs_infec*sizeof(int));
        int* controlador_vacina = (int*)malloc(labs_infec*sizeof(int));
        for(i=0;i<labs_infec;i++){
            controlador_labs[i] = 0;
            controlador_vacina[i] = 0;
        }

        info_lab_t* labs = (info_lab_t*)malloc(labs_infec*sizeof(info_lab_t));
        info_infec_t* infc = (info_infec_t*)malloc(labs_infec*sizeof(info_infec_t));

        // popular estrutura de laboratórios
        for(i=0;i<labs_infec;i++){
            labs[i].id = i;
            labs[i].tam_controlador = labs_infec;
            labs[i].mutex = mutex;// sem &
            labs[i].lst_sem = lst_sem;
            labs[i].controlador_labs = controlador_labs;
            labs[i].num_rodadas = num_rodadas;
            labs[i].mesa = mesa;
            if(i==0){
                labs[i].pos_prod_1 = 0;// virus morto
                labs[i].pos_prod_2 = 1;// injecao
            }else if(i==1){
                labs[i].pos_prod_1 = 2;// insumo secreto
                labs[i].pos_prod_2 = 3;// virus morto 
            }else{
                labs[i].pos_prod_1 = 4;// injecao
                labs[i].pos_prod_2 = 5;// insumo secreto
            }   
        }

        // popular estrutura de infectados
        for(i=0;i<labs_infec;i++){
            infc[i].id =i ;
            infc[i].mutex = mutex;//sem &
            infc[i].lst_sem = lst_sem;
            infc[i].controlador_vacina = controlador_vacina;
            infc[i].num_rodadas = num_rodadas;
            infc[i].mesa = mesa;
            if(i==0){ // Virus & insumo secreto
                infc[i].pos1_insumo_1 = 0;
                infc[i].pos1_insumo_2 = 2;
                infc[i].pos2_insumo_1 = 3;
                infc[i].pos2_insumo_2 = 5;
            }else if(i==1){ // injecao & virus
                infc[i].pos1_insumo_1 = 1;
                infc[i].pos1_insumo_2 = 0;
                infc[i].pos2_insumo_1 = 4;
                infc[i].pos2_insumo_2 = 3;
            }else{ //injecao & insumo
                infc[i].pos1_insumo_1 = 1;
                infc[i].pos1_insumo_2 = 2;
                infc[i].pos2_insumo_1 = 4;
                infc[i].pos2_insumo_2 = 5;
            }        
        }
        
        //criar threads labs
        for(i=0;i<labs_infec;i++){                        
            pthread_create(&labs[i].tid,NULL, thread_produtora,&(labs[i]));
        }
        
        //criar threads vacinas
        for(i=0;i<labs_infec;i++){                    
            pthread_create(&infc[i].tid,NULL,thread_consumidora,&(infc[i]));
        }

        // juntar threads labs
        for(i=0; i<labs_infec;i++){
            pthread_join(labs[i].tid,NULL);  
        }
        // juntar threads vacinas
        for(i=0; i<labs_infec;i++){
            pthread_join(infc[i].tid,NULL);  
        }

        // Resultados
        for(i=0;i<labs_infec;i++){                        
            printf("lab %d produziu %d vezes\n",labs[i].id+1,labs[i].controlador_labs[i]);
        }
        for(i=0;i<labs_infec;i++){                        
            printf("infc %d consumiu %d vezes\n",infc[i].id+1,infc[i].controlador_vacina[i]);
        }
    }
    return 0;
}


void* thread_produtora(void* arg){
    info_lab_t* pkg = (info_lab_t*)arg;
    
    while(pkg->controlador_labs[0]< pkg->num_rodadas || pkg->controlador_labs[1] < pkg->num_rodadas || pkg->controlador_labs[2] < pkg->num_rodadas){   
        pthread_mutex_lock(&pkg->mutex); //WARM diferença
        if(pkg->mesa[pkg->pos_prod_1] == 0 && pkg->mesa[pkg->pos_prod_2] == 0){
            sem_post(&pkg->lst_sem[pkg->pos_prod_1]);
            sem_post(&pkg->lst_sem[pkg->pos_prod_2]);
            pkg->mesa[pkg->pos_prod_1] = 1;
            pkg->mesa[pkg->pos_prod_2] = 1;            
        } 
        pkg->controlador_labs[pkg->id] = pkg->controlador_labs[pkg->id]+1;       
        pthread_mutex_unlock(&pkg->mutex);
    }
    return NULL;
} 

void* thread_consumidora(void* arg){
    info_infec_t* pkg = (info_infec_t*)arg;       
    
    while (pkg->controlador_vacina[0]< pkg->num_rodadas || pkg->controlador_vacina[1] < pkg->num_rodadas || pkg->controlador_vacina[2] < pkg->num_rodadas)
    {
        pthread_mutex_lock(&pkg->mutex); 
        if((pkg->mesa[pkg->pos1_insumo_1] != 0 || pkg->mesa[pkg->pos2_insumo_1] != 0) && (pkg->mesa[pkg->pos1_insumo_2] != 0 || pkg->mesa[pkg->pos2_insumo_2] != 0 ) )
        {
            if (pkg->mesa[pkg->pos1_insumo_1]!=0 && pkg->mesa[pkg->pos1_insumo_2]!=0)
            {
                sem_wait(&pkg->lst_sem[pkg->pos1_insumo_1]);
                sem_wait(&pkg->lst_sem[pkg->pos1_insumo_2]);
                pkg->mesa[pkg->pos1_insumo_1] = 0;
                pkg->mesa[pkg->pos1_insumo_2] = 0;
            }
            else if (pkg->mesa[pkg->pos2_insumo_1]!=0 && pkg->mesa[pkg->pos1_insumo_2]!=0)
            {
                sem_wait(&pkg->lst_sem[pkg->pos2_insumo_1]);
                sem_wait(&pkg->lst_sem[pkg->pos1_insumo_2]);
                pkg->mesa[pkg->pos2_insumo_1] = 0;
                pkg->mesa[pkg->pos1_insumo_2] = 0;
            }
            else if (pkg->mesa[pkg->pos2_insumo_1]!=0 && pkg->mesa[pkg->pos2_insumo_2]!=0)
            {
                sem_wait(&pkg->lst_sem[pkg->pos2_insumo_1]);
                sem_wait(&pkg->lst_sem[pkg->pos2_insumo_2]);
                pkg->mesa[pkg->pos2_insumo_1] = 0;
                pkg->mesa[pkg->pos2_insumo_2] = 0;
            }
            else if (pkg->mesa[pkg->pos1_insumo_1]!=0 && pkg->mesa[pkg->pos2_insumo_2]!=0)
            {
                sem_wait(&pkg->lst_sem[pkg->pos1_insumo_1]);
                sem_wait(&pkg->lst_sem[pkg->pos2_insumo_2]);
                pkg->mesa[pkg->pos1_insumo_1] = 0;
                pkg->mesa[pkg->pos2_insumo_2] = 0;
            }
           
        }
        pkg->controlador_vacina[pkg->id] = pkg->controlador_vacina[pkg->id]+1;
        pthread_mutex_unlock(&pkg->mutex);
    }  
    return NULL;
}
