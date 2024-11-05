#include <iostream>
#include <string>
using namespace std;

class pessoa {
public:
    string nome;
    int idade;

    pessoa():nome("Vazio"),idade(0){}
    string getnome(){return nome;}
    int getidade(){return idade;}

    void setnome(string n){nome = n;}
    void setidade(int i){idade = i;}
};

class fila {
private:
    int primeiro;
    int ultimo;
    int num_itens;
    int capacidade;
    pessoa *pessoas;
public:
    fila():primeiro(0), ultimo(-1), num_itens(0), capacidade(0){}
    ~fila() { delete[] pessoas; }

    void setpessoa(int c){pessoas = new pessoa[c];}
    void setprimeiro(int p){primeiro = p;}
    void setultimo(int u){ultimo = u;}
    void setnum_itens(int n){num_itens = n;}
    void setcapacidade(int c){capacidade = c;}

    int esta_cheio(){return num_itens == capacidade;}
    int esta_vazio(){return num_itens == 0;}

    void inserir_elemento(string n, int i){
        if(ultimo == capacidade -1){
            ultimo = 0;
        }else{
            ultimo++;
        }
        pessoas[ultimo].setnome(n);
        pessoas[ultimo].setidade(i);
        num_itens++;
    }
    void remover_elemento(){
        cout << "O nome da pessoa removida e: " << pessoas[primeiro].getnome() << endl;
        cout << "A idade da pessoa removida e: " << pessoas[primeiro].getidade() << endl;

        if(primeiro == capacidade -1){
            primeiro = 0;
        }else{
            primeiro++;
        }
        num_itens--;
    }


};

int main(){
    fila fila1;
    int c, opcao=0, idade;
    string nome;


    cout << "Defina a capacidade da sua fila: ";
    cin >> c;
    cin.ignore();

    fila1.setcapacidade(c);
    fila1.setpessoa(c);

    do{
        cout << "1. inserir elemento\n2. remover elemento\n0. sair." << endl;
        cout << "Selecione uma opcao: ";
        cin >> opcao;
        cin.ignore();
        if(opcao == 1 && fila1.esta_cheio()){
            cout << "A fila esta cheia. Remova um elemento para inserir outro." << endl;
        }else if(opcao == 1){
            cout << "Digite o nome da pessoa: ";
            getline(cin, nome);
            cout << "Digite a idade da pessoa: ";
            cin >> idade;
            cin.ignore();
            fila1.inserir_elemento(nome, idade);
        }

        if(opcao == 2 && fila1.esta_vazio()){
            cout << "A fila esta vazia. Por favor insira um elemento." << endl;
        }else if(opcao == 2){
            fila1.remover_elemento();
        }

        if(opcao == 0){
            cout << "Ate mais!" << endl;
        }else if(opcao != 1 && opcao != 2 && opcao != 0){
            cout << "Opcao invalida. tente novamente." << endl;
        }

    }while(opcao != 0);

}