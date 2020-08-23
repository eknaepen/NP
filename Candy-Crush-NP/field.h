#ifndef FIELD_H
#define FIELD_H
#include <stdio.h>
#include <string.h>

class Field
{
public:
    virtual unsigned char setsign()=0;  // virtual fuction --> childeren use it to set "sign"
protected:
    unsigned char sign;     // sign to fill the grid

};

#endif // FIELD_H
