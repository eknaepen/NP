#include <iostream>
#include "game.h"
using namespace std;

Game::Game()
{
    level = new Level();

    //cout << level->reset << endl;

    //zmq_setsockopt(level->sub, ZMQ_SUBSCRIBE, start_ask, strlen(start_ask));

    if(level->reset)
    {
        level->reset=0;
    }
    else
    {
        zmq_recv(level->sub, buf, 13, 0);
        buf[13]='\0';
        if(strcmp(buf,start_ask)==0)
        {
            zmq_send(level->pusher, start_ans, strlen(start_ans), 0);
            buf[0]='\0';
        }
    }

    //zmq_setsockopt(level->sub , ZMQ_SUBSCRIBE, get_x, strlen(get_x));

    zmq_recv(level->sub, buf, 10, 0);
    if(buf[7]=='s' && buf[8]=='t')
    {
        level->reset=1;
        zmq_send(level->pusher, start_ans, strlen(start_ans), 0);
        buf[0]='\0';
    }
    else if(buf[7]=='x' && (buf[9]=='5'||'6'||'7'||'8'||'9') && level->reset==0)
    {
        x=buf[9]-'0';
        buf[0]='\0';
    }

    //zmq_setsockopt(level->sub , ZMQ_SUBSCRIBE, get_y, strlen(get_y));

    zmq_recv(level->sub, buf, 10, 0);
    if(buf[7]=='s' && buf[8]=='t')
    {
        level->reset=1;
        zmq_send(level->pusher, start_ans, strlen(start_ans), 0);
        buf[0]='\0';
    }
    else if(buf[7]=='y' && (buf[9]=='5'||'6'||'7'||'8'||'9') && level->reset==0)
    {
        y=buf[9]-'0';
        buf[0]='\0';
    }

    //cout << level->reset << endl;

    level->MakeLevel(x,y);
    //level->MakeLevel(5,5);
    level->Find_Combo();
    //level->printGrid();
    level->MakeString();

    for(int i=0;i<limit;i++)        // make sure the turn limit is not crossed
    {
        //zmq_setsockopt(level->sub, ZMQ_SUBSCRIBE, play, strlen(play));

        if(level->reset)
        {
            quit=0;
        }
        else
        {
            zmq_recv(level->sub, buf, 13, 0);
            if(buf[7]=='s' && buf[8]=='t')
            {
                quit=0;
            }
            else
            {
                quit=buf[12]-'0';
                buf[0]='\0';
            }
        }

        if(quit)             // give option to give up if field is stuck
        {
            /*score=level->score;
            get_turn[12]=limit-i+'0';
            get_score[13]=score+'0';
            //cout << score << endl;

            Sleep(1);
            zmq_send(level->pusher, get_turn, strlen(get_turn), 0);
            Sleep(1);
            zmq_send(level->pusher, get_score, strlen(get_score), 0);
            //cout << limit-i << endl << score << endl;
            Sleep(1);*/
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
