#include <stdlib.h>
#include <stdio.h>

typedef struct produto
{
    char nome[100];
    int num_prod;
} produto;

typedef struct pilha
{
    int ultimo;
    int capacidade;
    produto *produto;
    int num_itens;
} pilha;

void criar_pilha(pilha *f, int capacidade)
{
    f->ultimo = -1;
    f->capacidade = capacidade;
    f->produto = malloc(capacidade * sizeof(produto));
    f->num_itens = 0;
}

void inserir_elementos(pilha *f, int elemento, char nome[100])
{
    f->ultimo++;
    f->produto[f->ultimo].num_prod = elemento;
    for (int a = 0; a < 100; a++)
    {
        f->produto[f->ultimo].nome[a] = nome[a];
    }
    f->num_itens++;
}

void remover_elemento(pilha *f)
{
    produto remover = f->produto[f->ultimo];
    printf("\nO produto removido foi\nNome: %s", remover.nome);
    printf("\nNumero: %d\n\n", remover.num_prod);
    f->ultimo--;
    f->num_itens--;
}

int esta_cheio(pilha *f)
{
    return f->num_itens == f->capacidade;
}

int esta_vazio(pilha *f)
{
    return f->num_itens == 0;
}

int main()
{
    pilha pilha;
    int cap, opcao, num;
    char nome[100];

    printf("\n\nDefina a capacidade da sua pilha: ");
    scanf("%d", &cap);

    criar_pilha(&pilha, cap);

    while (opcao != 0)
    {
        printf("\n========================================================\n\n1 - Inserir Elemento.\n2 - Remover Elemento.\n0 - Sair\n\nESCOLHA SUA OPCAO\n\nOpcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            if (esta_cheio(&pilha))
            {
                printf("\n\n========= ESTA CHEIO =========\n\n");
            }
            else
            {
                printf("\nInsira o nome do seu produto: ");
                scanf("%s", nome);
                printf("Insira o Numero do seu produto: ");
                scanf("%d", &num);
                inserir_elementos(&pilha, num, nome);
            }
            break;
        case 2:
            if (esta_vazio(&pilha))
            {
                printf("\n\n            ESTA VAZIO\n\n");
            }
            else
            {
                remover_elemento(&pilha);
            }
            break;
        case 0:
            printf("\n\n            Ate mais!\n\n");
            break;
        default:
            printf("\n\n!!!!!!!!!!!!!!!     OPCAO INVALIDA      !!!!!!!!!!!!!\n\n");
            break;
        }
    }
    free(pilha.produto);
}