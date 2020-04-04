#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

int charClass;
char lexeme[1000];
char nextChar;
char * line = NULL;
int lexLen;
int token;
int nextToken;
int currentIndexCount;
int lineNumber = 0;
int line_ind;
bool errorCalled = false;
bool successfulLineRead = false;

void addChar();
void getChar();
void stmt();
void expr();
void term();
void factor();
int lookup(char ch);
int is_empty(char *s);
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
size_t len = 0;
ssize_t read;

int main(int argc, const char * argv[]) {
    if ((fp = fopen(argv[1], "r")) == NULL)
        printf("Error - cannot open input.txt\n");
    while((read = getline(&line, &len, fp)) != -1) {
        successfulLineRead = true;
        lineNumber += 1;
        currentIndexCount = 0;
        getChar();
        do {
            lex();
            stmt();
        } while (nextToken != EOF && !(errorCalled));
    }
    if ((getline(&line, &len, fp)) == -1 && (!successfulLineRead)){
        getChar();
        lex();
    }
    printf("\n");
    return 0;
}

int is_empty(char *s) {
  while (*s != '\0') {
    if (!isspace((unsigned char)*s))
      return 0;
    s++;
  }
    return 1;
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
            case '=':
                addChar();
                nextToken = ASSIGN_OP;
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
    if (line[currentIndexCount] == '\n' || line[currentIndexCount] == '\0' || is_empty(&line[currentIndexCount])) {
        charClass = EOF;
    }
    else {
        nextChar = line[currentIndexCount];
        currentIndexCount += 1;
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else charClass = UNKNOWN;
    }
}

void addChar(){
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    } else {
        printf("Error - lexeme is too long\n");
    }
}

void getNonBlank(){
    while (isspace(nextChar))
        getChar();
}

int lex(){
    lexLen = 0;
    getNonBlank();
    switch(charClass){
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = IDENT;
            break;
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;
        case UNKNOWN:
            lookup(nextChar);
            getChar();
            break;
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;
    }
    printf("Next token is: %d Next lexeme is %s\n", nextToken, lexeme);
    return nextToken;
}

/*
    Parser for arithmetic exprsn:
    <stmt>    -->  id = <expr>
    <expr>    -->  <term>   {(+ | -) <term>}
    <term>    -->  <factor> {(* | /) <factor>}
    <factor>  -->  id | int_constant | (<expr>)
*/
void stmt(){
    printf("Enter <stmt>\n");
    if (nextToken == IDENT){
        lex();
        while (nextToken == ASSIGN_OP){
            lex();
            expr();
        }
    }
    printf("Exit <stmt>\n");
}

void expr(){
    printf("Enter <expr>\n");
    term();
    while (nextToken == ADD_OP || nextToken == SUB_OP) {
           lex ();
           term ();
       }
    printf("Exit <expr>\n");
} /* End of function expr */

void term() {
    printf("Enter <term>\n");
    factor();
    while (nextToken == MULT_OP || nextToken == DIV_OP) {
        lex();
        factor();
    }
    printf("Exit <term>\n");
}

void factor(){
    printf("Enter <factor>\n");
    if (nextToken == IDENT || nextToken == INT_LIT)
        lex();
    else {
        if (nextToken == LEFT_PAREN) {
            lex();
            expr();
            if (nextToken == RIGHT_PAREN) {
                lex();
            } else
                error();
        }
        else
            error();
    }
    printf("Exit <factor>\n");
}

void error() {
  printf("Error\n");
}
