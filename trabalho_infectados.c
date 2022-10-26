#define AUTOR "Marcelo Passamai Mendes"
#define DATA "20/10/2022"
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

/* Temos um array com 6 posições VM VM IS IS IN IN para
armazenar produtos a serem consumidos.
Temos 3 fabricas cada uma produz 2 produtos diferentes
elas são diferentes entre si. também temos 3 consumidores
cada um precisa consumir os 3 produtos disponiveis porém
cada um conta com uma estoque infinito de 1 produto, sendo
que esse estoque é diferente entre cada consumidor.*/

//[ VM IJ VM IS IJ IS ]

#define TAM 6
#define N_RODADAS 99

typedef struct data_t{
    int mesa[TAM];
    pthread_mutex_t mutex1;
}data_t;

int nao_tem_produto(void* arg,int p1, int p2){
    data_t* pkg = (data_t*) arg;
    return(pkg->mesa[p1]==0 || pkg->mesa[p2]==0 );    
}

void produzir_material(void* arg,int pos){
    /* se não tem virus morto  produz na posição
    indicada */
    data_t* pkg = (data_t*) arg;
    if(pkg->mesa[pos]==0){
        pthread_mutex_lock(&(pkg->mutex1));
        pkg->mesa[pos]=1;
        printf("%d foi produzido\n",pos);
        pthread_mutex_unlock(&(pkg->mutex1));
    }
}

void* L1_vm_ij(void* arg){
    /* Verificar se a posição [0] ou [1] estão vazias e preencher */
    data_t* pkg = (data_t*) arg;
    do {
        produzir_material((void*)pkg, 0);
        produzir_material((void*)pkg, 1);        
    }while(nao_tem_produto((void*)pkg,0,1));

    return NULL;
}

void* L2_vm_is(void* arg){
    /* Verificar se a posição [2] ou [3] estão vazias e preencher */
    data_t* pkg = (data_t*) arg;
    do {
        produzir_material((void*)pkg, 2);
        produzir_material((void*)pkg, 3);        
    }while(nao_tem_produto((void*)pkg,2,3));

    return NULL;
}

void* L3_ij_is(void* arg){
    /* Verificar se a posição [4] ou [5] estão vazias e preencher */
    data_t* pkg = (data_t*) arg;
    do {
        produzir_material((void*)pkg, 4);
        produzir_material((void*)pkg, 5);        
    }while(nao_tem_produto((void*)pkg,4,5));

    return NULL;
}



// consumidores

void pegar_material(void* arg, int pos1, int pos2){
    data_t* pkg = (data_t*) arg;
    //Se tiver o produto consumir;
    int tem_produto = nao_tem_produto((void*) pkg, pos1, pos2)!=0;
    if (tem_produto){
        
    }



    return NULL;
}

void * C1_vm_inf(void * arg){
    // pesquisar recurso na posição de IJ e IS   
    data_t* pkg = (data_t*) arg;
    // pegar injeção
    pegar_material((void*) pkg, 1, 4);
    // pegar insumo X
    pegar_material((void*) pkg, 3, 5);
    printf("vacina feita");
    return NULL;
}

void * C2_ij_inf(void * arg);

void * C3_is_inf(void * arg);

int main(){
    int i=0;
    data_t* pkg;
    pkg = malloc(sizeof(data_t));
    for (i=0;i<TAM;i++){
        pkg->mesa[i]=0;
    }
    
    pthread_t  thread_mv;
    pthread_create(&thread_mv, NULL, L1_vm_ij, (void*) pkg);
    pthread_join(thread_mv, NULL);
    
    pthread_t  thread_ij;
    pthread_create(&thread_ij, NULL, L2_vm_is, (void*) pkg);
    pthread_join(thread_ij, NULL);    

    pthread_t  thread_is;
    pthread_create(&thread_is, NULL, L3_ij_is, (void*) pkg);
    pthread_join(thread_is, NULL);


    

    
    return 0;
}

