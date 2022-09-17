#include <iostream>
#include <sstream>
#include <string>
#include <string.h>
#include "..\include\Registro.h"


// Construtor classe de Animes
Anime::Anime(std::string linha){
    std::vector<std::string> row;
    std::string word;

    // Leitura de linhas, separando atributos
    std::istringstream iss(linha);
    while(getline(iss, word, ',')){
        row.push_back(word);
    }
    // Set de atributos
    id = std::stoi(row[0]);
    strcpy(name, row[1].c_str());
    if(row[2] == "Unknown"){
        score = -1;
    }
    else{
        score = std::stof(row[2]);
    }
    strcpy(genres, row[3].c_str());
    strcpy(english_name, row[4].c_str());
    strcpy(type, row[5].c_str());
    if(row[6] == "Unknown"){
        episodes = -1;
    }
    else{
        episodes = std::stoi(row[6]);
    }
    strcpy(aired, row[7].c_str());
    strcpy(premiered, row[8].c_str());
    strcpy(producers, row[9].c_str());
    strcpy(licensors, row[10].c_str());
    strcpy(studios, row[11].c_str());
    strcpy(source, row[12].c_str());
    strcpy(duration, row[13].c_str());
    strcpy(rating, row[14].c_str());
    if(row[15] == "Unknown"){
        ranked = -1;
    }
    else{
        ranked = std::stoi(row[15]);
    }
    if(row[16] == "Unknown"){
        popularity = -1;
    }
    else{
        popularity = std::stoi(row[16]);
    }
    if(row[17] == "Unknown"){
        members = -1;
    }
    else{
        members = std::stoi(row[17]);
    }
    if(row[18] == "Unknown"){
        favorites = -1;
    }
    else{
        favorites = std::stoi(row[18]);
    }
    if(row[19] == "Unknown"){
        watching = -1;
    }
    else{
        watching = std::stoi(row[19]);
    }
    if(row[20] == "Unknown"){
        completed = -1;
    }
    else{
        completed = std::stoi(row[20]);
    }
    if(row[21] == "Unknown"){
        on_hold = -1;
    }
    else{
        on_hold = std::stoi(row[21]);
    }
    if(row[22] == "Unknown"){
        dropped = -1;
    }
    else{
        dropped = std::stoi(row[22]);
    }
    if(row[23] == "Unknown"){
        plan_to_watch = -1;
    }
    else{
        plan_to_watch = std::stoi(row[23]);
    }
    if(row[24] == "Unknown"){
        score10 = -1;
    }
    else{
        score10 = std::stoi(row[24]);
    }
    if(row[25] == "Unknown"){
        score9 = -1;
    }
    else{
        score9 = std::stoi(row[25]);
    }
    if(row[26] == "Unknown"){
        score8 = -1;
    }
    else{
        score8 = std::stoi(row[26]);
    }
    if(row[27] == "Unknown"){
        score7 = -1;
    }
    else{
        score7 = std::stoi(row[27]);
    }
    if(row[28] == "Unknown"){
        score6 = -1;
    }
    else{
        score6 = std::stoi(row[28]);
    }
    if(row[29] == "Unknown"){
        score5 = -1;
    }
    else{
        score5 = std::stoi(row[29]);
    }
    if(row[30] == "Unknown"){
        score4 = -1;
    }
    else{
        score4 = std::stoi(row[30]);
    }
    if(row[31] == "Unknown"){
        score3 = -1;
    }
    else{
        score3 = std::stoi(row[31]);
    }
    if(row[32] == "Unknown"){
        score2 = -1;
    }
    else{
        score2 = std::stoi(row[32]);
    }
    if(row[33] == "Unknown"){
        score1 = -1;
    }
    else{
        score1 = std::stoi(row[33]);
    }
}

// Construtor para leitura de arquivos
Anime::Anime(){

}

// Deconstrutor de classe Anime
Anime::~Anime(){

}

void Anime::printaAnime()
{
    std::cout << std::endl << "ID: " << this->id << "\n";
    std::cout << "Nome do anime: " << this->name << "\n";
    if(this->score == -1){
        std::cout << "Pontuacao de 1 a 10: " << "Desconhecida" << "\n";
    }
    else{
        std::cout << "Pontuacao de 1 a 10: " << this->score << "\n";
    }
    std::cout << "Generos: " << this->genres << "\n";
    std::cout << "Nome em ingles: " << this->english_name << "\n";
    std::cout << "Tipo: " << this->type << "\n";
    if(this->episodes == -1){
        std::cout << "Numero de episodios: " << "Desconhecido" << "\n";
    }
    else{
        std::cout << "Numero de episodios: " << this->episodes << "\n";
    }
    std::cout << "Quando foi ao ar: " << this->aired << "\n";
    std::cout << "Temporada: " << this->premiered << "\n";
    std::cout << "Produtores: " << this->producers << "\n";
    std::cout << "Licensiado por: " << this->licensors << "\n";
    std::cout << "Studios: " << this->studios << "\n";
    std::cout << "Fonte: " << this->source << "\n";
    std::cout << "Duracao: " << this->duration << "\n";
    std::cout << "Classificacao indicativa: " << this->rating << "\n";
    if(this->ranked == -1){
        std::cout << "Rank: " << "Desconhecido" << "\n";
    }
    else{
        std::cout << "Rank: " << this->ranked << "\n";
    }
    if(this->popularity == -1){
        std::cout << "Popularidade: " << "Desconhecida" << "\n";
    }
    else{
        std::cout << "Popularidade: " << this->popularity << "\n";
    }
    if(this->members == -1){
        std::cout << "Membros: " << "Desconhecido" << "\n";
    }
    else{
        std::cout << "Membros: " << this->members << "\n";
    }
    if(this->favorites == -1){
        std::cout << "Favoritos: " << "Desconhecido" << "\n";
    }
    else{
        std::cout << "Favoritos: " << this->favorites << "\n";
    }
    if(this->watching == -1){
        std::cout << "Assistindo: " << "Desconhecido" << "\n";
    }
    else{
        std::cout << "Assistindo: " << this->watching << "\n";
    }
    if(this->completed == -1){
        std::cout << "Ja assistiram: " << "Desconhecido" << "\n";
    }
    else{
        std::cout << "Ja assistiram: " << this->completed << "\n";
    }
    if(this->on_hold == -1){
        std::cout << "Pausaram: " << "Desconhecido" << "\n";
    }
    else{
        std::cout << "Pausaram: " << this->on_hold << "\n";
    }
    if(this->dropped == -1){
        std::cout << "Desistiram: " << "Desconhecido" << "\n";
    }
    else{
        std::cout << "Desistiram: " << this->dropped << "\n";
    }
    if(this->plan_to_watch == -1){
        std::cout << "Planejam assistir: " << "Desconhecido" << "\n";
    }
    else{
        std::cout << "Planejam assistir: " << this->plan_to_watch << "\n";
    }
    if(this->score10 == -1){
        std::cout << "Notas 10: " << "Desconhecido" << "\n";
    }
    else{
        std::cout << "Notas 10: " << this->score10 << "\n";
    }
    if(this->score9 == -1){
        std::cout << "Notas 9: " << "Desconhecido" << "\n";
    }
    else{
        std::cout << "Notas 9: " << this->score9 << "\n";
    }
    if(this->score8 == -1){
        std::cout << "Notas 8: " << "Desconhecido" << "\n";
    }
    else{
        std::cout << "Notas 8: " << this->score8 << "\n";
    }
    if(this->score7 == -1){
        std::cout << "Notas 7: " << "Desconhecido" << "\n";
    }
    else{
        std::cout << "Notas 7: " << this->score7 << "\n";
    }
    if(this->score6 == -1){
        std::cout << "Notas 6: " << "Desconhecido" << "\n";
    }
    else{
        std::cout << "Notas 6: " << this->score6 << "\n";
    }
    if(this->score5 == -1){
        std::cout << "Notas 5: " << "Desconhecido" << "\n";
    }
    else{
        std::cout << "Notas 5: " << this->score5 << "\n";
    }
    if(this->score4 == -1){
        std::cout << "Notas 4: " << "Desconhecido" << "\n";
    }
    else{
        std::cout << "Notas 4: " << this->score4 << "\n";
    }
    if(this->score3 == -1){
        std::cout << "Notas 3: " << "Desconhecido" << "\n";
    }
    else{
        std::cout << "Notas 3: " << this->score3 << "\n";
    }
    if(this->score2 == -1){
        std::cout << "Notas 2: " << "Desconhecido" << "\n";
    }
    else{
        std::cout << "Notas 2: " << this->score2 << "\n";
    }
    if(this->score1 == -1){
        std::cout << "Notas 1: " << "Desconhecido" << "\n";
    }
    else{
        std::cout << "Notas 1: " << this->score1 << "\n" << std::endl;
    }
}
