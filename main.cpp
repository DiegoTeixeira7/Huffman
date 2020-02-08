#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>
#include "Huffman.h"
#include "MyVec.h"

using namespace std;

//função que converte um número um vetor de 0s e 1s (vetor de numeros binarios) em seu número decimal
int baseDecimal(int *bit){ 
    int num = 0;    //variável que receberá o número decimal

    //percorre o vetor e trás para frente e sempre multiplica o número[bit[i-1]](menos significativo)
    //vezes o valor de (8-i) elevado a 2
    //(8-i) porque como o primeiro número é o menos significativo , (8 - i ) varia de 0 a 7

    for(int i=8;i>0;i--)
        num = num + bit[i-1] * pow(2,8 - i);

    return num;
}

void leitura(MyVec<unsigned char> &in,int *freqs,ifstream &fin){
    
    //variáveis  auxiliares para a leitura  
    string entrada;
    unsigned char pos;
    int tam = 0;

    //enquanto o arquivo não termina o programa não sai do while
    while(!fin.eof()){
        getline(fin,entrada); //pega cada linha do arquivo

        //enquanto a linha atual não termina o for não acaba
        for(int i=0;i<entrada.length();i++){
            pos = entrada[i]; //recebe cada caracter do arquivo
            freqs[pos]++;  //a cada caracter a posição do vetor referente a esse caracter incrementa
            in.push_back(pos);  //adiciona o caracter no vetor 
            tam++;  // qtd de caracter
        }
        in.push_back('\n');//como getline n lê \n , temos que adcionar a cada nova linha
        freqs[10]++;  // 10 é o numero do \n na tabela ascii
    }
    
    //caso esecial
    //se o arquivo só contiver \n, tam vai ser 0, logo vai ter um \n a  mais 
    if(tam == 0 && freqs[10] != 0){ 
        freqs[10]-=1; //decrementa ele
        in.pop_back(); // e o retira do vetor de char
    }
}

void gerarArquivo(char *name,int *freqs,HuffManTree &arvore,MyVec<bool> &comprimido){
    ofstream out(name,ios::binary);   //arquivo binario comprimido

    //salva o vetor frequencias no arquivo para ser capaz de montar a árvore 
    //a partir do arquivo comprimido
    out.write(reinterpret_cast<char *>(freqs),sizeof(int)*256);  

    //struct para armazenar o simbolo referente a a cada letra
    //seu tamanho é a qtd de caracters diferentes
    simbolo s[arvore.getTam()];
    arvore.tabelaDeSimbolos(s);

    //variáveis auxiliares
    //se tenho uma codificação com 20 bols (10101001 01010101 00), tem 2 pares de 8 e 1 par de 2
    //para armazenar no arquivo, a cada 8 bools , vira 1 byte na impemntação, abaixo.
    //como sobra 2, temos que tratar esse caso

    unsigned char mod = '0';
    int resto = comprimido.size()%8;  //verifica se a codificação tem um cnjunto diferente de 8

    if(resto != 0)   //se sim, salvamos esse resto , usaremos mais pra frente
        mod = resto + '0';
    
    //salvando o valo referente ao resto no  arquivo
    out.write(reinterpret_cast<char *>(&mod),sizeof(unsigned char));
    //qtd de chars salvos no arquivo
    int tamanho = (comprimido.size()/8);
    out.write(reinterpret_cast<char *>(&tamanho),sizeof(int));
    
    // vetor que armazenar os bits para serem convertidos pra decimal e posteriomente em char,
    //pois o tipo char gasta 1 byte, logo ficará menor no arquivo

    int bit[8] = {0}; 
    int num;

    //varre o vetor de bool de menos o resto, já que o resto não tem 8 bools
    for(int j=0;j<(comprimido.size() - resto);){
        //a cada 8 bools, salva no vetor q sera convertido em numero
        for(int i=0;i<8;i++){
            if(comprimido[j] == true)
                bit[i] = 1;
            else
                bit[i] = 0;
            j++;
        }
        
        num = baseDecimal(bit); // transforma o vetor de bit em número decimal
        unsigned char s = (int)num; // faz um cast do numero (4 bytes) para char ( 1 byte)
        //salva o char no arquivo
        out.write(reinterpret_cast<char *>(&s),sizeof(unsigned char)); 

        //vota com o vetor de bits para ir pra príoxima iteração de j
        for(int i=0;i<8;i++)
            bit[i] = 0;
    }

    int j = 0;
    
    //se tiver resto, salvamos esse resto num vetor de de char de no máximo tamanho 7
    if(resto != 0){
        char b[resto];
        //salva o resto no vetor
        for(int i=(comprimido.size() - resto);i<comprimido.size();i++){
            if(comprimido[i] == true)
                b[j] = '1';
            else
                b[j] = '0';
            j++;
        }
        //salva o resto no arquivo binário       
        out.write(reinterpret_cast<char *>(b),sizeof(char)*resto); 

    }
    //fecha o arquivo já que não usaremos mais ele
    out.close();
}


int main(int argc, char **argv){
    // primeiro argumento se for c, o programa comprimi, se for d o programa descomprimi
    string argumento = argv[1];
    ifstream fin(argv[2]);  //  segundo argumetno, nome do arquivo que será comrimido ou descomprimido
    
    if(argumento == "c"){
    
        int freqs[256] = {0};    //vetor de frequencias inicialmente com todas posições sendo 0
        MyVec<unsigned char> in; //vetor que ficará o conteúdo do arquivo
        leitura(in,freqs,fin);  //função para ler o arquivo
        fin.close();     //fecha o arquvo pois ja fez a leitura e não precisa mais dele

        HuffManTree arvore(freqs);  //ja tem um o vetor frequência, daí controi a arvore
        MyVec<bool> comprimido;     //vetor para armazenar a codificação comprimida
        arvore.comprimir(comprimido, in); //função que comprimi
        
        char *name = argv[3]; // nome do arquivo que receberá o arquivo comprimido
        
        gerarArquivo(name,freqs,arvore,comprimido); //função que gera arquivo comprimido

    }

    else if(argumento == "d"){
        //arquivo que foi comprimido, agora será descomprimido
        ifstream out(argv[2],ios::binary);
        
        //criamos o vetor frequncias para o  recuperar do arquivo binário 
        int freqs[256] = {0};
        out.read(reinterpret_cast<char *>(freqs),sizeof(int)*256);

        //com o vetor frequencias recuperado, criamos o arvore novamente
        HuffManTree arvore(freqs);
        
        //a partir da arvore, cria os simbolos novamente 
        simbolo s[arvore.getTam()];
        arvore.tabelaDeSimbolos(s);

        //pega o mod/8 , se for 0, não tem resto, caso contrário tem resto
        unsigned char mod;
        out.read(reinterpret_cast<char *>(&mod),sizeof(unsigned char));
        int resto = mod - '0';
        char b[resto];

        //recuperamos o tamanho (qtd de char salvos no arquivo)
        int tamanho ;
        out.read(reinterpret_cast<char *>(&tamanho),sizeof(int));

        //recuperamos os cha salvos no arquivo, fazendo cast para int 
        MyVec<int> numBits;
        unsigned char st;
        
        //enquanto o tamanho for maior que 0, recuperamos os chars salvos
        while(tamanho>0){
            out.read(reinterpret_cast<char *>(&st),sizeof(unsigned char));
            //após ler os os char, fazemos um cast para int para salvar no myvec
            numBits.push_back(int(st));
            tamanho--;
        }

        //se tiver resto recupermaos o vetor de char com o resto
        if(mod != '0'){
            out.read(reinterpret_cast<char *>(b),sizeof(char)*resto);
           
        }

        //fecha o arquivo binário
        out.close();

        //tenho valores int no vetor numBIts, ando bit a bit nas primeiras 8 posiçoes
        //para recuerar os valores bools antigos  saval em comprimido

        MyVec<bool> comprimido;
        int pos = 0; 

        //se o ao andar bit a bit for 1, salva true no vetor de bool e false caso contrário
        for(int i=0;i<numBits.size();i++){
            for(int j=7;j>=0;j--){
                if((numBits[i] & (1<<j)) != 0)
                    comprimido.push_back(true);
                else
                    comprimido.push_back(false);
                pos++;
            }
        }

        //mesmo caso para o vetor de caracter
        for(int i=0;i<resto;i++){
            if(b[i] == '1')
                comprimido.push_back(true);
            else
                comprimido.push_back(false);
            pos++;
        }

        //vetor que receberá o conteúdo do arquivo original
        MyVec<unsigned char> descomprimido;
        //função que descomprimi
        arvore.descomprimir(descomprimido, comprimido);
        
        //nome da arquivo q vai ser criado para salvar o conteúdo do arquivo original
        ofstream saida(argv[3]);

        //como o arquivo já original já está no vetor descomrimido, o savamos no arquivo
        for(int i=0;i<descomprimido.size();i++)
            saida << descomprimido[i];

        //fechamos o arquivo
        saida.close();
    }   

    return 0;
}