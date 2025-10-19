#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <ctype.h>
#include <string>
#include <map>

using namespace std;

//Enumeração com todos os tokens da linguagem X++
enum Names
{
    UNDEF,
    END_OF_FILE,

    //Identificador
    ID,

    //Literais
    INTEGER_LITERAL,
    STRING_LITERAL,

    //Palavras Reservadas
    CLASS,
    EXTENDS,
    INT,
    STRING,
    BREAK,
    PRINT,
    READ,
    RETURN,
    SUPER,
    IF,
    ELSE,
    FOR,
    NEW,
    CONSTRUCTOR,

    //Operadores
    OP_LE, // <=
    OP_GE, // >=
    OP_EQ, // ==
    OP_NE, // !=
    OP_LT, // <
    OP_GT, // >
    OP_ASSIGN, // =
    OP_SUM, // +
    OP_SUB, // -
    OP_MUL, // *
    OP_DIV, // /
    OP_MOD, // %

    //Separadores
    SEP_LPAREN,   // (
    SEP_RPAREN,   // )
    SEP_LBRACKET, // [
    SEP_RBRACKET, // ]
    SEP_LBRACE,   // {
    SEP_RBRACE,   // }
    SEP_COMMA,    // ,
    SEP_DOT,      // .
    SEP_SEMICOLON // ;
};

class Token
{
public:
    int name;
    int attribute;
    string lexeme;

    Token(int name)
    {
        this->name = name;
        attribute = UNDEF;
    }

    Token(int name, string l)
    {
        this->name = name;
        attribute = UNDEF;
        lexeme = l;
    }

    Token(int name, int attr)
    {
        this->name = name;
        attribute = attr;
    }
};

#endif