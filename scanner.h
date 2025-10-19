#ifndef SCANNER_H
#define SCANNER_H

#include <fstream>
#include <map>
#include <string>
#include "token.h"

class Scanner
{
private:
    string input; 
    int pos;    
    int line;
    map<string, int> reservedKeywords; //Mapa de palavras reservadas

public:
    //Construtor
    Scanner(string);

    int getLine();

    //Funções auxiliares para o Parser 
    int getPos();
    void setPos(int);
    int getLineInternal();
    void setLine(int);


    //Método que retorna o próximo token da entrada
    Token *nextToken();

    //Método para manipular erros
    void lexicalError(string);
};

#endif