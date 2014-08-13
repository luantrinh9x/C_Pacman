#ifndef INTERFACES
#define INTERFACES

/** declare array with each char is a element */
extern char * f_names[];


char qQ(char x);    /** return upper left corner char */
char wx(char x);    /** return upper horizontal line char */
char space(char x); /** return new space char */
char eE(char x);    /** return upper right corner char */
char ad(char x);    /** return vertical line char */
char s(char x);     /** return pellet char */
char S(char x);     /** return power pellet char */
char zZ(char x);    /** return lower left corner char */
char cC(char x);    /** return lower left corner char */
char gG(char x);    /** return G char */
char pP(char x);    /** return P char */
char fF(char x);    /** return F char */
char W(char x);     /** return top T line char */
char A(char x);     /** return left T line char */
char D(char x);     /** return right T line char */
char X(char x);     /** return bottom T line char */

/** call function pointer
  * @param function pointer 
  * @param x is char used to pass into function pointer
  */
void addKey(char (*funcs)(char), char * x);

/** get key from user input then compare with each element in f_names array
  * @param key is received from main class pacman.c & IO.c
  */
void getKey(char * key);

#endif
