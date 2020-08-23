#ifndef CANDY_FIELD_H
#define CANDY_FIELD_H
#include "random_candy.h"
#include "field.h"


class Candy_Field : public Field
{
public:
    Candy_Field() : rand(new Random_candy){}    // get a new random_candy
    virtual ~Candy_Field(){delete rand;}

    unsigned char setsign(){    // asign the sign to fill grid
        sign=rand->color;
        return sign;
    }
private:
    const Random_candy * rand;
};

#endif // CANDY_FIELD_H
