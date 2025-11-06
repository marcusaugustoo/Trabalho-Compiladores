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
    PUBLIC,  // 16
    
    //Literais
    INTEGER_LITERAL, // 17
    STRING_LITERAL, // 18
    
    //Operadores 
    OP_LT,   // < 19
    OP_GT,   // > 20
    OP_LE,   // <= 21
    OP_GE,   // >= 22
    OP_PLUS, // + 23
    OP_MINUS,// - 24
    OP_MULT, // * 25
    OP_DIV,  // / 26
    OP_MOD,  // % 27
    OP_ASSIGN, // = 28  
    OP_EQ,   // == 29
    OP_NE,   // != 30
    
    //Separadores
    SEP_LPAREN,   // ( 31
    SEP_RPAREN,   // ) 32
    SEP_LBRACKET, // [ 33
    SEP_RBRACKET, // ] 34
    SEP_LBRACE,   // { 35
    SEP_RBRACE,   // } 36
    SEP_SEMICOLON,// ; 37
    SEP_DOT,      // . 38
    SEP_COMMA,    // , 39

    END_OF_FILE // 40
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