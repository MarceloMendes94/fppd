#include<stdio.h>
#include<stdlib.h>
int main(){
    int var,teste;

    var = 32767;
    printf("%d\n",var);

    teste = 65539;
    printf("%d\n",(int)teste);

    printf("%ld\n",sizeof(teste));
    return 0;
}
