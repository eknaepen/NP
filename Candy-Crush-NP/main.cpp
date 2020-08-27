#include <iostream>
#include <zmq.h>
#include "game.h"
using namespace std;

typedef enum{StartUp, Play}GameMode; // set-up for state machine
GameMode NextState=StartUp;

int loop=0;
int reset;

int main()
{
    while(loop==0)
    {
        switch(NextState)
        {
            case StartUp:
                //cout << "Startup" << endl;
                Game * game;        // make a game
                NextState=Play;
                break;
            case Play:
                game = new Game;    // new game
                NextState=StartUp;
                break;
        }
    }
    return 0;
}
