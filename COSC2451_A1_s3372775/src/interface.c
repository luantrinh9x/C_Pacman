#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "string.h"
#include "interface.h"

char * f_names[] = {"q","Q","w","x"," ","e","E","a","d","s","S","z","Z","c","C","g","G","p","P","f","F","W","A","D","X"};

char (*funcs[])(char) = {qQ,qQ,wx,wx,space,eE,eE,ad,ad,s,S,zZ,zZ,cC,cC,gG,gG,pP,pP,fF,fF,W,A,D,X};

char qQ(char x)
{
    attrset(COLOR_PAIR(0));
    return addch(ACS_ULCORNER);
}

char wx(char x)
{
    attrset(COLOR_PAIR(0));
    return addch(ACS_HLINE);
}

char space(char x)
{
    attrset(COLOR_PAIR(0));
    return addch(' ');
}

char eE(char x)
{
    attrset(COLOR_PAIR(0));
    return addch(ACS_URCORNER);
}

char ad(char x)
{
    attrset(COLOR_PAIR(0));
    return addch(ACS_VLINE);
}

char s(char x)
{
    attrset(COLOR_PAIR(0));
    return addch(ACS_BULLET);
}

char S(char x)
{
    attrset(COLOR_PAIR(0));
    return addch(ACS_DEGREE);
}

char zZ(char x)
{
    attrset(COLOR_PAIR(0));
    return addch(ACS_LLCORNER);
}

char cC(char x)
{
    attrset(COLOR_PAIR(0));
    return addch(ACS_LRCORNER);
}

char gG(char x)
{
    attrset(COLOR_PAIR(2));
    return addch('G');
}

char pP(char x)
{
    attrset(COLOR_PAIR(2));
    return addch('P');
}

char fF(char x)
{
    attrset(COLOR_PAIR(2));
    return addch('F');
}

char W(char x)
{   
    attrset(COLOR_PAIR(0));
    return addch(ACS_TTEE);
}

char A(char x)
{
    attrset(COLOR_PAIR(0));
    return addch(ACS_LTEE);
}

char D(char x)
{
    attrset(COLOR_PAIR(0));
    return addch(ACS_RTEE);
}

char X(char x)
{
    attrset(COLOR_PAIR(0));
    return addch(ACS_BTEE);
}

void addKey(char (*funcs)(char), char * x)
{
    funcs(*x);
}

void getKey(char * key)
{
    for(int i = 0; i < sizeof(funcs)/sizeof(*funcs); i++)
    {
        if(strncmp(f_names[i],key,1) == 0)
        {       
            addKey(funcs[i],key);  
        }
    }
}
