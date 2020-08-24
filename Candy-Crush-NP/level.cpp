#include <iostream>
#include "level.h"
using namespace std;

Level::Level()        // create and fill grid --> playing field or level
{
    context = zmq_ctx_new();
    pusher = zmq_socket(context, ZMQ_PUSH);
    sub = zmq_socket(context, ZMQ_SUB);
    //zmq_connect( pusher, "tcp://benternet.pxl-ea-ict.be:24041" );
    //zmq_connect(sub, "tcp://benternet.pxl-ea-ict.be:24042");
    zmq_connect(pusher, "tcp://localhost:24041");
    zmq_connect( sub, "tcp://localhost:24042");
}

Level::~Level()
{
    zmq_close(pusher);
    zmq_close(sub);
    zmq_ctx_shutdown( context );
    zmq_ctx_term( context );
}

void Level::MakeLevel(int row, int colum)
{
    r=row;
    c=colum+2;
    srand((unsigned)time(nullptr));
    for(int x=0;x<r;x++)
    {
        for(int y=0;y<c;y++)
        {
            if(y==0 || y==(c-1))
            {
                Field* wall = new Wall;      // wall's on the side for default level
                grid[x][y] = wall->setsign();
            }
            else
            {
                Field* candy_field = new Candy_Field;     // fill the rest with candy
                grid[x][y] = candy_field->setsign();
            }
        }
    }
}


void Level::MakeString()
{
    char buffer[256];
    char buff[c];
    char grid_ask[]="Candy>grid?>";
    char tag[13]="Candy>grid!>";
    char message[13+c];

    zmq_setsockopt(sub, ZMQ_SUBSCRIBE, grid_ask, strlen(grid_ask));
    zmq_recv(sub, buffer, 256, 0);

    for(int x=0;x<r;x++)
    {
        message[0]='\0';
        for(int y=0;y<c;y++)
        {
            buff[y]=grid[x][y];
        }
        strcat(message, tag);
        strcat(message,buff);
        message[13+c]='\0';
        zmq_send(pusher, message, strlen(message), 0);
        //cout << message << endl;
    }
}

void Level::printGrid()     // print grid
{
    cout << "  ";
    for(int i=0;i<c-2;i++)
    {
        cout << i;
    }
    cout << endl;

    for(int x=0;x<r;x++)
    {
        cout << x;
        for(int y=0;y<c;y++)
        {
            cout << grid[x][y];         //loop trough grid from [0][0] to [r][c] --> r=rows, c=colums
        }
        cout << endl;
    }
    cout << endl;
}

void Level::Move()      // make a move on the field
{
    char buffer[256];
    char colum_ask[]="Candy>colum?>";
    char get_colum[]="Candy>colum!>";
    char row_ask[]="Candy>row?>";
    char get_row[]="Candy>row!>";
    char ask_move[]="Candy>move?>";
    char get_move[]="Candy>move!>";
    char legal[]="Candy>legal!>";

    int a1;
    int b1;
    int a2;
    int b2;
    char move;          // choose candy for move
    zmq_send(pusher, colum_ask, strlen(colum_ask), 0);
    zmq_setsockopt(sub, ZMQ_SUBSCRIBE, get_colum, strlen(get_colum));
    zmq_recv(sub, buffer, 256, 0);
    b1=buffer[13]-'0';
    b1++;
    buffer[0]='\0';

    zmq_send(pusher, row_ask, strlen(row_ask), 0);
    zmq_setsockopt(sub, ZMQ_SUBSCRIBE, get_row, strlen(get_row));
    zmq_recv(sub, buffer, 256, 0);
    a1=buffer[11]-'0';
    buffer[0]='\0';

    //cout << b1 << "   " << a1 << endl;

    /*cout << "Up=U, Down=D, Left=L, Right=R\n" << "Choice: ";        // choose wich move --> up is switch candy witch candy above ....
    cin >> move;
    cout << endl;*/

    zmq_send(pusher, ask_move, strlen(ask_move), 0);
    zmq_setsockopt(sub, ZMQ_SUBSCRIBE, get_move, strlen(get_move));
    zmq_recv(sub, buffer, 256, 0);
    move=buffer[12];
    buffer[0]='\0';
    cout << buffer << endl << move << endl;

    if(Check_Move(b1,a1,move)==3)  // Up        // if move is legal position wise switch the candy's
    {
        a2=a1-1;
        b2=b1;
    }
    else if(Check_Move(b1,a1,move)==4) //Down
    {
        a2=a1+1;
        b2=b1;
    }
    else if(Check_Move(b1,a1,move)==1) //Left
    {
        a2=a1;
        b2=b1-1;

    }
    else if(Check_Move(b1,a1,move)==2) //Right
    {
        a2=a1;
        b2=b1+1;
    }
    else
    {
        a2=a1;
        b2=b1;
        legal[13]='1';
        zmq_send(pusher, legal,strlen(legal), 0);
        //cout << endl << "Illegal move --> no combo" << endl << endl;
        //printGrid();
        MakeString();
        Move();
    }

    grid[a1][b1]^=grid[a2][b2];
    grid[a2][b2]^=grid[a1][b1];
    grid[a1][b1]^=grid[a2][b2];


    if(Check_Break_Move(a2,b2))         // if move is illegal combo wise switch back and try again
    {
        grid[a1][b1]^=grid[a2][b2];
        grid[a2][b2]^=grid[a1][b1];
        grid[a1][b1]^=grid[a2][b2];

        legal[13]='1';
        zmq_send(pusher, legal,strlen(legal), 0);
        //cout << endl << "Illegal move --> no combo" << endl << endl;
        //printGrid();
        MakeString();
        Move();
    }
    else
    {
        legal[13]='0';
        zmq_send(pusher, legal,strlen(legal), 0);
        //printGrid();
        MakeString();
    }
}

int Level::Check_Move(int x, int y, char moves)      // check position of chosen candy and look for wall's or level boundarys
{                                                       // if wall or boundary move is illegal because switch not possible
    if(moves==Left && x!=1)
    {
        return 1;   //left side --> left move enable
    }
    if(moves==Right && x!=c-2)
    {
        return 2;   //right side --> right move enable
    }
    if(moves==Up && y!=0)
    {
        return 3;   //top side --> up move enable
    }
    if(moves==Down && y!=r-1)
    {
        return 4;   //bottom side --> down move enable
    }
    else
    {
        return 0;
    }
}

bool Level::Check_Break_Move(int x, int y)              // check if the move made a new combo --> if not switch back and ask for new move
{
    if(grid[x][y]==grid[x+1][y] && grid[x][y]==grid[x-1][y])
    {
        return false;
    }
    else if(grid[x][y]==grid[x-1][y] && grid[x][y]==grid[x-2][y])
    {
        return false;

    }
    else if(grid[x][y]==grid[x+1][y] && grid[x][y]==grid[x+2][y])
    {
        return false;

    }
    else if(grid[x][y]==grid[x][y+1] && grid[x][y]==grid[x][y-1])
    {
        return false;
    }
    else if(grid[x][y]==grid[x][y-1] && grid[x][y]==grid[x][y-2])
    {
        return false;
    }
    else if(grid[x][y]==grid[x][y+1] && grid[x][y]==grid[x][y+2])
    {
        return false;
    }
    else
    {
        return true;
    }
}



void Level::Find_Combo()                            // check the field for combo's and turn into new candy's
{
    Candy_Field * new1 = new Candy_Field;
    Candy_Field * new2 = new Candy_Field;
    Candy_Field * new3 = new Candy_Field;
    for(int x=0;x<r;x++)
    {
        for(int y=1;y<c-1;y++)              // loop trough the grid/field and check in all directions
        {
            if(grid[x][y]==grid[x+1][y] && grid[x][y]==grid[x-1][y])
            {
                grid[x][y]=new1->setsign();
                grid[x+1][y]=new2->setsign();           // fill field with new candy
                grid[x-1][y]=new3->setsign();
                score++;                                // add score because combo give's you points
            }
            else if(grid[x][y]==grid[x-1][y] && grid[x][y]==grid[x-2][y])
            {
                grid[x][y]=new1->setsign();
                grid[x-1][y]=new2->setsign();
                grid[x-2][y]=new3->setsign();
                score++;
            }
            else if(grid[x][y]==grid[x+1][y] && grid[x][y]==grid[x+2][y])
            {
                grid[x][y]=new1->setsign();
                grid[x+1][y]=new2->setsign();
                grid[x+2][y]=new3->setsign();
                score++;
            }
            else if(grid[x][y]==grid[x][y+1] && grid[x][y]==grid[x][y-1])
            {
                grid[x][y]=new1->setsign();
                grid[x][y+1]=new2->setsign();
                grid[x][y-1]=new3->setsign();
                score++;
            }
            else if(grid[x][y]==grid[x][y-1] && grid[x][y]==grid[x][y-2])
            {
                grid[x][y]=new1->setsign();
                grid[x][y-1]=new2->setsign();
                grid[x][y-2]=new3->setsign();
                score++;
            }
            else if(grid[x][y]==grid[x][y+1] && grid[x][y]==grid[x][y+2])
            {
                grid[x][y]=new1->setsign();
                grid[x][y+1]=new2->setsign();
                grid[x][y+2]=new3->setsign();
                score++;
            }
        }
    }
}
