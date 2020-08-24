#include <iostream>
#include <zmq.h>
#include "game.h"
using namespace std;

typedef enum{StartUp, Play, Reset, End}GameMode; // set-up for state machine
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
            case Reset:
                cout << "play again press 1: ";
                cin >> reset;       // reset yes=1
                if(reset==1)
                {
                    NextState=Play;
                }
                else
                {
                    NextState=End;  // shutdown program
                }
                break;
            case End:
                loop=1;
                break;
        }
    }
    return 0;
}
