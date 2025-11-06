#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include <string>

class Parser
{
private:
    Scanner *scanner;
    Token *lToken; 
    SymbolTable* globalST;
    SymbolTable* currentST;

    void advance();
    void match(int);
    void initSimbolTable();
    void error(string);

    //Funções para cada não-terminal da gramática
    void program();
    void classList();
    void classDecl();
    void classBody();
    void varDeclListOpt();
    
    void varDeclList(); 

    void varDecl();
    void varDeclOpt();
    void type();
    void constructDeclListOpt();
    void constructDecl();
    void methodDeclListOpt();
    void methodDecl();
    void methodBody();
    void paramListOpt();
    void paramList();
    void param();
    void statementsOpt();
    void statements();
    void statement();
    void atribStat();
    void printStat();
    void readStat();
    void returnStat();
    void superStat();
    void ifStat();
    void forStat();
    void atribStatOpt();
    void expressionOpt();
    void lValue();
    void lValueComp();
    void expression();
    void allocExpression();
    void numExpression();
    void relOp();
    void term();
    void unaryExpression();
    void factor();
    void argListOpt();
    void argList();


public:
    Parser(string);
    void run();
};

#endif