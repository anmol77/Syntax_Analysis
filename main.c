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
int lex();
void error();

#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99
#define TERM_CHAR 55

#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26

FILE *fp;

int main(int argc, const char * argv[]) {
    if ((fp = fopen(argv[1], "r")) == NULL)
        printf("Error - cannot open input.txt\n");
    else {
        getChar();
        do {
            lex();
        } while (nextToken != EOF);
    }
    return 0;
}

int lookup(char ch){
    switch (ch) {
            case '(':
                addChar();
                nextToken = LEFT_PAREN;
                break;
            case ')':
                addChar();
                nextToken = RIGHT_PAREN;
                break;
            case '+':
                addChar();
                nextToken = ADD_OP;
                break;
            case '-':
                addChar();
                nextToken = SUB_OP;
                break;
            case '*':
                addChar();
                nextToken = MULT_OP;
                break;
            case '/':
                addChar();
                nextToken = DIV_OP;
                break;
            case '\n':
                addChar();
                nextToken = '\n';
            default:
                addChar();
                nextToken = EOF;
                break;
        }
    return nextToken;
}

void getChar(){
    if ((nextChar = getc(in_fp)) != EOF){
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else charClass = UNKNOWN;
    }
    else
        charClass = EOF;
}

void addChar(){
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    }
}
