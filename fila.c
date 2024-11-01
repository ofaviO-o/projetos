#include <stdio.h>
#include <stdlib.h>

typedef struct{
   int capacidade;
   float *dados;
   int primeiro;
   int ultimo;
   int num_itens;
}fila;

void criar_fila(fila *f, int c)
{
    f->capacidade = c;
    f->dados = malloc(c * sizeof(float));
    f->primeiro = 0;
    f->ultimo = -1;
    f->num_itens = 0;
}

void inserir_elemento(fila *f, float elemento)
{
    if(f->ultimo == f->capacidade-1)
        f->ultimo = -1;
        
    f->ultimo++;
    f->dados[f->ultimo] = elemento;
    f->num_itens++;
}

float remover_elemento(fila *f)
{
    float aux = f->dados[f->primeiro];
    f->primeiro++;
    
    if(f->primeiro == f->capacidade)
        f->primeiro = 0;
    
    f->num_itens--;
    
    return aux;
}

int esta_vazia(fila *f)
{
    return (f->num_itens == 0);
}

int esta_cheia(fila *f)
{
    return (f->num_itens == f->capacidade);
}

// Função para mostrar todos os elementos da fila
void mostrar_fila(fila *f)
{
    if (esta_vazia(f)) {
        printf("\n\nA fila está vazia!\n");
        return;
    }

    printf("\n\nElementos na fila: ");
    
    int i = f->primeiro;
    int contador = 0;

    // Percorre a fila e imprime cada elemento
    while (contador < f->num_itens) {
        printf("%2.f ", f->dados[i]);
        i = (i + 1) % f->capacidade; // Avança circularmente
        contador++;
    }
    printf("\n");
}

int main()
{
    int opcao = -1, capacidade;
    float valor;
    fila sus;
    
    printf("Informe a capacidade: ");
    scanf("%d", &capacidade);
    criar_fila(&sus, capacidade);
    
    while(opcao != 0)
    {
        printf("\n1 - Inserir elemento");
        printf("\n2 - Remover elemento");
        printf("\n3 - Mostrar elementos da fila");
        printf("\n0 - Sair");
        printf("\nOpcao: ");
        scanf("%d", &opcao);
        
        switch(opcao)
        {
            case 1:
                if(esta_cheia(&sus))
                {
                    printf("\n\nA fila esta cheia.");
                }
                else
                {
                    printf("\nInforme o valor a ser inserido: ");
                    scanf("%f", &valor);
                    inserir_elemento(&sus, valor);
                }
                break;
            
            case 2:
                if(esta_vazia(&sus))
                {
                    printf("\n\nA fila esta vazia.");
                }
                else
                {
                   valor = remover_elemento(&sus);
                   printf("\n\nElemento removido: %2.f", valor);
                }
                break;
            
            case 3:
                mostrar_fila(&sus); // Exibe todos os elementos da fila
                break;
            
            case 0:
                printf("\n\nAte mais!");
                break;
            
            default:
                printf("\n\nOpcao Invalida.");
        }
    }
    // Libera a memória alocada para a fila
    free(sus.dados);

    return 0;
}