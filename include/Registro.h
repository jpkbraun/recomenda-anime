#ifndef REGISTRO_H
    #define REGISTR0_H

#include <string>
#include <vector>
#include <ostream>

// Registro de anime
class Anime{
    public:

        Anime(std::string linha);
        Anime();
        ~Anime();

        void printaAnime();

        int id;
        char name[200];
        float score;
        char genres[200];
        char english_name[200];
        char type[200];
        int episodes;
        char aired[200];
        char premiered[200];
        char producers[200];
        char licensors[200];
        char studios[200];
        char source[200];
        char duration[200];
        char rating[200];
        int ranked;
        int popularity;
        int members;
        int favorites;
        int watching;
        int completed;
        int on_hold;
        int dropped;
        int plan_to_watch;
        int score10;
        int score9;
        int score8;
        int score7;
        int score6;
        int score5;
        int score4;
        int score3;
        int score2;
        int score1;

    private:
};

#endif // REGISTRO_H
