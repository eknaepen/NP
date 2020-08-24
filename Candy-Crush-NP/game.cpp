#include <iostream>
#include "game.h"
using namespace std;

Game::Game()
{
    char start_ask[]="Candy>start?>";
    char start_ans[]="Candy>start!>";
    char get_x[]="Candy>x!>";
    char get_y[]="Candy>y!>";
    char play[]="Candy>play!>";
    char get_score[]="Candy>score!>";
    char get_turn[]="Candy>turn!>";

    char buf[13];

    level = new Level();

    zmq_setsockopt(level->sub, ZMQ_SUBSCRIBE, start_ask, strlen(start_ask));

    zmq_recv(level->sub, buf, 13, 0);
    zmq_send(level->pusher, start_ans, strlen(start_ans), 0);

    buf[0]='\0';

    zmq_setsockopt(level->sub , ZMQ_SUBSCRIBE, get_x, strlen(get_x));
    zmq_recv(level->sub, buf, 10, 0);
    x=buf[9]-'0';
    buf[0]='\0';

    zmq_setsockopt(level->sub , ZMQ_SUBSCRIBE, get_y, strlen(get_y));
    zmq_recv(level->sub, buf, 10, 0);
    y=buf[9]-'0';
    buf[0]='\0';

    level->MakeLevel(x,y);
    //level->MakeLevel(5,5);
    level->Find_Combo();
    //level->printGrid();
    level->MakeString();
    for(int i=0;i<limit;i++)        // make sure the turn limit is not crossed
    {
        int quit;
        zmq_setsockopt(level->sub, ZMQ_SUBSCRIBE, play, strlen(play));
        zmq_recv(level->sub, buf, 13, 0);
        quit=buf[12]-'0';
        buf[0]='\0';

        if(quit==1)             // give option to give up if field is stuck
        {
            score=level->score;
            get_turn[12]=limit-i+'0';
            get_score[13]=score+'0';
            //cout << score << endl;

            Sleep(1);
            zmq_send(level->pusher, get_turn, strlen(get_turn), 0);
            Sleep(1);
            zmq_send(level->pusher, get_score, strlen(get_score), 0);
            //cout << limit-i << endl << score << endl;
            Sleep(1);
            Round();
        }
        else
        {
            i=limit;
        }
    }
    //score=level->score*100;
    //cout << "Your score: " << score << "\n";
}

void Game::Round()      // do a move and get the combo of the field
{
    level->Move();
    level->Find_Combo();
    //level->printGrid();
    level->MakeString();
}
