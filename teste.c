#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//função que soma todos os elementos do vetor
int soma_dos_elementos(int vet[], int tam){
    int soma = 0;
    if(tam > 0){
        tam--;
        //chamada recursiva
        soma = soma_dos_elementos(vet,tam);
        soma+=vet[tam];
    }
    return soma;
}

int main(){
    int tam,soma,caso = 1;
    srand(time(NULL));

    //inicia um processo de repetição para caso queira testar mais vezes
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

                //for para inserir numeros aleatórios utilizando rand()
                for(int cont = 0; cont< tam; cont++){
                    //aqui estou utilizando somente numeros de 0 a 10, caso queira utilizar numeros maiores só alterar.
                    vet[cont] = (int)rand() % 10;
                }

                soma = soma_dos_elementos(vet, tam);
                //for para mostrar os numeros contidos no vertor
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
    return 0
}
