#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int charClass;
char lexeme[1000];
char nextChar;
char * line = NULL;
int lexLen;
int token;
int nextToken;
int line_ind;

void addChar();
void getChar();
void expr();
void term();
void factor();
int lookup(char ch);
