#ifndef MyPriorityQueue_H
#define MyPriorityQueue_H

#include "MyVec.h"
#include <iostream>
using namespace std;

template<class T>
class MyPriorityQueue {
public:
	void push(const T&el);   //adiciona elmento na fila
	const T & top() const { return heap[ 0 ]; }   //retorna o topo
	void pop();     //remove o primeiro elmento

	int size() const { return heap.size(); }  //retorna o tamanho da fila

	MyPriorityQueue & operator=(const MyPriorityQueue &); //operador de atribuição

	//para estudar o estado do heap...
	void print()  const ;
private:
	void moveUp(int pos);
	void moveDown(int pos);

	MyVec<T> heap;
};


template<class T>
void MyPriorityQueue<T>::moveDown(int pos) { //refaz o heap da posicao "pos" em diante
	while(2*pos+1 < heap.size()) { //enquanto a posicao atual tiver pelo menos um filho...
		int maiorFilho = 2*pos+1;
		if(2*pos+2 < heap.size() && *heap[2*pos+2] > *heap[maiorFilho]) //ha um segundo filho e ele eh maior do que o primeiro?
			maiorFilho = 2*pos+2;
		if(*heap[pos] > *heap[maiorFilho]) {
			return; //nao precisamos continuar... por que?
		} else {
			swap(heap[pos],heap[maiorFilho]); //troque o atual com o maior filho 
			pos = maiorFilho; //repita o processo agora na posicao "maiorFilho"
		}
	}
}


//dado um novo elemento (na posicao pos, inicialmente no final) do heap, refaz as propriedades do heap "subindo-o" 
template<class T>
void MyPriorityQueue<T>::moveUp(int pos) { 
	while(pos>0) {
		int pai = (pos-1)/2; //pai da posicao i...
		if(*heap[pos] > *heap[pai]) {
			swap(heap[pos],heap[pai]);
			pos = pai;
		}
		else break;
	}	
}


template<class T>
void MyPriorityQueue<T>::push(const T&elem) {
	//adicione o elemento no final do heap e use moveUp para restaurar as propriedades de heap
	heap.push_back(elem);
	moveUp(heap.size()-1);
}

template<class T>
void MyPriorityQueue<T>::pop() {
	swap(heap[heap.size()-1],heap[0]); //pega o ultimo elemento do heap e coloca-o na primeira posicao
	heap.resize(heap.size()-1); //reduz o tamanho do heap
	moveDown(0); //restaura as propriedades de heap a partir da posicao 0
}

template<class T>
void MyPriorityQueue<T>::print() const {
	cout << heap << endl;
}



template<class T>
MyPriorityQueue<T> & MyPriorityQueue<T>::operator=(const MyPriorityQueue<T> &other) {
	//se a arvore ja aponta para arvore que estamos querendo copiar, a retonamos
	if(this==&other) 
		return *this; 
	
	//caso contrário, a destruímos
	heap.clear();

	//copiamos o conteúdo de M para this
	MyVec<T> heap;

	heap = other.heap;
	
	return *this;
}


#endif
