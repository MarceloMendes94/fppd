# Como medir tempo em C

Para medir tempo em C é necessário fazer uso da biblioteca ```time``` para fazer a a importação.
```
#include<time.h> 
```
A ideia de tempo é a diferença entre momentos ou seja: momento final - momento inicial = Tempo
para capturar um momento devemos declarar um tipo ```clock_t``` e solictar o clock  atual.
```
clock_t momento; //declaração.
momento = clock(); //captura do clock.
```
Apresentação do tempo em milisegundos

(momento final - momento inicial)*1000/CLOCKS_PER_SEC;




Exemplo completo
```
#include <stdio.h>
#include <time.h>


int main(void)
{
    clock_t Ticks[2];
    Ticks[0] = clock();
    //O código a ter seu tempo de execução medido ficaria neste ponto.
    Ticks[1] = clock();
    double Tempo = (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;
    printf("Tempo gasto: %g ms.", Tempo);
    return 0;
}
```
