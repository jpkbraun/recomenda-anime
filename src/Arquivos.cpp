#include <iostream>
#include <fstream>
#include "../include/Operacoes.h"

// Função para abrir arquivos pela stdio.h
short AbreArquivo(FILE **arq, char *nome_arq, char *modo){
    *arq = fopen(nome_arq, modo);
    if(*arq == NULL){
        return 0;
    }
    else{
        return 1;
    }
}

// Função para saber se o arquivo existe ou não
short ArquivoExiste(std::string name){
    FILE *file = fopen(name.c_str(), "r");
    if(file){
        fclose(file);
        return 0;
    }
    else{
        return -1;
    }
}


// Função que lê CSVs e cria arquivos binarios
void ProcessaArquivoCSV(std::string nomecsv_anime, int bin_existe){
    // Buffers de leitura de linha
    std::ifstream entrada_anime(nomecsv_anime);
    std::string linha;

    // Declaracao do vetores de registros que contem a database
    std::vector<Anime> dados_entrada_anime;

    if(bin_existe == 1){ // Se o arquivo binário já existe, lê ele primeiro antes do CSV
        std::ifstream arq_entrada_bin;
        Anime buffer_anime;
        arq_entrada_bin.open("anime.bin", std::ios::binary);
        arq_entrada_bin.read((char *) &buffer_anime, sizeof(Anime));
        do{
            dados_entrada_anime.push_back(buffer_anime);
            arq_entrada_bin.read((char *) &buffer_anime, sizeof(Anime));
        }while(!(arq_entrada_bin.eof()));
        arq_entrada_bin.close();
    }

    // Leitura do arquivo CSV
    while(std::getline(entrada_anime, linha)){
        Anime reg1(linha);
        dados_entrada_anime.push_back(reg1);
    }

    /* 2. Após coletar ou extrair os dados, o programa deverá usar arquivos binários próprios que armazenarão todos os dados, independentes dos arquivos originais.
          Tais arquivos poderão ser organizados tanto de forma sequencial, quanto serial. Porém é obrigatório que sejam implementados índices de acesso que auxiliem
          na consulta a esses dados. */

    // Criação das árvores e Manipulação de arquivos binários
    std::ofstream bin_anime;
    bin_anime.open("anime.bin", std::ios::binary);
    BPTree bpt_anime(GRAU), bpt_manga(GRAU);
    NodoTrie* raiztrie_anime = cria_NodoTrie('\0');
    for(unsigned int i = 0; i < dados_entrada_anime.size(); i++){
        bpt_anime.insereBPTree(dados_entrada_anime[i].id, i);
        char* nome = (char*) calloc (strlen(dados_entrada_anime[i].name) + 1, sizeof(char));
        for (unsigned int j = 0; j < strlen(dados_entrada_anime[i].name); j++)
        {
            nome[j] = dados_entrada_anime[i].name[j];
        }
        raiztrie_anime = insert_trie(raiztrie_anime, nome, i);
        free(nome);
        bin_anime.write(reinterpret_cast<char *>(&(dados_entrada_anime[i])), sizeof(dados_entrada_anime[i]));
    }
    bin_anime.close();

    char bpt_anime_arq[] = "bpt_anime.bin", writeb[] = "wb";
    FILE *bin_bpt_anime = NULL;
    AbreArquivo(&bin_bpt_anime, bpt_anime_arq, writeb);
    bpt_anime.armazenaBPTree(bpt_anime.getRaiz(), bin_bpt_anime);
    fclose(bin_bpt_anime);

    char bin_trie_arq1[] = "trie_anime.bin";
    FILE *bin_trie1 = NULL;
    AbreArquivo(&bin_trie1, bin_trie_arq1, writeb);
    armazenaTRIE(raiztrie_anime, bin_trie1);
    fclose(bin_trie1);
    free_NodoTrie(raiztrie_anime);


    FILE *arq_licensors = NULL, *arq_genres = NULL, *arq_studios = NULL;
    char nome_licensors[] = "licensors.bin", nome_genres[] = "genres.bin", nome_studios[] = "studios.bin";
    trie_string* raiz_licensors = cria_trie_string('\0');
    raiz_licensors = cria_arq_inv(raiz_licensors, LICENSORS);
    AbreArquivo(&arq_licensors, nome_licensors, writeb);
    armazenaTRIESTRING(raiz_licensors, arq_licensors);
    fclose(arq_licensors);
    free_trie_string(raiz_licensors);

    trie_string* raiz_genres = cria_trie_string('\0');
    raiz_genres = cria_arq_inv(raiz_genres, GENRES);
    AbreArquivo(&arq_genres, nome_genres, writeb);
    armazenaTRIESTRING(raiz_genres, arq_genres);
    fclose(arq_genres);
    free_trie_string(raiz_genres);

    trie_string* raiz_studios = cria_trie_string('\0');
    raiz_studios = cria_arq_inv(raiz_studios, STUDIOS);
    AbreArquivo(&arq_studios, nome_studios, writeb);
    armazenaTRIESTRING(raiz_studios, arq_studios);
    fclose(arq_studios);
    free_trie_string(raiz_studios);

}

Nodo::Nodo(std::size_t _grau){ // Construtor de Nodo
    this->ehFolha = false;
    this->grau = _grau;
    this->numChaves = 0;
    int* _chave = new int[grau - 1];
    int* _indice = new int[grau - 1];
    for(unsigned int i = 0; i < (grau - 1); i++){
        _chave[i] = 0;
        _indice[i] = 0;
    }
    this->chave = _chave;
    this->indice = _indice;
    Nodo** _filhos = new Nodo*[grau];
    for(unsigned int i=0; i<grau; i++){
        _filhos[i] = nullptr;
    }
    this->filhos = _filhos;
    this->pai = nullptr;
}

BPTree::BPTree(std::size_t _grau){ // Construtor de árvore B+
    this->raiz = nullptr;
    this->grau = _grau;
}

BPTree::~BPTree(){ // Destrutor
    limpaBPT(this->raiz);
}

Nodo* BPTree::getRaiz(){
    return this->raiz;
}

Nodo* BPTree::procuraNodo(Nodo* nodo, int id){
    if(nodo == nullptr){ // Se a árvore é vazia, devolve NULL
        return nullptr;
    }
    else{
        Nodo* cursor = nodo;
        while(!cursor->ehFolha){
            for(unsigned int i = 0; i < cursor->numChaves; i++){
                if(id < cursor->chave[i]){
                    cursor = cursor->filhos[i];
                    break;
                }
                if(i == (cursor->numChaves - 1)){
                    cursor = cursor->filhos[i + 1];
                    break;
                }
            }
        }
        for(unsigned int i = 0; i < cursor->numChaves; i++){
            if(cursor->chave[i] == id){
                return cursor; // Retorna nodo que contém chave procurada
            }
        }
        return nullptr; // Se chave não está na árvore, devolve NULL
    }
}

Nodo* BPTree::procuraEmAlcanceBPTree(Nodo* nodo, int id){
    if(nodo == nullptr){
        return nullptr; // Retorna NULL, se o nodo for vazio
    }
    else{
        Nodo* cursor = nodo;
        while(!cursor->ehFolha){
            for(unsigned int i = 0; i < cursor->numChaves; i++){ // Percorre a árvore até a folha
                if(id < cursor->chave[i]){
                    cursor = cursor->filhos[i];
                    break;
                }
                if(i == (cursor->numChaves - 1)){
                    cursor = cursor->filhos[i + 1];
                    break;
                }
            }
        }
        return cursor; // Retorna nodo que contém a chave
    }
}

int BPTree::procuraEmAlcance(int inicio, int fim, int* resultado){
    int index = 0;
    Nodo* nodo_inicio = procuraEmAlcanceBPTree(this->raiz, inicio);
    Nodo* cursor = nodo_inicio;
    int temp = cursor->chave[0];
    while(temp <= fim){
        if(cursor == nullptr){
            break;
        }
        for(unsigned int i = 0; i < cursor->numChaves; i++){
            temp = cursor->chave[i];
            if((temp >= inicio) && (temp <= fim)){
                resultado[index] = temp; // Preenche o vetor com chaves dentro do range procurado
                index++;
            }
        }
        cursor = cursor->filhos[cursor->numChaves];
    }
    return index; // Retorna indice que termina o range procurado
}

int BPTree::procuraBPTree(int id){
    Nodo* nodo = procuraNodo(this->raiz, id);
    if(nodo == nullptr){
        return -1; // Retorna -1, se a chave não está contida na B+
    }
    else{
        for(unsigned int i = 0; i < nodo->numChaves; i++){
            if(nodo->chave[i] == id){
                return (nodo->indice[i]); // Retorna o indice se existe
            }
        }
    }
    return -1; // Retorna -1, se a chave não está contida na B+
}


int BPTree::achaIndice(int* vetor, int data, int tam){ // Acha indice ordenado de uma chave em um vetor
    int index = 0;
    for(int i = 0; i < tam; i++){
        if(data < vetor[i]){
            index = i;
            break;
        }
        if(i == (tam - 1)){
            index = tam;
            break;
        }
    }
    return index;
}

int* BPTree::insereChave(int* vetor_ch, int data, int tam){ // Insere chave dentro de um vetor de chaves, de forma ordenada
    int indice = 0;
    for(int i = 0; i < tam; i++){
        if(data < vetor_ch[i]){
            indice = i;
            break;
        }
        if(i == (tam-1)){
            indice = tam;
            break;
        }
    }
    for(int i = tam; i > indice; i--){
        vetor_ch[i] = vetor_ch[i - 1];
    }
    vetor_ch[indice] = data;
    return vetor_ch;
}

int* BPTree::insereIndice(int* vetor_ch, int* vetor_in, int data, int index, int tam){ // Insere indice dentro de um vetor de indices, ordenado pelas chaves
    int indice = 0;
    for(int i = 0; i < tam; i++){
        if(data < vetor_ch[i]){
            indice = i;
            break;
        }
        if(i == (tam - 1)){
            indice = tam;
            break;
        }
    }
    for(int i = tam; i > indice; i--){
        vetor_in[i] = vetor_in[i - 1];
    }
    vetor_in[indice] = index;
    return vetor_in;
}

Nodo** BPTree::insereFilho(Nodo** vetor_filho, Nodo* filho, int tam, int indice){ // Insere filho no nodo
    for(int i = tam; i > indice; i--){
        vetor_filho[i] = vetor_filho[i - 1];
    }
    vetor_filho[indice] = filho;
    return vetor_filho;
}

Nodo* BPTree::insereChaveFilho(Nodo* nodo, int data, int index, Nodo* filho){ // Insere chave no nodo e aponta pro filho
    int chave_index = 0;
    int filho_index = 0;
    for(unsigned int i = 0; i < nodo->numChaves; i++){
        if(data < nodo->chave[i]){
            chave_index = i;
            filho_index = i + 1;
            break;
        }
        if(i == (nodo->numChaves - 1)){
            chave_index = nodo->numChaves;
            filho_index = nodo->numChaves + 1;
            break;
        }
    }
    for(int i = nodo->numChaves; i > chave_index; i--){
        nodo->chave[i] = nodo->chave[i - 1];
        nodo->indice[i] = nodo->indice[i - 1];
    }
    for(int i = (nodo->numChaves + 1); i > filho_index; i--){
        nodo->filhos[i] = nodo->filhos[i - 1];
    }
    nodo->chave[chave_index] = data;
    nodo->indice[chave_index] = index;
    nodo->filhos[filho_index] = filho;
    return nodo;
}

void BPTree::inserePai(Nodo* pai, Nodo* filho, int data, int index){ // Insere alterando raiz da subarvore local
    Nodo* cursor = pai;
    if(cursor->numChaves < (this->grau - 1)){
        cursor = insereChaveFilho(cursor, data, index, filho);
        cursor->numChaves++;
    }
    else{
        auto* n_nodo = new Nodo(this->grau);
        n_nodo->pai = cursor->pai;
        int* buffer_ch = new int[cursor->numChaves + 1];
        int* buffer_in = new int[cursor->numChaves + 1];
        for(unsigned int i = 0; i < cursor->numChaves; i++){
            buffer_ch[i] = cursor->chave[i];
            buffer_in[i] = cursor->indice[i];
        }
        buffer_in = insereIndice(buffer_ch, buffer_in, data, index, cursor->numChaves);
        buffer_ch = insereChave(buffer_ch, data, cursor->numChaves);
        auto** buffer_filho = new Nodo*[cursor->numChaves + 2];
        for(unsigned int i = 0; i < (cursor->numChaves + 1); i++){
            buffer_filho[i] = cursor->filhos[i];
        }
        buffer_filho[cursor->numChaves + 1] = nullptr;
        buffer_filho = insereFilho(buffer_filho, filho, (cursor->numChaves + 1), achaIndice(buffer_ch, data, (cursor->numChaves + 1)));
        cursor->numChaves = (this->grau) / 2;
        if((this->grau) % 2 == 0){
            n_nodo->numChaves = (this->grau) / 2 - 1;
        }
        else{
            n_nodo->numChaves = (this->grau) / 2;
        }
        for(unsigned int i = 0; i < cursor->numChaves; i++){
            cursor->chave[i] = buffer_ch[i];
            cursor->indice[i] = buffer_in[i];
            cursor->filhos[i] = buffer_filho[i];
        }
        cursor->filhos[cursor->numChaves] = buffer_filho[cursor->numChaves];
        for(unsigned int i = 0; i < n_nodo->numChaves; i++){
            n_nodo->chave[i] = buffer_ch[cursor->numChaves + i +1];
            n_nodo->indice[i] = buffer_in[cursor->numChaves + i +1];
            n_nodo->filhos[i] = buffer_filho[cursor->numChaves + i + 1];
            n_nodo->filhos[i]->pai = n_nodo;
        }
        n_nodo->filhos[n_nodo->numChaves] = buffer_filho[cursor->numChaves + n_nodo->numChaves + 1];
        n_nodo->filhos[n_nodo->numChaves]->pai = n_nodo;
        int ch_pai = buffer_ch[this->grau / 2];
        int in_pai = buffer_in[this->grau / 2];
        delete[] buffer_ch;
        delete[] buffer_in;
        delete[] buffer_filho;
        if(cursor->pai == nullptr){
            auto* n_pai = new Nodo(this->grau);
            cursor->pai = n_pai;
            n_nodo->pai = n_pai;
            n_pai->chave[0] = ch_pai;
            n_pai->indice[0] = in_pai;
            n_pai->numChaves++;
            n_pai->filhos[0] = cursor;
            n_pai->filhos[1] = n_nodo;
            this->raiz = n_pai;
        }
        else{
            inserePai(cursor->pai, n_nodo, ch_pai, in_pai);
        }
    }
}

void BPTree::insereBPTree(int data, int index){ // Insere na árvore B+
    if(this->raiz == nullptr){
        this->raiz = new Nodo(this->grau);
        this->raiz->ehFolha = true;
        this->raiz->chave[0] = data;
        this->raiz->indice[0] = index;
        this->raiz->numChaves = 1;
    }
    else{
        Nodo* cursor = this->raiz;
        cursor = procuraEmAlcanceBPTree(cursor, data);
        if(cursor->numChaves < (this->grau - 1)){
            cursor->indice = insereIndice(cursor->chave, cursor->indice, data, index, cursor->numChaves);
            cursor->chave = insereChave(cursor->chave, data, cursor->numChaves);
            cursor->numChaves++;
            cursor->filhos[cursor->numChaves] = cursor->filhos[cursor->numChaves - 1];
            cursor->filhos[cursor->numChaves - 1] = nullptr;
        }
        else{
            auto* n_nodo = new Nodo(this->grau);
            n_nodo->ehFolha = true;
            n_nodo->pai = cursor->pai;
            int* buffer_ch = new int[cursor->numChaves + 1];
            int* buffer_in = new int[cursor->numChaves + 1];
            for(unsigned int i = 0; i < cursor->numChaves; i++){
                buffer_ch[i] = cursor->chave[i];
                buffer_in[i] = cursor->indice[i];
            }
            buffer_in = insereIndice(buffer_ch, buffer_in, data, index, cursor->numChaves);
            buffer_ch = insereChave(buffer_ch, data, cursor->numChaves);
            cursor->numChaves = (this->grau) / 2;
            if((this->grau) % 2 == 0){
                n_nodo->numChaves = (this->grau) / 2;
            }
            else{
                n_nodo->numChaves = (this->grau) / 2 + 1;
            }
            for(unsigned int i = 0; i < cursor->numChaves; i++){
                cursor->chave[i] = buffer_ch[i];
                cursor->indice[i] = buffer_in[i];
            }
            for(unsigned int i = 0; i < n_nodo->numChaves; i++){
                n_nodo->chave[i] = buffer_ch[cursor->numChaves + i];
                n_nodo->indice[i] = buffer_in[cursor->numChaves + i];
            }
            cursor->filhos[cursor->numChaves] = n_nodo;
            n_nodo->filhos[n_nodo->numChaves] = cursor->filhos[this->grau - 1];
            cursor->filhos[this->grau - 1] = nullptr;
            delete[] buffer_ch;
            delete[] buffer_in;
            int ch_pai = n_nodo->chave[0];
            int in_pai = n_nodo->indice[0];
            if(cursor->pai == nullptr){
                auto* n_pai = new Nodo(this->grau);
                cursor->pai = n_pai;
                n_nodo->pai = n_pai;
                n_pai->chave[0] = ch_pai;
                n_pai->indice[0] = in_pai;
                n_pai->numChaves++;
                n_pai->filhos[0] = cursor;
                n_pai->filhos[1] = n_nodo;
                this->raiz = n_pai;
            }
            else{
                inserePai(cursor->pai, n_nodo, ch_pai, in_pai);
            }
        }
    }
}

void BPTree::limpaBPT(Nodo* cursor){ // Deleta a árvore recursivamente
    if(cursor != nullptr){
        if(!cursor->ehFolha){
            for(unsigned int i=0; i <= cursor->numChaves; i++){
                limpaBPT(cursor->filhos[i]);
            }
        }
        delete[] cursor->chave;
        delete[] cursor->indice;
        delete[] cursor->filhos;
        delete cursor;
    }
}

void BPTree::printaBPT(){
    printaNodos(this->raiz);
}

void BPTree::printaNodos(Nodo* cursor){ // Printa a arvore recursivamente
    if(cursor != NULL){
        for(unsigned int i = 0; i < cursor->numChaves; ++i){
            std::cout << cursor->chave[i] << "." << cursor->indice[i] << " ";
        }
        std::cout << "\n";
        if(!cursor->ehFolha){
            for(unsigned int i = 0; i < (cursor->numChaves + 1); ++i){
                printaNodos(cursor->filhos[i]);
            }
        }
    }
}

void BPTree::armazenaBPTree(Nodo *cursor, FILE* arq){ // Função para armazenar árvore B+ em binário recursivamente
    int aux1, aux2;
    if(cursor != NULL){
        for(unsigned int i = 0; i < cursor->numChaves; i++){
            if(cursor->ehFolha == true){
                aux1 = cursor->chave[i];
                aux2 = cursor->indice[i];
                fwrite(&aux1, sizeof(aux1), 1, arq);
                fwrite(&aux2, sizeof(aux2), 1, arq);
            }
        }
        if(cursor->ehFolha != true){
            for(unsigned int i = 0; i < (cursor->numChaves + 1); i++){
                armazenaBPTree(cursor->filhos[i], arq);
            }
        }
    }
}

NodoTrie* cria_NodoTrie(char caracter) {
    // Aloca memoria para um NodoTrie
    NodoTrie* nodo = (NodoTrie*) calloc (1, sizeof(NodoTrie));
    for (int i=0; i<NTRIE; i++)
        nodo->filhos[i] = NULL;
    nodo->eh_folha = 0;
    nodo->caracter = caracter;
    nodo->index = -1;
    return nodo;
}

trie_string* cria_trie_string(char caracter)
{   // Aloca memoria para um nodo trie_string, um nodo TRIE que guarda um vetor de inteiros
    trie_string* nodo = (trie_string*) calloc (1, sizeof(trie_string));
    for (int i=0; i<NTRIE; i++)
        nodo->filhos[i] = NULL;
    nodo->eh_folha = 0;
    nodo->caracter = caracter;
    nodo->ids = {};
    return nodo;
}

void free_NodoTrie(NodoTrie* nodo) {
    // Desaloca  a memoria ocupada pelo nodo e todos os seus filhos
    for(int i=0; i<NTRIE; i++) {
        if (nodo->filhos[i] != NULL) {
            free_NodoTrie(nodo->filhos[i]);
        }
    }
    free(nodo);
}

void free_trie_string(trie_string* raiz)
{   // Desaloca a memoria ocupada pelo nodo trie_string e todos os seus filhos
    for (int i=0; i<NTRIE; i++)
    {
        if (raiz->filhos[i] != NULL)
        {
            free_trie_string(raiz->filhos[i]);
        }
    }
    free(raiz);
}

NodoTrie* insert_trie(NodoTrie* raiz, char* name, int index) {
    // Insere o nome do manga ou do anime numa arvore TRIE
    NodoTrie* temp = raiz;
    for (int i=0; name[i] != '\0'; i++) {
        if (name[i] <= 'Z' && name[i] >= 'A')
        {
            name[i] += 32;
        }
        if(name[i] <= 'z' && name[i] >= 'a'){
            // Pega a posicao em que a letra sera filha do nodo
            int j = name[i] - 'a';
            if (temp->filhos[j] == NULL) {
                // Se o filho ainda nao existe, cria o filho
                temp->filhos[j] = cria_NodoTrie(name[i]);
            }
            temp = temp->filhos[j];
        }
    }
    // Temp eh folha
    temp->eh_folha = 1;
    // Temp recebe index da lista onde o dado se encontra
    temp->index = index;
    return raiz;
}

trie_string* insert_trie_string(trie_string* raiz, char* name, int index)
{
    // Insere o nome da string para arquivo invertido numa arvore TRIE
    trie_string* temp = raiz;
    int i = 0;
    while (name[i] != '\0')
    {
        while (name[i] != ';' && name[i] != '\0')
        {
            if (name[i] <= 'Z' && name[i] >= 'A')
            {
                name[i] += 32;
            }
            if(name[i] <= 'z' && name[i] >= 'a'){
                // Pega a posicao em que a letra sera filha do nodo
                int j = name[i] - 'a';
                if (temp->filhos[j] == NULL) {
                    // Se o filho ainda nao existe, cria o filho
                    temp->filhos[j] = cria_trie_string(name[i]);
                }
                temp = temp->filhos[j];
            }
            i++;
        }
        if (name[i] == ';')
        {   // Separador de palavras foi encontrado, esse eh o fim de um ramo
            temp->eh_folha = 1;
            temp->ids.push_back(index);
            i++;
            temp = raiz;
        }
    }

    temp->eh_folha = 1;
    temp->ids.push_back(index); // Ultima palavra foi lida
    return raiz;
}

int busca_trie(NodoTrie* raiz, char* name)
{
    // Procura pelo nome do anime/manga na TRIE
    NodoTrie* temp = raiz;

    for(int i=0; name[i]!='\0'; i++)
    {
        if (name[i] <= 'Z' && name[i] >= 'A')
        {   // Tratamento de maiusculas para se tornarem minusculas
            name[i] += 32;
        }
        if(name[i] <= 'z' && name[i] >= 'a'){
            int j = name[i] - 'a';
            if (temp->filhos[j] == NULL){
                return -1;
            }
            temp = temp->filhos[j];
        }
    }
    if (temp != NULL && temp->eh_folha == 1)
    {   // Nome encontrado, retorna o seu indice
        return temp->index;
    }
    return -1;
}

std::vector<int> busca_trie_string(trie_string* raiz, char* name)
{   // Funcao analoga a busca_trie, mas seu retorno eh um vetor de inteiros
    trie_string* temp = raiz;

    for(int i=0; name[i]!='\0'; i++)
    {
        if (name[i] <= 'Z' && name[i] >= 'A')
        {
            name[i] += 32;
        }
        if(name[i] <= 'z' && name[i] >= 'a'){
            int j = name[i] - 'a';
            if (temp->filhos[j] == NULL){
                return {};
            }
            temp = temp->filhos[j];
        }
    }
    if (temp != NULL && temp->eh_folha == 1)
    {
        return temp->ids;
    }
    return {};
}

int nodo_eh_folha(NodoTrie* raiz, char* name) {
    // Checa se o nome na raiz passada eh um nodo folha
    NodoTrie* temp = raiz;
    for (int i=0; name[i]; i++) {
        if (name[i] <= 'Z' && name[i] >= 'A')
        {
            name[i] += 32;
        }
        if(name[i] <= 'z' && name[i] >= 'a'){
            int j = name[i] - 'a';
            if (temp->filhos[j]) {
                temp = temp->filhos[j];
            }
        }
    }
    return temp->eh_folha;
}

void pega_ids(NodoTrie* raiz, std::vector <int> &ids) {
    // Funcao que coleta todos ids a partir de um determinado nodo de TRIE
    if (!raiz)
        return;

    NodoTrie* temp = raiz;
    if (temp->eh_folha)
    {
        ids.push_back(temp->index);
    }
    for (int i=0; i<NTRIE; i++) {
        pega_ids(temp->filhos[i], ids);
    }
}

NodoTrie* atualiza_ids(NodoTrie* raiz, char* name, int index)
{
    // Funcao responsavel por atualizar ID de um registro na TRIE
    NodoTrie* temp = raiz;
    for (int i=0; name[i] != '\0'; i++) {
        if (name[i] <= 'Z' && name[i] >= 'A')
        {
            name[i] += 32;
        }
        if(name[i] <= 'z' && name[i] >= 'a'){
            int j = name[i] - 'a';
            temp = temp->filhos[j];
        }
    }
    // Temp eh folha
    temp->index = index;
    return raiz;
}

void armazenaTRIE(NodoTrie* raiz, FILE* arq)
{
    // Funcao responsavel por armazenar um NodoTrie em binario
    char caracter;
    int folha, idx;
    NodoTrie* filhos[NTRIE];
    NodoTrie* temp = raiz;

    if (temp != NULL)
    {
        caracter = temp->caracter;
        folha = temp->eh_folha;
        idx = temp->index;
        for (int i = 0; i < NTRIE; i++)
        {
            filhos[i] = temp->filhos[i];
        }
        // Escreve no arquivo binario os valores do nodo
        fwrite(&caracter, sizeof(caracter), 1, arq);
        fwrite(&folha, sizeof(folha), 1, arq);
        fwrite(&idx, sizeof(idx), 1, arq);
        for (int i = 0; i < NTRIE; i++)
        {   // Segue escrevendo recursivamente
            fwrite(&filhos[i], sizeof(filhos[i]), 1, arq);
            armazenaTRIE(filhos[i], arq);
        }
    }
}

void armazenaTRIESTRING(trie_string* raiz, FILE* arq)
{
    // Funcao responsavel por armazenar um trie_string em binario
    char caracter;
    int folha;
    char especial = '@';
    std::vector<int> ids = {};
    trie_string* filhos[NTRIE];
    trie_string* temp = raiz;

    if (temp != NULL)
    {
        caracter = temp->caracter;
        folha = temp->eh_folha;
        unsigned int tamanho = temp->ids.size();
        for (unsigned int i = 0; i < tamanho; i++)
        {
            ids.push_back(temp->ids[i]);
        }
        for (int i = 0; i < NTRIE; i++)
        {
            filhos[i] = temp->filhos[i];
        }
        // Escreve no arquivo binario os valores do nodo
        fwrite(&especial, sizeof(especial), 1, arq);
        fwrite(&tamanho, sizeof(tamanho), 1, arq);
        for (unsigned int i = 0; i < tamanho; i++)
        {
            fwrite(&ids[i], sizeof(ids[i]), 1, arq);
        }
        fwrite(&caracter, sizeof(caracter), 1, arq);
        fwrite(&folha, sizeof(folha), 1, arq);
        for (int i = 0; i < NTRIE; i++)
        {   // Segue escrevendo recursivamente
            fwrite(&filhos[i], sizeof(filhos[i]), 1, arq);
            armazenaTRIESTRING(filhos[i], arq);
        }
    }
}

NodoTrie* recuperaTRIE(NodoTrie* raiz, FILE* arq)
{
    // Funcao responsavel por recuperar um NodoTrie de um arquivo binario
    char buffer_caracter;
    int buffer_folha, buffer_idx;
    NodoTrie* buffer_filhos[NTRIE];

    NodoTrie* temp = raiz;
    NodoTrie* buffer;

    // Le as variaveis do arquivo binario
    fread(&buffer_caracter, sizeof(buffer_caracter), 1, arq);
    fread(&buffer_folha, sizeof(buffer_folha), 1, arq);
    fread(&buffer_idx, sizeof(buffer_idx), 1, arq);

    temp->caracter = buffer_caracter;
    temp->eh_folha = buffer_folha;
    temp->index = buffer_idx;
    for (int i = 0; i < NTRIE; i++)
    {
        fread(&buffer_filhos[i], sizeof(buffer_filhos[i]), 1, arq);
        temp->filhos[i] = buffer_filhos[i];
        if (temp->filhos[i] != NULL)
        {
            // Aloca memoria e recursivamente continua lendo o arquivo
            buffer = cria_NodoTrie('\0');
            temp->filhos[i] = buffer;
            temp->filhos[i] = recuperaTRIE(temp->filhos[i], arq);
        }
    }
    return raiz;
}

trie_string* recuperaTRIESTRING(trie_string* raiz, FILE* arq)
{
    // Funcao responsavel por recuperar um trie_string de um arquivo binario
    char buffer_caracter;
    int buffer_folha;
    char buffer_especial;
    int buffer_id;
    std::vector<int> buffer_ids = {};
    unsigned int buffer_tamanho;
    trie_string* buffer_filhos[NTRIE];

    trie_string* temp = raiz;
    trie_string* buffer;

    // Le as variaveis do arquivo binario
    fread(&buffer_especial, sizeof(buffer_especial), 1, arq);
    fread(&buffer_tamanho, sizeof(buffer_tamanho), 1, arq);
    for (unsigned int i = 0; i < buffer_tamanho; i++)
    {
        fread(&buffer_id, sizeof(buffer_id), 1, arq);
        buffer_ids.push_back(buffer_id);
    }
    fread(&buffer_caracter, sizeof(buffer_caracter), 1, arq);
    fread(&buffer_folha, sizeof(buffer_folha), 1, arq);

    temp->caracter = buffer_caracter;
    temp->eh_folha = buffer_folha;
    for (unsigned int i = 0; i < buffer_tamanho; i++)
    {
        temp->ids.push_back(buffer_ids[i]);
    }
    for (int i = 0; i < NTRIE; i++)
    {
        fread(&buffer_filhos[i], sizeof(buffer_filhos[i]), 1, arq);
        temp->filhos[i] = buffer_filhos[i];
        if (temp->filhos[i] != NULL)
        {
            // Aloca memoria e recursivamente continua lendo o arquivo
            buffer = cria_trie_string('\0');
            temp->filhos[i] = buffer;
            temp->filhos[i] = recuperaTRIESTRING(temp->filhos[i], arq);
        }
    }
    return raiz;
}
