#include <iostream>
#include <string>
#include <climits>
#include <fstream>
#include <sstream>
#include <utility>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#define GRAU 4 // Define o grau das árvores B+
#define NTRIE 26
#define ANIME 1
#define MANGA 2

short AbreArquivo(FILE **arq, char nome_arq[], char *modo);
short ArquivoExiste(std::string name);
void ProcessaArquivoCSV(std::string nomecsv_anime, int bin_existe);

// Nodo da árvore B+
struct Nodo{
    public:
        Nodo(std::size_t _grau);
        bool ehFolha;
        std::size_t grau;
        std::size_t numChaves;
        int* chave;
        int* indice;
        Nodo** filhos;
        Nodo* pai;
};

// Árvore B+
class BPTree{
    public:
        BPTree(std::size_t _grau);
        ~BPTree();
        Nodo* getRaiz();
        Nodo* procuraNodo(Nodo* nodo, int id);
        Nodo* procuraEmAlcanceBPTree(Nodo* nodo, int id);
        int procuraEmAlcance(int inicio, int fim, int* resultado);
        int procuraBPTree(int id);
        int achaIndice(int* vetor, int data, int tam);
        int* insereChave(int* vetor_ch, int data, int tam);
        int* insereIndice(int* vetor_ch, int* vetor_in, int data, int index, int tam);
        Nodo** insereFilho(Nodo** vetor_filho, Nodo* filho, int tam, int indice);
        Nodo* insereChaveFilho(Nodo* nodo, int data, int index, Nodo* filho);
        void inserePai(Nodo* pai, Nodo* filho, int data, int index);
        void insereBPTree(int data, int index);
        void limpaBPT(Nodo* cursor);
        void printaBPT();
        void printaNodos(Nodo* cursor);
        void armazenaBPTree(Nodo *cursor, FILE* arq);
        Nodo* raiz;
        std::size_t grau;
};

typedef struct NodoTrie NodoTrie;

struct NodoTrie {
    char caracter;
    NodoTrie* filhos[NTRIE];
    int eh_folha;
    int index;
};

typedef struct trie_string trie_string;

struct trie_string {
    char caracter;
    trie_string* filhos[NTRIE];
    int eh_folha;
    std::vector<int> ids;
};

NodoTrie* cria_NodoTrie(char caracter);

trie_string* cria_trie_string(char caracter);

void free_NodoTrie(NodoTrie* nodo);

void free_trie_string(trie_string* raiz);

NodoTrie* insert_trie(NodoTrie* raiz, char* name, int index);

trie_string* insert_trie_string(trie_string* raiz, char* name, int index);

int busca_trie(NodoTrie* raiz, char* name);

std::vector<int> busca_trie_string(trie_string* raiz, char* name);

int nodo_eh_folha(NodoTrie* raiz, char* name);

void pega_ids(NodoTrie* raiz, std::vector <int> &ids);

NodoTrie* atualiza_ids(NodoTrie* raiz, char* name, int index);

void armazenaTRIE(NodoTrie *raiz, FILE *arq);

void armazenaTRIESTRING(trie_string* raiz, FILE* arq);

NodoTrie* recuperaTRIE(NodoTrie* raiz, FILE* arq);

trie_string* recuperaTRIESTRING(trie_string* raiz, FILE* arq);
