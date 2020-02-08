#include "Huffman.h"
#include <cstring>
#include <iostream>
#include <fstream>


using namespace std;

//explicação da função recusrsiva

//condição de parada, ter um elemntento na fila de prioridades,, ou seja, a árvore pronta

//cria um nodo que recebe os dois mnores elementos da árvore
//foi feito o min heap na fila de rioridades, logo o elemnto
//do topo sempre é o menor elemento da fila de pioridades

//Node<int> * root = new Node<int>(PQ.top()->elem); // novo nodo recebe o primeiro elemnto

//root->left = PQ.top(); //faz com q o primeiro elemento  seja apontado a esquerda do novo nodo

//PQ.pop();  //remove o elemento

//como o elemento do novo nodo recebe a soma dos dois elemntos que serão retirados da fila,
//somamos o elemneto do novo nodo (que é do primeiro nodo) com o elem do segundo

//faz com q o segundo elemento menor seja apontado a direita do novo nodo

//removemos o segundo elemento menor da fila de prioridades 

//adicionamos a novo nodo na fila de prioridades

//chama a função novamente

void HuffManTree::criandoArvore(){
    
    if(PQ.size() <= 1)
        return;

    Node<int> * root = new Node<int>(PQ.top()->elem);
   
    root->left = PQ.top();

    PQ.pop();

    root->elem = root->elem +  PQ.top()->elem;
    
    root->right = PQ.top();

    PQ.pop();

    PQ.push(root);

    criandoArvore();

}

//explicação do construtor

//copia as frequencias passadas na função para as frequancias da classe

//se alguma frequência for diferente de 0, tem um caracter, logo
//cria um nodo com essa frequência e a adiciona na fila de prioridades

//criandoArvore(); //chama a função para criar a arrvore , 
//já que ja temos a fila de prioridades de nodos

//caso especial
//se tiver um arquivo vazio, não teremos nenhum ele,nto na fila e a arvore nao seria criada
//com isso foramos a fila de prioridades ter um elemnto, 0, com isso ao criar a arvore,
//saberemos que o arquivo não tem elmentos

HuffManTree::HuffManTree(const int freqs[]){
    arvore = NULL;
    tam = 0;
    
    for(int i=0;i<256;i++){
        this->freqs[i] = freqs[i];
        if(freqs[i] != 0 ){
            Node<int> *elementos = new Node<int>(freqs[i]);
            elementos->letra = (unsigned char)i;
            PQ.push(elementos);
            tam++;
        }
    }

    criandoArvore();
    
    if(PQ.size() == 0)
        PQ.push(0);
    arvore = PQ.top();
    
}

//função auxiliar para copiar uma arvore
Node<int> * HuffManTree::copyElements(const Node<int> *root)const{
    //caso base, se a raiz for NUUL, ja copiamos tudo, e podemos retornar
	if(root == NULL)
		return NULL;
	
    //cria um novo nodo apontando para a raiz
    Node<int> *ptr = new Node<int>(root->elem);
	
    ptr->left = copyElements(root->left);   //copia tudo do lado esquerdo
	ptr->right = copyElements(root->right);  //copia tudo do lado direito

	return ptr;   //retorna o nodo

}

//operador de atribuição
HuffManTree & HuffManTree::operator=(const HuffManTree &M){
	//se a arvore ja aponta para arvore que estamos querendo copiar, a retonamos
    if(this == &M)
		return *this;
	
    //caso contrário, a destruímos
	destroy(arvore);
	
    //copiamos o conteúdo de M para this
	
    tam = M.tam;

    for(int i=0;i<tam;i++)
        freqs[i] = M.freqs[i];
    
    PQ = M.PQ;

	arvore = copyElements(M.arvore);
	
	return *this;
}

//construtor padrão
HuffManTree::HuffManTree(const HuffManTree &M){
    //caso base, se a raiz for NUUL, ja copiamos tudo, e podemos retornar
	arvore = NULL;
	tam = 0;
    
    //inicializamos as frequencias com valor 0
    for(int i=0;i<tam;i++)
        freqs[i] = 0;

    //copiamos conteúdo de M para this
	*this = M;
}

//função auxiliar que destroi a arvore
void HuffManTree::destroy(Node<int> *root){
    //se root é NULL, a árvore ja foi destruída
	if(root == NULL)
		return;
	

	destroy(root->left); //destroi a parte esquerda
	destroy(root->right); //destroi a parte direita
	
	delete root;  //destroi o nodo
}

//destrutor
HuffManTree::~HuffManTree(){
	destroy(arvore);
}


void HuffManTree::comprimir(MyVec<bool> &out, const MyVec<unsigned char> &in){
    //cria a tabela de simbolos
    
    simbolo s[tam];
    tabelaDeSimbolos(s);

    //variáveis auxiliares

    int c = 0;
    bool b;

    //percorre todo vetor de char(i), proucura a posição na struct(j) que está o caracter i,
    //quando encontra, grava no vetor de bool a codificação, true se for 1 e false caso contrário
    for(int i=0;i<in.size();i++){
        for(int j=0;j<tam;j++){
            if(in[i] == s[j].letra){
                for(int k=0;k<strlen(s[j].bits);k++){
                    if(s[j].bits[k] == '1')
                        b = true;
                    else
                        b = false;
                    
                    out.push_back(b);
                    c++;
                }
                break;
            }
        }
    }
}

void HuffManTree::descomprimir(MyVec<unsigned char> &out, const MyVec<bool> &in){
    //cria a tabela de simbolos

    simbolo s[tam];
    tabelaDeSimbolos(s);
    
    char pos;
    
    //cria um nodo igual a arvore
    Node<int> * it = arvore;


    //percorre todo vetor in(i), se achar uma letra adiciona no vetor final que armazenará 
    //o conteúdo do arquivo original.


    for(int i=0;i<in.size();i++){
        if(it->letra.length() != 0){
            pos = it->letra[0];
            out.push_back(pos);
            it = arvore;  //como achamos uma letra, temos que voltar na raiz da árvore

        }

        //se in for true, it vai anda pra esquerda
        if(in[i] == true && it->left != NULL )
            it = it->left;
        
        //se in for false, it vai anda pra direita
        else if(in[i] == false && it->right != NULL)
            it = it->right;
        
    }
}

//função para imprimir a árvore
void HuffManTree::imprimeDFS_pre(const Node<int> *p) const {
	if(!p) return;
	cout << p->elem << " " << p->letra << endl;
	imprimeDFS_pre(p->left);
	imprimeDFS_pre(p->right);
} 

//imprime a árvore
void HuffManTree::print(){
    imprimeDFS_pre(arvore);
}


void HuffManTree::construindoTabela(const Node<int> *p,simbolo *s,int &i,string bits){
    //condição de parada, ter percorrido toda a árvore
    if(!p)
        return;
	
    //variáveis auxiliares
    unsigned char aux;
    int k = 0;
    
    //se ao percorrer a arvore, achamos um caracter, salvamos esse caracter na posição i da struct
    //e salvamos o bit desse gerado desse número
    if(p->letra.length() != 0){
        s[i].letra = p->letra[0];
        for(int j=0;j<bits.length();j++){
            aux = bits[j];
            s[i].bits[k] = aux;
            k++;
        }
        s[i].bits[k] = '\0';
        k = 0;
        i++;
    }
	
    //percorre a aŕvore para a esquerda, cada hora q anda adiciona 1 na codificação
    construindoTabela(p->left,s,i,bits + '1');
    //percorre a aŕvore para a direita, cada hora q anda adiciona 0 na codificação
	construindoTabela(p->right,s,i,bits + '0');
}

void HuffManTree::tabelaDeSimbolos(simbolo *s){
    //inicialmente bits e letra de s são nulos 
    s->bits[20] = '\0';
    s->letra = '\0';
    int i = 0;
    
    //cria a tabela
    construindoTabela(arvore,s,i,"");

    //caso i for 1, temos apenas uma letra e o código não gera um simbolo para o caracter
    //logo criamos uma codificação para o caracter, nesse caso optei por 1
    if(i==1){
        s[0].bits[0] = '1';
        s[0].bits[1] = '\0';
        s[0].letra = arvore->letra[0];
    }
}

//retorna a qtd de elmentos na fila de prioridades
int HuffManTree::getTam() const{
    return tam;
}