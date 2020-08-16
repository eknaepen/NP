#ifndef RANDOM_CANDY_H
#define RANDOM_CANDY_H

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "candy.h"

class Random_candy
{
public:
    unsigned char color;
    Random_candy() {candy = new Candy(Candy::COLOR(rand()%5));      //create a new Candy with random color
                    color=candy->print();}
    ~Random_candy() {delete candy;}
private:
    Candy * candy;
};

#endif // RANDOM_CANDY_H
