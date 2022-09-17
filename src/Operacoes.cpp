#include "../include/Operacoes.h"

void buscaAnimePorID(int id){
    // Funcao responsavel por buscar animes pelo seu ID
    Anime buffer_anime;
    BPTree buffer_bpt(GRAU);
    int buffer_ch, buffer_in;
    std::ifstream arq, arq_bpt;
    // Le o arquivo binario das bpt e gera sua estrutura
    arq_bpt.open("bpt_anime.bin", std::ios::binary);
    arq_bpt.read((char *) &buffer_ch, sizeof(int));
    arq_bpt.read((char *) &buffer_in, sizeof(int));
    do{
        buffer_bpt.insereBPTree(buffer_ch, buffer_in);
        arq_bpt.read((char *) &buffer_ch, sizeof(int));
        arq_bpt.read((char *) &buffer_in, sizeof(int));
    }while(!(arq_bpt.eof()));
    // Procura o id na bpt
    buffer_in = buffer_bpt.procuraBPTree(id);
    arq_bpt.close();
    if(buffer_in == -1){    // ID nao encontrado
        std::cout << "ID invalido" << std::endl << std::endl;
    }
    else{   // Encontra o anime em seu arquivo binario e o imprime
        arq.open("anime.bin", std::ios::binary);
        for(int i = 0; i <= buffer_in; i++){
            arq.read((char *) &buffer_anime, sizeof(Anime));
        }
        arq.close();
        buffer_anime.printaAnime();
    }
}

void buscaAnimePorPrefixo(char* nome)
{
    // Funcao responsavel por imprimir na tela todos os animes
    // que inicam por "nome" e seus indices para futuras pesquisas

    // Declaracao de variaveis e abertura do arquivo binario de TRIE
    Anime buffer_anime;
    NodoTrie* raiz = cria_NodoTrie('\0');
    NodoTrie* raiz_original = raiz;
    FILE *arqAnime, *arqTrie;
    char nome_trie_bin[] = "trie_anime.bin", readbinary[] = "rb";
    AbreArquivo(&arqTrie, nome_trie_bin, readbinary);
    recuperaTRIE(raiz, arqTrie);
    fclose(arqTrie);

    for(int i=0; nome[i]!='\0'; i++)
    {
        if (nome[i] <= 'Z' && nome[i] >= 'A')
        {
            nome[i] += 32;
        }
        if(nome[i] <= 'z' && nome[i] >= 'a'){
            int j = nome[i] - 'a';
            if (raiz->filhos[j] != NULL){
                raiz = raiz->filhos[j];
            }
            else
            {   // Nao ha animes que iniciam por nome
                std::cout << "Erro, nao ha animes que iniciam com " << nome << std::endl;
                free_NodoTrie(raiz_original);
                return;
            }
        }
    }

    // Abre o binario de animes para recuperar os registros
    std::vector<Anime> dados_entrada_anime;
    std::vector<int> ids;
    char nome_anime_bin[] = "anime.bin";
    AbreArquivo(&arqAnime, nome_anime_bin, readbinary);
    fread(&buffer_anime, sizeof(buffer_anime), 1, arqAnime);
    do{
        dados_entrada_anime.push_back(buffer_anime);
        fread(&buffer_anime, sizeof(buffer_anime), 1, arqAnime);
    }while(!feof(arqAnime));
    fclose(arqAnime);
    // Coleta o vetor de indices para acessar diretamente no vetor dos registros
    pega_ids(raiz, ids);

    for (unsigned int i = 0; i < ids.size(); i++)
    {   // Imprime todos os animes e seus respectivos IDs
        std::cout << "Encontrado: " << dados_entrada_anime[ids[i]].name << std::endl << "E seu ID: " << dados_entrada_anime[ids[i]].id << std::endl;
    }
    // Desaloca memoria da TRIE
    free_NodoTrie(raiz_original);
}

void ordenaAnime(){
    // Funcao responsavel por ordenar o vetor de ANIME
    // e imprimir os top N (3) de acordo com seu ranking

    // Leitura do arquivo
    std::vector<Anime> dados_entrada_anime;
    Anime buffer_anime;
    std::ifstream arq_entrada;
    arq_entrada.open("anime.bin", std::ios::binary);
    arq_entrada.read((char *) &buffer_anime, sizeof(Anime));
    do{
        dados_entrada_anime.push_back(buffer_anime);
        arq_entrada.read((char *) &buffer_anime, sizeof(Anime));
    }while(!(arq_entrada.eof()));
    arq_entrada.close();

    // Ordenação via shellSort usando o ranking como parametro
    int tamanhogaps;
    int n = dados_entrada_anime.size();
    int i = 0, j, k, l, h = 1;
    std::vector<int> sequenciaDeGaps = geraSequenciaDeGaps(n);
    tamanhogaps = sequenciaDeGaps.size();
    do{
        i++;
    }while(i < tamanhogaps);
    i = i - 2;
    h = sequenciaDeGaps[i];
    while(h >= 1){
        for(j = 0; j < h; j++){
            for(k = j + h; k < n; k = k + h){
                if((dados_entrada_anime[k].ranked == -1) || (dados_entrada_anime[k].ranked == 0)){ // Caso a informação for desconhecida, joga para o fim
                    dados_entrada_anime[k].ranked = 99999;
                }
                buffer_anime = dados_entrada_anime[k];
                l = k - h;
                while((l >= 0) && (dados_entrada_anime[l].ranked > buffer_anime.ranked)){
                    dados_entrada_anime[l + h] = dados_entrada_anime[l];
                    l = l - h;
                }
                dados_entrada_anime[l + h] = buffer_anime;
            }
        }
        if(i != 0){
            i--;
            h = sequenciaDeGaps[i];
        }
        else{
            h = 0;
        }
    }

    // Escrita no arquivo binario
    BPTree bpt_anime(GRAU);
    std::ofstream bin_anime;
    bin_anime.open("anime.bin", std::ios::binary);
    for(unsigned int i = 0; i < dados_entrada_anime.size(); i++){
        if(dados_entrada_anime[i].ranked == 99999){ // Correção
            dados_entrada_anime[i].ranked = -1;
        }
        bpt_anime.insereBPTree(dados_entrada_anime[i].id, i);
        bin_anime.write(reinterpret_cast<char *>(&(dados_entrada_anime[i])), sizeof(dados_entrada_anime[i]));
    }
    bin_anime.close();
    char bpt_anime_arq[] = "bpt_anime.bin", writeb[] = "wb";
    FILE *bin_bpt_anime = NULL;
    AbreArquivo(&bin_bpt_anime, bpt_anime_arq, writeb);
    bpt_anime.armazenaBPTree(bpt_anime.getRaiz(), bin_bpt_anime);
    fclose(bin_bpt_anime);

    std::cout << std::endl << "TOP 3 ANIMES:" << std::endl;
    dados_entrada_anime[0].printaAnime();
    dados_entrada_anime[1].printaAnime();
    dados_entrada_anime[2].printaAnime();

    // Update da TRIE
    NodoTrie* raiz_trie = cria_NodoTrie('\0');
    FILE *arq_trie_leitura = NULL;
    char nome_trie_bin[] = "trie_anime.bin", readbinary[] = "rb";
    AbreArquivo(&arq_trie_leitura, nome_trie_bin, readbinary);
    raiz_trie = recuperaTRIE(raiz_trie, arq_trie_leitura);
    fclose(arq_trie_leitura);
    for (unsigned int i = 0; i < dados_entrada_anime.size(); i++)
    {
        raiz_trie = atualiza_ids(raiz_trie, dados_entrada_anime[i].name, i);
    }
    FILE *arq_trie_escrita = NULL;
    AbreArquivo(&arq_trie_escrita, nome_trie_bin, writeb);
    armazenaTRIE(raiz_trie, arq_trie_escrita);
    fclose(arq_trie_escrita);
    free_NodoTrie(raiz_trie);

    // Update dos arquivos invertidos
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

void ordenaAnimeInverso(){
    // Leitura do arquivo
    std::vector<Anime> dados_entrada_anime;
    Anime buffer_anime;
    std::ifstream arq_entrada;
    arq_entrada.open("anime.bin", std::ios::binary);
    arq_entrada.read((char *) &buffer_anime, sizeof(Anime));
    do{
        dados_entrada_anime.push_back(buffer_anime);
        arq_entrada.read((char *) &buffer_anime, sizeof(Anime));
    }while(!(arq_entrada.eof()));
    arq_entrada.close();

    // Ordenação via shellSort usando o ranking como parametro
    int tamanhogaps;
    int n = dados_entrada_anime.size();
    int i = 0, j, k, l, h = 1;
    std::vector<int> sequenciaDeGaps = geraSequenciaDeGaps(n);
    tamanhogaps = sequenciaDeGaps.size();
    do{
        i++;
    }while(i < tamanhogaps);
    i = i - 2;
    h = sequenciaDeGaps[i];
    while(h >= 1){
        for(j = 0; j < h; j++){
            for(k = j + h; k < n; k = k + h){
                buffer_anime = dados_entrada_anime[k];
                l = k - h;
                while((l >= 0) && (dados_entrada_anime[l].ranked < buffer_anime.ranked)){
                    dados_entrada_anime[l + h] = dados_entrada_anime[l];
                    l = l - h;
                }
                dados_entrada_anime[l + h] = buffer_anime;
            }
        }
        if(i != 0){
            i--;
            h = sequenciaDeGaps[i];
        }
        else{
            h = 0;
        }
    }

    // Escrita no arquivo binario
    BPTree bpt_anime(GRAU);
    std::ofstream bin_anime;
    bin_anime.open("anime.bin", std::ios::binary);
    for(unsigned int i = 0; i < dados_entrada_anime.size(); i++){
        bpt_anime.insereBPTree(dados_entrada_anime[i].id, i);
        bin_anime.write(reinterpret_cast<char *>(&(dados_entrada_anime[i])), sizeof(dados_entrada_anime[i]));
    }
    bin_anime.close();
    char bpt_anime_arq[] = "bpt_anime.bin", writeb[] = "wb";
    FILE *bin_bpt_anime = NULL;
    AbreArquivo(&bin_bpt_anime, bpt_anime_arq, writeb);
    bpt_anime.armazenaBPTree(bpt_anime.getRaiz(), bin_bpt_anime);
    fclose(bin_bpt_anime);

    std::cout << std::endl << "BOTTOM 3 ANIMES :" << std::endl;
    dados_entrada_anime[0].printaAnime();
    dados_entrada_anime[1].printaAnime();
    dados_entrada_anime[2].printaAnime();

    // Update da TRIE
    NodoTrie* raiz_trie = cria_NodoTrie('\0');
    FILE *arq_trie_leitura = NULL;
    char nome_trie_bin[] = "trie_anime.bin", readbinary[] = "rb";
    AbreArquivo(&arq_trie_leitura, nome_trie_bin, readbinary);
    raiz_trie = recuperaTRIE(raiz_trie, arq_trie_leitura);
    fclose(arq_trie_leitura);
    for (unsigned int i = 0; i < dados_entrada_anime.size(); i++)
    {
        raiz_trie = atualiza_ids(raiz_trie, dados_entrada_anime[i].name, i);
    }
    FILE *arq_trie_escrita = NULL;
    AbreArquivo(&arq_trie_escrita, nome_trie_bin, writeb);
    armazenaTRIE(raiz_trie, arq_trie_escrita);
    fclose(arq_trie_escrita);
    free_NodoTrie(raiz_trie);

    // Update dos arquivos invertidos
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

void deletaAnime(int id){
    std::vector<Anime> dados_entrada_anime;
    Anime buffer_anime;
    BPTree bpt_anime(GRAU);
    std::ifstream arq_entrada;
    unsigned int i = 0;
    int ctrl_delete = 0;
    arq_entrada.open("anime.bin", std::ios::binary);
    arq_entrada.read((char *) &buffer_anime, sizeof(Anime));
    do{
        if(buffer_anime.id != id){
            dados_entrada_anime.push_back(buffer_anime);
            bpt_anime.insereBPTree(buffer_anime.id, i);
            i++;
        }
        else{
            std::cout << "O anime " << buffer_anime.name << " foi deletado!" << std::endl << std::endl;
            ctrl_delete = 1;
        }
        arq_entrada.read((char *) &buffer_anime, sizeof(Anime));
    }while(!(arq_entrada.eof()));
    arq_entrada.close();
    std::ofstream bin_anime;
    bin_anime.open("anime.bin", std::ios::binary);
    for(i = 0; i < dados_entrada_anime.size(); i++){
        bin_anime.write(reinterpret_cast<char *>(&(dados_entrada_anime[i])), sizeof(dados_entrada_anime[i]));
    }
    bin_anime.close();
    FILE *bpt = nullptr;
    char bpt_anime_arq[] = "bpt_anime.bin", writeb[] = "wb";
    AbreArquivo(&bpt, bpt_anime_arq, writeb);
    bpt_anime.armazenaBPTree(bpt_anime.getRaiz(), bpt);
    fclose(bpt);

    NodoTrie* raiztrie_anime = cria_NodoTrie('\0');
    for (unsigned int i = 0; i < dados_entrada_anime.size(); i++)
    {
        char* nome = (char*) calloc (strlen(dados_entrada_anime[i].name) + 1, sizeof(char));
        for (unsigned int j = 0; j < strlen(dados_entrada_anime[i].name); j++)
        {
            nome[j] = dados_entrada_anime[i].name[j];
        }
        raiztrie_anime = insert_trie(raiztrie_anime, nome, i);
        free(nome);
    }

    // Update da TRIE
    char bin_trie_arq[] = "trie_anime.bin";
    FILE *bin_trie = NULL;
    AbreArquivo(&bin_trie, bin_trie_arq, writeb);
    armazenaTRIE(raiztrie_anime, bin_trie);
    fclose(bin_trie);
    free_NodoTrie(raiztrie_anime);

    // Update dos arquivos invertidos
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
    if(ctrl_delete == 0){
        std::cout << "ID nao encontrado" << std::endl << std::endl;
    }
}

// Geração de sequência de gaps para utilização na ordenação shellSort via CIURA
std::vector<int> geraSequenciaDeGaps(int tam){
    std::vector<int> v;
    int e = 1;
    v.push_back(e);
    int seq[8] = {1, 4, 10, 23, 57, 132, 301, 701};
    int i = 1;
    while((i < 8) && (seq[i] < tam)){
        v.push_back(seq[i]);
        i++;
    }
    e = 701;
    while((e = floor(2.25*(float)e)) < tam){
        v.push_back(e);
    }
    return v;
}

trie_string* cria_arq_inv(trie_string* raiz, int tipo)
{
    // Funcao responsavel por colocar alguns dados do registro Anime
    // na forma de arquivo invertido para facilitar sua busca

    // Abertura do vetor de registros
    FILE *arq_anime = NULL;
    Anime buffer_anime;
    std::vector<Anime> dados_entrada_anime;
    char nome_anime_bin[] = "anime.bin", readingbinary[] = "rb";
    AbreArquivo(&arq_anime, nome_anime_bin, readingbinary);
    fread(&buffer_anime, sizeof(buffer_anime), 1, arq_anime);
    do{
        dados_entrada_anime.push_back(buffer_anime);
        fread(&buffer_anime, sizeof(buffer_anime), 1, arq_anime);
    }while(!feof(arq_anime));
    fclose(arq_anime);

    if (tipo == GENRES)
    {   // Se escolhido genres, o arquivo invertido sera de generos
        for (unsigned int i = 0; i < dados_entrada_anime.size(); i++)
        {
            raiz = insert_trie_string(raiz, dados_entrada_anime[i].genres, i);
        }
    }
    else if (tipo == LICENSORS)
    {   // Se escolhido licensors, o arquivo invertido sera de licenciadores
        for (unsigned int i = 0; i < dados_entrada_anime.size(); i++)
        {
            raiz = insert_trie_string(raiz, dados_entrada_anime[i].licensors, i);
        }
    }
    else if (tipo == STUDIOS)
    {   // Se escolhido studios, o arquivo invertido sera de studios
        for (unsigned int i = 0; i < dados_entrada_anime.size(); i++)
        {
            raiz = insert_trie_string(raiz, dados_entrada_anime[i].studios, i);
        }
    }

    return raiz;
}

void Busca_Um_Campo(char* nome, int tipo){
    std::vector<Anime> dados_entrada_anime;
    Anime buffer_anime;
    std::ifstream arq_entrada;
    char readbinary[] = "rb", nome_genres[] = "genres.bin", nome_studios[] = "studios.bin";
    arq_entrada.open("anime.bin", std::ios::binary);
    arq_entrada.read((char *) &buffer_anime, sizeof(Anime));
    do{
        dados_entrada_anime.push_back(buffer_anime);
        arq_entrada.read((char *) &buffer_anime, sizeof(Anime));
    }while(!(arq_entrada.eof()));
    arq_entrada.close();

    // Leitura de trie_string
    trie_string* raiz = cria_trie_string('\0');
    if (tipo == GENRES)
    {
        FILE* arq_genres = NULL;
        AbreArquivo(&arq_genres, nome_genres, readbinary);
        raiz = recuperaTRIESTRING(raiz, arq_genres);
        fclose(arq_genres);
    } else if (tipo == STUDIOS)
    {
        FILE* arq_studios = NULL;
        AbreArquivo(&arq_studios, nome_studios, readbinary);
        raiz = recuperaTRIESTRING(raiz, arq_studios);
        fclose(arq_studios);
    }


    std::vector<int> ids;
    ids = busca_trie_string(raiz, nome);
    for (unsigned int i = 0; i < ids.size(); i++)
    {
        dados_entrada_anime[ids[i]].printaAnime();
    }
    free_trie_string(raiz);
}

void Busca_Dois_Campos(char* nome1, char* nome2)
{
    // Funcao que proporciona a busca por um determinado registro Anime
    // por dois campos diferentes, ou seja, busca o anime que tem nome1
    // como propriedade 1 e nome2 como propriedade 2

    // Declaracao de variaveis e vetores
    FILE* arq_anime = NULL;
    Anime buffer_anime;
    std::vector<Anime> dados_entrada_anime;
    std::vector<int> ids1 = {};
    std::vector<int> ids2 = {};
    std::vector<int> ids_conjunto = {};
    char nome_anime_bin[] = "anime.bin", readingbinary[] = "rb", nome_genres[] = "genres.bin", nome_licensors[] = "licensors.bin";

    // Gera as 2 estruturas trie_string
    trie_string* raiz1 = cria_trie_string('\0');
    FILE* arq_genres = NULL;
    AbreArquivo(&arq_genres, nome_genres, readingbinary);
    raiz1 = recuperaTRIESTRING(raiz1, arq_genres);
    fclose(arq_genres);

    trie_string* raiz2 = cria_trie_string('\0');
    FILE* arq_licensors = NULL;
    AbreArquivo(&arq_licensors, nome_licensors, readingbinary);
    raiz2 = recuperaTRIESTRING(raiz2, arq_licensors);
    fclose(arq_licensors);

    // Obtem o vetor de indices que contem nome1 e nome2 nas TRIEs
    ids1 = busca_trie_string(raiz1, nome1);
    ids2 = busca_trie_string(raiz2, nome2);
    AbreArquivo(&arq_anime, nome_anime_bin, readingbinary);

    // Carrega o vetor de registro Anime
    fread(&buffer_anime, sizeof(buffer_anime), 1, arq_anime);
    do{
        dados_entrada_anime.push_back(buffer_anime);
        fread(&buffer_anime, sizeof(buffer_anime), 1, arq_anime);
    }while(!feof(arq_anime));
    fclose(arq_anime);

    // Realiza uma interseccao dos valores encontrados
    for (unsigned int i = 0; i < ids1.size(); i++)
    {
        for (unsigned int j = 0; j < ids2.size(); j++)
        {
            if (ids1[i] == ids2[j])
            {
                ids_conjunto.push_back(ids1[i]);
            }
        }
    }

    // Imprime os animes que possuem ambas as propriedades
    for (unsigned int i = 0; i < ids_conjunto.size(); i++)
    {
        dados_entrada_anime[ids_conjunto[i]].printaAnime();
    }
    free_trie_string(raiz1);
    free_trie_string(raiz2);
}

void Busca_Dois_Mesmo_Campo(char* nome1, char* nome2)
{
    // Funcao que proporciona a busca simultanea de dois itens do mesmo campo,
    // ou seja, encontra um determinado registro Anime que possua ou nome1 como
    // propriedade ou nome2 como propriedade de um certo campo

    // Declaracao das variaveis e vetores
    FILE* arq_anime = NULL;
    Anime buffer_anime;
    std::vector<Anime> dados_entrada_anime;
    std::vector<int> ids1 = {};
    std::vector<int> ids2 = {};
    std::vector<int> ids_conjunto = {};
    char nome_anime_bin[] = "anime.bin", readingbinary[] = "rb", nome_studios[] = "studios.bin";

    // Le o arq trie_string de studios
    trie_string* raiz1 = cria_trie_string('\0');
    FILE* arq_studios = NULL;
    AbreArquivo(&arq_studios, nome_studios, readingbinary);
    raiz1 = recuperaTRIESTRING(raiz1, arq_studios);
    fclose(arq_studios);

    ids1 = busca_trie_string(raiz1, nome1);
    ids2 = busca_trie_string(raiz1, nome2);
    AbreArquivo(&arq_anime, nome_anime_bin, readingbinary);

    // Carrega do arquivo binario o vetor de registros Anime
    fread(&buffer_anime, sizeof(buffer_anime), 1, arq_anime);
    do{
        dados_entrada_anime.push_back(buffer_anime);
        fread(&buffer_anime, sizeof(buffer_anime), 1, arq_anime);
    }while(!feof(arq_anime));
    fclose(arq_anime);

    // Joga no vetor em comum todos os dados
    for (unsigned int i = 0; i < ids1.size(); i++)
    {
        ids_conjunto.push_back(ids1[i]);
    }

    for (unsigned int i = 0; i < ids2.size(); i++)
    {
        ids_conjunto.push_back(ids2[i]);
    }

    unsigned int tam = ids_conjunto.size();
    // Deleta todos os valores repetidos que forem encontrados, resultando,
    // assim, numa uniao entre os dois vetores de indices
    for (unsigned int i = 0; i < tam; i++)
    {
        for (unsigned j = i + 1; j < tam; j++)
        {
            if (ids_conjunto[i] == ids_conjunto[j])
            {
                for (unsigned k = j; k < tam - 1; k++)
                {
                    ids_conjunto[k] = ids_conjunto[k+1];
                }
                tam--;
                j--;
            }
        }
    }

    // Imprime os registros Anime encontrados
    for (unsigned int i = 0; i < ids_conjunto.size(); i++)
    {
        dados_entrada_anime[ids_conjunto[i]].printaAnime();
    }
    free_trie_string(raiz1);
}


void recomendaAnime(int id){
    // Leitura do arquivo
    std::vector<Anime> dados_entrada_anime;
    Anime buffer_anime_leitura;
    std::ifstream arq_entrada;
    char nome_genres[] = "genres.bin", nomes_studios[] = "studios.bin", readbinary[] = "rb";
    arq_entrada.open("anime.bin", std::ios::binary);
    arq_entrada.read((char *) &buffer_anime_leitura, sizeof(Anime));
    do{
        dados_entrada_anime.push_back(buffer_anime_leitura);
        arq_entrada.read((char *) &buffer_anime_leitura, sizeof(Anime));
    }while(!(arq_entrada.eof()));
    arq_entrada.close();

    // Le o arquivo binario das trie_string e gera suas estruturas
    trie_string* raiz_gen = cria_trie_string('\0');
    trie_string* raiz_stu = cria_trie_string('\0');
    FILE *arq_gen = NULL, *arq_stu = NULL;
    AbreArquivo(&arq_gen, nome_genres, readbinary);
    raiz_gen = recuperaTRIESTRING(raiz_gen, arq_gen);
    fclose(arq_gen);
    AbreArquivo(&arq_stu, nomes_studios, readbinary);
    raiz_stu = recuperaTRIESTRING(raiz_stu, arq_stu);
    fclose(arq_stu);

    BPTree buffer_bpt(GRAU);
    int buffer_in, buffer_ch;
    std::ifstream arq_bpt;
    // Le o arquivo binario das bpt e gera sua estrutura
    arq_bpt.open("bpt_anime.bin", std::ios::binary);
    do{
        arq_bpt.read((char *) &buffer_ch, sizeof(int));
        arq_bpt.read((char *) &buffer_in, sizeof(int));
        buffer_bpt.insereBPTree(buffer_ch, buffer_in);
    }while(!(arq_bpt.eof()));
    // Procura o id na bpt
    buffer_in = buffer_bpt.procuraBPTree(id);
    if(buffer_in == -1){
        std::cout << "ID invalido" << std::endl << std::endl;
        return;
    }
    arq_bpt.close();

    std::vector<std::pair<int, int>> vetor_proximidade;
    std::pair<int, int> par_buffer;
    unsigned int i;
    char string_gen[200], string_stu[200], *string_buffer;
    buffer_anime_leitura = dados_entrada_anime[buffer_in];
    strcpy(string_gen, buffer_anime_leitura.genres);
    strcpy(string_stu, buffer_anime_leitura.studios);
    std::vector<int> ids_gen = {}, ids_stu  = {};
    int contador_aux = 0;
    // Procura registros de genero igual
    string_buffer = strtok(string_gen, ";.,");
    while(string_buffer != NULL){
        ids_gen = busca_trie_string(raiz_gen, string_buffer);
        for(unsigned int j = 0; j < ids_gen.size(); j++){
            par_buffer.first = ids_gen[j];
            par_buffer.second = 5;
            for(i = 0; i < vetor_proximidade.size(); i++){
                if(vetor_proximidade[i].first == par_buffer.first){
                    vetor_proximidade[i].second += par_buffer.second;
                    break;
                }
            }
            if(i == vetor_proximidade.size()){
                vetor_proximidade.push_back(par_buffer);
            }
        }
        ids_gen.erase(ids_gen.begin(), ids_gen.end());
        string_buffer = strtok(NULL, ";.,");
        contador_aux++;
    }

    // Procura registros de estudio igual
    string_buffer = strtok(string_stu, ";.,");
    while(string_buffer != NULL){
        ids_stu = busca_trie_string(raiz_stu, string_buffer);
        for(unsigned int j = 0; j < ids_stu.size(); j++){
            par_buffer.first = ids_stu[j];
            par_buffer.second = 6;
            for(i = 0; i < vetor_proximidade.size(); i++){
                if(vetor_proximidade[i].first == par_buffer.first){
                    vetor_proximidade[i].second += par_buffer.second;
                    break;
                }
            }
            if(i == vetor_proximidade.size()){
                vetor_proximidade.push_back(par_buffer);
            }
        }
        ids_stu.erase(ids_stu.begin(), ids_stu.end());
        string_buffer = strtok(NULL, ";.,");
        contador_aux++;
    }

    // Seleciona o similar de maior nota
    Anime buffer_anime;
    int proximidade_buffer = 0;
    buffer_anime.score = 0.0;
    strcpy(string_stu, buffer_anime_leitura.name);
    char *string_buffer2 = strtok(string_stu, " :,./");
    for(i = 0; i < vetor_proximidade.size(); i++){
        if(vetor_proximidade[i].second >= (contador_aux * 3)){ // Se é similar
            if(vetor_proximidade[i].first != buffer_in){ // Se não é o mesmo registro
                if((dados_entrada_anime[vetor_proximidade[i].first].score * vetor_proximidade[i].second) >= (buffer_anime.score * proximidade_buffer)){ // Pega o com maior similaridade*nota
                    strcpy(string_gen, dados_entrada_anime[vetor_proximidade[i].first].name);
                    string_buffer = strtok(string_gen, " :,./");
                    if(strcmp(string_buffer, string_buffer2) != 0){ // Se não tiver mesma primeira palavra (evitar pegar novas temporadas do mesmo registro)
                        buffer_anime = dados_entrada_anime[vetor_proximidade[i].first];
                        proximidade_buffer = vetor_proximidade[i].second;
                    }
                }
            }
        }
    }
    std::cout << "Se voce gosta de: " << buffer_anime_leitura.name << std::endl << "Voce tambem vai gostar de: " << buffer_anime.name << "! (ID = " << buffer_anime.id << ")" << std::endl << std::endl;

    free_trie_string(raiz_gen);
    free_trie_string(raiz_stu);
}
