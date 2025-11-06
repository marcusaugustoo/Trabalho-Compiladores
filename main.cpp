#include "parser.h"
#include <iostream>   
#include <stdexcept>  
#include <cstdlib>    

int main(int argc, char* argv[]) 
{
    if (argc != 2)
    {
        std::cout << "Uso: ./xpp nome_arquivo.xpp\n"; 
        return 1;
    }

    try
    {
        Parser* parser = new Parser(argv[1]);
        parser->run();
        std::cout << "Compilacao encerrada com sucesso!" << std::endl;
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}