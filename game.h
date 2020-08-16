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

    Level * level;
    Game();
    ~Game(){delete level;}
    void Round();
private:
    const int limit=3;  // max turns in game
};

#endif // GAME_H
