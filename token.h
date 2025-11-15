#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <ctype.h>
#include <string>

using namespace std;

enum Names 
{
    UNDEF, 
    ID, 
    
    //Palavras Reservadas
    CLASS, // 2
    EXTENDS, // 3
    INT, // 4
    STRING, // 5
    BREAK, // 6
    PRINT, // 7
    READ, // 8
    RETURN, // 9
    SUPER, // 10
    IF, // 11
    ELSE, // 12
    FOR, // 13
    NEW, // 14
    CONSTRUCTOR, // 15
    
    //Literais
    INTEGER_LITERAL, // 16
    STRING_LITERAL, // 17
    
    //Operadores 
    OP_LT,   // < 18
    OP_GT,   // > 19
    OP_LE,   // <= 20
    OP_GE,   // >= 21
    OP_PLUS, // + 22
    OP_MINUS,// - 23
    OP_MULT, // * 24
    OP_DIV,  // / 25
    OP_MOD,  // % 26
    OP_ASSIGN, // = 27  
    OP_EQ,   // == 28
    OP_NE,   // != 29
    
    //Separadores
    SEP_LPAREN,   // ( 30
    SEP_RPAREN,   // ) 31
    SEP_LBRACKET, // [ 32
    SEP_RBRACKET, // ] 33
    SEP_LBRACE,   // { 34
    SEP_RBRACE,   // } 35
    SEP_SEMICOLON,// ; 36
    SEP_DOT,      // . 37
    SEP_COMMA,    // , 38

    END_OF_FILE // 39
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