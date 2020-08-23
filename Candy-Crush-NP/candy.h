#ifndef CANDY_H
#define CANDY_H
#include <stdio.h>
#include <string.h>
#include <iostream>

class Candy
{
public:
    enum COLOR  // to asign color
    {
        RED,      //1 for randomizer
        BLUE,     //2
        GREEN,    //3
        ORANGE,   //4
        YELLOW    //5
    };

    Candy(COLOR color)  : color(color){}    //create candy
    ~Candy() {}

    COLOR getColor() const  // get color for combo checker
    {
        return color;
    }

    unsigned char print()    // print color
    {
        unsigned char color_sign;
        switch(color)
        {
        case RED:
             color_sign='R';
            break;
        case BLUE:
            color_sign='B';
            break;
        case GREEN:
            color_sign='G';
            break;
        case YELLOW:
            color_sign='Y';
            break;
        case ORANGE:
            color_sign='O';
            break;
        default:
            color_sign=' ';
            break;
        }
        return color_sign;
    }
private:
    const COLOR color;
};

#endif // CANDY_H
