#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "MyPriorityQueue.h"
#include "MyVec.h"
#include "Node.h"

//struct responsável para armazenar a tebela de simbolos
struct simbolo{
    unsigned char letra;  //caracter
    char bits[20];        //sua codificação
};

class HuffManTree{
    public:
        HuffManTree(const int freqs[]);  //construtor
        HuffManTree(const HuffManTree &); //connsstrutor por cópia
	    HuffManTree & operator=(const HuffManTree &); // operador de atribuição
	    ~HuffManTree(); //destrutor
        
        //funções que comprimi e descomprimi
        void comprimir(MyVec<bool> &out, const MyVec<unsigned char> &in);
        void descomprimir(MyVec<unsigned char> &out, const MyVec<bool> &in);
        
        //função para gerar a tabela de simbolos
        void tabelaDeSimbolos(simbolo *s);
        int getTam() const;   //retorna o a qtd de elementos  da árvore

        void print(); //imprimi a árvore
        
    private:
    
        MyPriorityQueue< Node<int> * > PQ;  //fila de prioridades de apontadores para nodo
        Node<int> *arvore;      //arvore de HUffman
        int freqs[256];       //vetor frequencias
        int tam;             //qtd de caracters

        //funções auxiliares usadas nas funções públicas
        
        void imprimeDFS_pre(const Node<int> *p) const ;        
        void criandoArvore();                                   
        void construindoTabela(const Node<int> *p,simbolo *s,int &i,string bits);
        void destroy(Node<int> *root);
	    Node<int> *copyElements(const Node<int> *root)const;
};

#endif