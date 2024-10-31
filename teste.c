#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int soma_dos_elementos(int vet[], int tam){
    int soma = 0;
    if(tam > 0){
        tam--;
        soma = soma_dos_elementos(vet,tam);
        soma+=vet[tam];
    }
    return soma;
}

int main(){
    int tam,soma,caso = 1;
    srand(time(NULL));

    
    while(caso!=2){
        printf("1 - iniciar novo calculo.\n");
        printf("2 - sair.\n");
        printf("escolha sua opcao: ");
        scanf("%d", &caso);
        
        switch (caso) {
            case 1: {
                printf("insira o tamanho do seu vetor: ");
                scanf("%d", &tam);
                int vet[tam];
                for(int cont = 0; cont< tam; cont++){
                     vet[cont] = (int)rand() % 10;
                }

                soma = soma_dos_elementos(vet, tam);

                /*for(int cont =0; cont< tam; cont++){
                    printf("vet[%d] = %d\n", cont, vet[cont]);
                }*/

                printf("A soma de todos os elementos do vetor e: %d\n", soma);
                break;
            }
            case 2:
                printf("Ate mais!");
                break;
            default:
                printf("Opcao invalida tente novamente.\n");
                break;
        }
    }
}