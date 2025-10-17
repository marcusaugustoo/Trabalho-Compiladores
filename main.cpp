#include "scanner.h"
#include <iostream>

int main(int argc, char* argv[]) 
{

    if (argc != 2)
    {
        cout << "Uso: ./compiler nome_arquivo.mj\n";
        return 1;
    }
    
    Scanner* scanner = new Scanner(argv[1]);
    
    Token* t;
    
    do
    {
        t = scanner->nextToken();
        
        // Imprime o nome (enum) e o lexema do token
        cout << "<" << t->name << ", \"" << t->lexeme << "\">" << endl;

    } while (t->name != END_OF_FILE);

    delete scanner;
    
    return 0;
}