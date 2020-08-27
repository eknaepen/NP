#ifndef HEADER_H
#define HEADER_H

typedef enum{StartUp, Play, Reset, End}GameMode; // set-up for state machine
GameMode NextState=StartUp;

char buffer[256];
char candy_zmq[]="Candy?>";
char start_ask[]="Candy!>start>";
char start_ans[]="Candy?>start>";
char grid_ask[]="Candy!>grid>";
char grid_ans[]="Candy?>grid>";
char get_x[]="Candy!>x>";
char get_y[]="Candy!>y>";
char play[]="Candy!>play>";
char get_score[]="Candy!>score>";
char get_turn[]="Candy!>turn>";
char colum_ask[]="Candy?>colum>";
char get_colum[]="Candy!>colum>";
char row_ask[]="Candy?>row>";
char get_row[]="Candy!>row>";
char ask_move[]="Candy?>move>";
char get_move[]="Candy!>move>";
char legal[]="Candy?>legal>";
char a;
char b;
char quit;

int x;
int y;
int colum;
int row;
int score;
int turn;
int loop=0;
int reset;

#endif // HEADER_H
