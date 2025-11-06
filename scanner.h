#include "symboltable.h"
#include <fstream>

class Scanner 
{
    private: 
        string input; 
        int pos; 
        int line;
        SymbolTable* st;
    
        void advance(); 

    public:
    //Construtor
        Scanner(string, SymbolTable*);

        int getLine();
    
        //Método que retorna o próximo token da entrada
        Token* nextToken();
    
        //Método para manipular erros
        void lexicalError(string);

};