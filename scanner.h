#ifndef SCANNER_H
#define SCANNER_H

#include <fstream>
#include "token.h"

class Scanner 
{
    private: 
        string input;
        int pos;
        int line;
    
    public:
    //Construtor
        Scanner(string);

        int getLine();
    
        //Método que retorna o próximo token da entrada
        Token* nextToken();        
    
        //Método para manipular erros
        void lexicalError(string);
};

#endif