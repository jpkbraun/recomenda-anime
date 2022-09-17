#include "../include/Registro.h"
#include "../include/Arquivos.h"
#include <cmath>
#define GENRES 0
#define LICENSORS 1
#define STUDIOS 2

void buscaAnimePorPrefixo(char* nome);
void buscaAnimePorID(int id);
void ordenaAnime();
void ordenaAnimeInverso();
void deletaAnime(int id);

std::vector<int> geraSequenciaDeGaps(int tam);

trie_string* cria_arq_inv(trie_string* raiz, int tipo);

void Busca_Um_Campo(char* nome, int tipo);
void Busca_Dois_Campos(char* nome1, char* nome2);
void Busca_Dois_Mesmo_Campo(char* nome1, char* nome2);

void recomendaAnime(int id);
