#ifndef NODE_H
#define NODE_H

//classe para representar os nodos da árvore

template  <class T>
class Node {
	public:
        // construtor padrão inicilamente tudo aponta pra NULL, elem é 0 e a letra é nula
        Node() :  elem(0),letra(""),left(NULL), right(NULL) {}
		Node(const T &elem_);
		
        Node<T> *left, *right;
		T elem;
        string letra;  //caracter de cada nodo

        //sobrecargas dos operadores menor e maior, já que comparo nodos na fila de prioridades
        bool operator>(const Node<T> &other) const;
        bool operator<(const Node<T> &other) const;
        T operator*(const T) const;

        template<class T2>
        friend std::ostream& operator<<(std::ostream &out, const Node<T> &n);

        //operador de atribuição
        Node & operator=(const Node &);
};


//contrutor, elem recebe o elemto passado e, a letra inicialmente é vazia
template <class T>
Node<T>::Node(const T &elem_){
    elem = elem_;
    letra = "\0";
    left = right  = NULL;
}

//criei min heap na fia de prioridades, para isso, invertir os operadores de menor e maior
//logo o operador maior virou o operador menor  e vice-versa

template <class T>
bool Node<T>::operator>(const Node<T> &other) const {
    if(elem > other.elem) 
        return false;
    else 
        return true;
}

template <class T>
bool Node<T>::operator<(const Node<T> &other) const {
    if(elem > other.elem) 
        return true;
    else 
        return false;
}

//operador de multilicação
template <class T>
T Node<T>::operator*(const T other) const{
    return elem * other;
}

//operador de leitura
template <class T2>
std::ostream& operator<<(std::ostream &out, const Node<T2> &n){
    out << n.elem;
    return out;
}

template<class T>
Node<T> & Node<T>::operator=(const Node<T> &other) {
	if(this==&other) 
		return *this; 
	
    //copio o o conteúdo de other para this

	elem = other->elem;
    left =  other->left;
    right =  other->right;
    letra = other->letra;
	
	return *this;
}


#endif