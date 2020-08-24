#include <iostream>
#include <string.h>
#include <time.h>
#include <zmq.h>
#include <header.h>

void * context = zmq_ctx_new();
void * pusher = zmq_socket(context, ZMQ_PUSH);
void * sub = zmq_socket(context, ZMQ_SUB);

void Setup();
void Print_Grid();
void Move();

using namespace std;

int main()
{
    while(loop==0)
    {
        switch(NextState)
        {
            case StartUp:
                Setup();        //setup a game
                NextState=Play;
                break;
            case Play:
                cout << "Give up=0, Play=1  :";
                cin >> quit;
                play[12]=quit;
                zmq_send(pusher, play, strlen(play), 0);
                if(quit=='1')
                {
                    Move();
                    NextState=Play;
                }
                else
                {
                    NextState=Reset;
                }

                break;
            case Reset:
                cout << "play again press 1: ";
                cin >> reset;       // reset yes=1
                if(reset==1)
                {
                    NextState=StartUp;
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

    zmq_close(pusher);
    zmq_close(sub);
    zmq_ctx_shutdown( context );
    zmq_ctx_term( context );

    return 0;
}

void Setup()
{
    cout << "geef aantal rijen(min 5, max 9): ";
    cin >> a;
    cout << "geef aantal kolomen(min 5, max 9): ";
    cin >> b;
    get_x[9]=a;
    get_y[9]=b;

    x=a-'0';
    y=b-'0';

    /*zmq_connect( pusher, "tcp://benternet.pxl-ea-ict.be:24041" );
    zmq_connect(sub, "tcp://benternet.pxl-ea-ict.be:24042");*/
    zmq_connect(pusher, "tcp://localhost:24041");
    zmq_connect( sub, "tcp://localhost:24042");

    zmq_setsockopt(sub, ZMQ_SUBSCRIBE, start_ans, strlen(start_ans));
    zmq_send(pusher, start_ask, strlen(start_ask), 0);
    zmq_recv(sub, buffer, 256, 0);

    zmq_send(pusher, get_x, strlen(get_x), 0);
    Sleep(1);
    zmq_send(pusher, get_y, strlen(get_y), 0);
    Sleep(1);

    Print_Grid();
}

void Print_Grid()
{
    cout << endl;
    zmq_send(pusher, grid_ask, strlen(grid_ask), 0);
    zmq_setsockopt(sub, ZMQ_SUBSCRIBE, grid_ans, strlen(grid_ans));
    cout << "  ";
    for(int g=0;g<y;g++)
    {
        cout << g;
    }
    cout << endl;

    for(int i=0;i<x;i++)
    {
        cout << i;
        zmq_recv(sub, buffer, 256, 0);  // ontvang grid message
        for(int j=0;j<y+2;j++)            //parse grid message zodat enkel grid overblijft
        {
            buffer[j]^=buffer[12+j];
            buffer[12+j]^=buffer[j];
            buffer[j]^=buffer[12+j];
        }
        buffer[y+2]='\0';

        cout << buffer << endl;         // print grid
        buffer[0]='\0';
    }
    cout << endl;
}

void Move()
{
    char move[1];
    int illegal;

    zmq_setsockopt(sub, ZMQ_SUBSCRIBE, get_turn, strlen(get_turn));
    zmq_recv(sub, buffer, 256, 0);
    turn=buffer[12]-'0';
    buffer[0]='\0';

    zmq_setsockopt(sub, ZMQ_SUBSCRIBE, get_score, strlen(get_score));
    zmq_recv(sub, buffer, 256, 0);
    score=buffer[13]-'0';
    score=score*100;

    cout << "Turn's: " << turn << endl << "Your score: " << score << endl;

    zmq_setsockopt(sub, ZMQ_SUBSCRIBE, colum_ask, strlen(colum_ask));
    zmq_recv(sub, buffer, 256, 0);
    cout << "Colum: ";
    cin >> colum;
    //cout << colum << endl;
    get_colum[13]=colum+'0';
    //cout << get_colum << endl;
    zmq_send(pusher, get_colum, strlen(get_colum), 0);

    zmq_setsockopt(sub, ZMQ_SUBSCRIBE, row_ask, strlen(row_ask));
    zmq_recv(sub, buffer, 256, 0);
    cout << "Row: ";
    cin >> row;
    //cout << row << endl;
    get_row[11]=row+'0';
    //cout << get_row << endl;
    zmq_send(pusher, get_row, strlen(get_row), 0);

    zmq_setsockopt(sub, ZMQ_SUBSCRIBE, ask_move, strlen(ask_move));
    zmq_recv(sub, buffer, 256, 0);
    buffer[0]='\0';
    cout << "Up=U, Down=D, Left=L, Right=R\n" << "Choice: ";
    cin >> move;
    cout << endl;
    get_move[12]=move[0];
    zmq_send(pusher, get_move, strlen(get_move), 0);

    zmq_setsockopt(sub, ZMQ_SUBSCRIBE, legal, strlen(legal));
    zmq_recv(sub, buffer, 256, 0);
    illegal=buffer[13]-'0';
    buffer[0]='\0';
    if(illegal)
    {
        cout << endl << "Illegal move --> no combo" << endl << endl;
        Print_Grid();
        Move();
    }
    else
    {
        Print_Grid();
    }
    Print_Grid();
}
