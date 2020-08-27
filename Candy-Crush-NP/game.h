#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "level.h"
using namespace std;

class Game
{
public:
    int score=0;
    int x;              // amount of rows
    int y;              // amount of colums
    int quit;

    char start_ask[14]="Candy!>start>";
    char start_ans[14]="Candy?>start>";
    char get_x[10]="Candy!>x>";
    char get_y[10]="Candy!>y>";
    char play[13]="Candy!>play>";
    char get_score[14]="Candy!>score>";
    char get_turn[13]="Candy!>turn>";
    char buf[13];

    Level * level;
    Game();
    ~Game(){delete level;}
    void Round();
private:
    const int limit=3;  // max turns in game
};

#endif // GAME_H
