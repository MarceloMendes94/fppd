# fppd

## Como compilar
```
gcc -pthread -o 'nome_executavel' programa.c
```

## Uso da biblioteca de Pthread
1. Você deve importar a biblioteca para poder usar os tipos e funções disponibilizados.
```
#include <pthread.h>
```

2. tipo default de thread é o pthread você deve declarar a thread antes de usa-lá.
```
pthread_t tid;
```
3. Para criação de uma thread é usado a função pthread_create, que recebe: endereço de memória de um pthread, NULL, função de thread, parametro para função de thread.
```
pthread_create(&tid,NULL, funcao, &var1)
```

``` vale ressaltar que a função de thread deve ter uma assinatura recebendo um ponteiro pra void e retornando ponteiro para void, a passagem de paramentos é feita  com cast e decasting.```
```
void* funcao_thread (void* args); 
```
4. Cabe ao sistema operacional gerenciar os recursos das threads, por esse motivo devemos esperar o fim de uma thread para recolher o resultado do seu processamento, para esse fim temos a função pthread_join.
```
pthread_join(tid, NULL);
```
<hr>

## Atividade 1
Faça um programa que calcule a soma de um vertor de 100.000 das seguintes maneiras: de forma iterativa, com 2 Threads e com 4 Threads e meça o tempo de execução em cada situação.
Ao final faça seu programa de forma genérica para qualquer quantidade de threads.


[Como medir tempo em C](medir_tempo.md)
