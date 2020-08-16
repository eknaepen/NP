#ifndef WALL_H
#define WALL_H
#include <iostream>
#include "field.h"

class Wall : public Field
{
public:
    Wall(){}
    unsigned char setsign(){        // create a wall --> candy can't cross
        sign='|';
        return sign;
    }
};

#endif // WALL_H
