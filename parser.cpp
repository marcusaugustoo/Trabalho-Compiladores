#include "parser.h"
#include <string>

Parser::Parser(string input)
{
    scanner = new Scanner(input);
}

void Parser::advance()
{
    lToken = scanner->nextToken();
}

void Parser::match(int t)
{
    if (lToken->name == t)
        advance();
    else
    {
        error("Erro sintatico: esperado token <" + to_string(t) + "> mas encontrado <" + to_string(lToken->name) + "> (" + lToken->lexeme + ")");
    }
}

void Parser::run()
{
    advance();
    program();
    
    if (lToken->name != END_OF_FILE) {
        error("Erro sintatico: lixo encontrado apos o fim do programa.");
    }
    
    cout << "Compilacao encerrada com sucesso!" << endl;
}

void Parser::error(string str)
{
    cout << "Linha " << scanner->getLine() << ": " << str << endl;
    exit(EXIT_FAILURE);
}

//1. Program → ClassList
void Parser::program()
{
    if (lToken->name == CLASS) {
        classList();
    } else {
        error("Um programa deve comecar com a declaracao de uma classe.");
    }
}

//2. ClassList → ClassDecl ClassList | ClassDecl
//Implementação (não recursiva à esquerda): ClassList -> ClassDecl { ClassDecl }
void Parser::classList()
{
    classDecl();
    while (lToken->name == CLASS)
    {
        classDecl();
    }
}

//3. ClassDecl → class ID ClassBody | class ID extends ID ClassBody
void Parser::classDecl()
{
    match(CLASS);
    match(ID);
    if (lToken->name == EXTENDS)
    {
        advance();
        match(ID);
    }
    classBody();
}

//4. ClassBody → { VarDeclListOpt ConstructDeclListOpt MethodDeclListOpt }
void Parser::classBody()
{
    match(SEP_LBRACE);
    varDeclListOpt();
    constructDeclListOpt();
    methodDeclListOpt();
    match(SEP_RBRACE);
}

//5. VarDeclListOpt → VarDeclList | ε
//Implementação: { VarDecl }*
//Precisamos de lookahead(2) para diferenciar VarDecl de MethodDecl
void Parser::varDeclListOpt()
{
    while (lToken->name == INT || lToken->name == STRING || lToken->name == ID)
    {
        int oldPos = scanner->getPos();
        int oldLine = scanner->getLineInternal();
        Token *t1 = lToken; 
        
        advance();
        Token *t2 = lToken; 
        
        advance();
        Token *t3 = lToken; 

        scanner->setPos(oldPos);
        scanner->setLine(oldLine);
        lToken = t1; 

        if (t3->name == SEP_LPAREN)
        {
            break;
        }

        varDecl();
    }
}

//6. VarDeclList → VarDeclList VarDecl | VarDecl (implementado em varDeclListOpt)
//7. VarDecl → Type ID VarDeclOpt;
void Parser::varDecl()
{
    type();
    match(ID);
    varDeclOpt();
    match(SEP_SEMICOLON);
}

//8. VarDeclOpt → , ID VarDeclOpt | ε
void Parser::varDeclOpt()
{
    if (lToken->name == SEP_COMMA)
    {
        advance();
        match(ID);
        varDeclOpt();
    }
}

//9. Type → int | string | ID
void Parser::type()
{
    if (lToken->name == INT)
        advance();
    else if (lToken->name == STRING)
        advance();
    else if (lToken->name == ID)
        advance();
    else
        error("Erro sintatico: esperado um tipo (int, string, ou ID).");
}

//10. ConstructDeclListOpt → ConstructDeclList | ε
//Implementação: { ConstructDecl }*
void Parser::constructDeclListOpt()
{
    while (lToken->name == CONSTRUCTOR)
    {
        constructDecl();
    }
}

//11. ConstructDeclList → ... | ConstructDecl (implementado acima)
//12. ConstructDecl → constructor MethodBody
void Parser::constructDecl()
{
    match(CONSTRUCTOR);
    methodBody();
}

//13. MethodDeclListOpt → MethodDeclList | ε
//Implementação: { MethodDecl }*
void Parser::methodDeclListOpt()
{
    while (lToken->name == INT || lToken->name == STRING || lToken->name == ID)
    {
        methodDecl();
    }
}

//14. MethodDeclList → ... | MethodDecl (implementado acima)
//15. MethodDecl → Type ID MethodBody (assumindo 'id' é typo para 'ID')
void Parser::methodDecl()
{
    type();
    match(ID);
    methodBody();
}

//16. MethodBody → ( ParamListOpt ) { StatementsOpt }
void Parser::methodBody()
{
    match(SEP_LPAREN);
    paramListOpt();
    match(SEP_RPAREN);
    match(SEP_LBRACE);
    statementsOpt();
    match(SEP_RBRACE);
}

//17. ParamListOpt → ParamList | ε
void Parser::paramListOpt()
{
    if (lToken->name == INT || lToken->name == STRING || lToken->name == ID)
    {
        paramList();
    }
}

//18. ParamList → ParamList , Param | Param
//Implementação: Param { , Param }
void Parser::paramList()
{
    param();
    while (lToken->name == SEP_COMMA)
    {
        advance();
        param();
    }
}

//19. Param → Type ID
void Parser::param()
{
    type();
    match(ID);
}

//20. StatementsOpt → Statements | ε
void Parser::statementsOpt()
{
    if (lToken->name == ID || lToken->name == PRINT || lToken->name == READ ||
        lToken->name == RETURN || lToken->name == SUPER || lToken->name == IF ||
        lToken->name == FOR || lToken->name == BREAK || lToken->name == SEP_SEMICOLON ||
        lToken->name == INT || lToken->name == STRING)
    {
        statements();
    }
}

//21. Statements → Statements Statement | Statement
//Implementação: { Statement }
void Parser::statements()
{
    while (lToken->name == ID || lToken->name == PRINT || lToken->name == READ ||
           lToken->name == RETURN || lToken->name == SUPER || lToken->name == IF ||
           lToken->name == FOR || lToken->name == BREAK || lToken->name == SEP_SEMICOLON ||
           lToken->name == INT || lToken->name == STRING)
    {
        statement();
    }
}

//22. Statement → ...
void Parser::statement()
{
    switch (lToken->name)
    {
    case ID: 
        atribStat();
        match(SEP_SEMICOLON);
        break;
    case INT: 
    case STRING:
        varDecl(); 
        break;
    case PRINT:
        printStat();
        match(SEP_SEMICOLON);
        break;
    case READ:
        readStat();
        match(SEP_SEMICOLON);
        break;
    case RETURN:
        returnStat();
        match(SEP_SEMICOLON);
        break;
    case SUPER:
        superStat();
        match(SEP_SEMICOLON);
        break;
    case IF:
        ifStat();
        break;
    case FOR:
        forStat();
        break;
    case BREAK:
        advance();
        match(SEP_SEMICOLON);
        break;
    case SEP_SEMICOLON:
        advance();
        break;
    default:
        error("Erro sintatico: instrucao (statement) invalida.");
    }
}

//23. AtribStat → LValue = Expression | LValue = AllocExpression
void Parser::atribStat()
{
    lValue();
    match(OP_ASSIGN);
    if (lToken->name == NEW)
    {
        allocExpression();
    }
    else
    {
        expression();
    }
}

//24. PrintStat → print Expression
void Parser::printStat()
{
    match(PRINT);
    expression();
}

//25. ReadStat → read LValue
void Parser::readStat()
{
    match(READ);
    lValue();
}

//26. ReturnStat → return Expression
void Parser::returnStat()
{
    match(RETURN);
    if(lToken->name != SEP_SEMICOLON)
    {
       expression();
    }
}

//27. SuperStat → super 
void Parser::superStat()
{
    match(SUPER);
    match(SEP_LPAREN);
    argListOpt();
    match(SEP_RPAREN);
}

//28. IfStat → if ( Expression ) { Statements } [ | if ( Expression ) { Statements } else { Statements } ]
void Parser::ifStat()
{
    match(IF);
    match(SEP_LPAREN);
    expression();
    match(SEP_RPAREN);
    match(SEP_LBRACE);
    statements();
    match(SEP_RBRACE);

    if (lToken->name == ELSE)
    {
        advance();
        match(SEP_LBRACE);
        statements();
        match(SEP_RBRACE);
    }
}

//29. ForStat → for ( AtribStatOpt ; ExpressionOpt ; AtribStatOpt ) { Statements }
void Parser::forStat()
{
    match(FOR);
    match(SEP_LPAREN);
    atribStatOpt();
    match(SEP_SEMICOLON);
    expressionOpt();
    match(SEP_SEMICOLON);
    atribStatOpt();
    match(SEP_RPAREN);
    match(SEP_LBRACE);
    statements();
    match(SEP_RBRACE);
}

//30. AtribStatOpt → AtribStat | ε
void Parser::atribStatOpt()
{
    if (lToken->name == ID) 
    {
        atribStat();
    }
}

//31. ExpressionOpt → Expression | ε
void Parser::expressionOpt()
{
    if (lToken->name == OP_SUM || lToken->name == OP_SUB ||
        lToken->name == INTEGER_LITERAL || lToken->name == STRING_LITERAL ||
        lToken->name == ID || lToken->name == SEP_LPAREN)
    {
        expression();
    }
}

//32. LValue → ID LValueComp
void Parser::lValue()
{
    match(ID);
    lValueComp();
}

//33. LValueComp → . ID LValueComp | [ Expression ] LValueComp | . ID ( ArgListOpt ) LValueComp | ε
void Parser::lValueComp()
{
    if (lToken->name == SEP_DOT)
    {
        advance();
        match(ID);
        if (lToken->name == SEP_LPAREN) 
        {
            advance();
            argListOpt();
            match(SEP_RPAREN);
        }
        lValueComp();
    }
    else if (lToken->name == SEP_LBRACKET)
    {
        advance();
        expression();
        match(SEP_RBRACKET);
        lValueComp();
    }
}

//34. Expression → NumExpression [ | NumExpression RelOp NumExpression ]
void Parser::expression()
{
    numExpression();
    if (lToken->name == OP_LT || lToken->name == OP_GT || lToken->name == OP_LE ||
        lToken->name == OP_GE || lToken->name == OP_EQ || lToken->name == OP_NE)
    {
        relOp();
        numExpression();
    }
}

//35. AllocExpression → new ID ( ArgListOpt ) | Type [ Expression ]
void Parser::allocExpression()
{
    if (lToken->name == NEW)
    {
        advance();
        match(ID);
        match(SEP_LPAREN);
        argListOpt();
        match(SEP_RPAREN);
    }
    else if (lToken->name == INT || lToken->name == STRING || lToken->name == ID)
    {
        type();
        match(SEP_LBRACKET);
        expression();
        match(SEP_RBRACKET);
    }
    else
    {
        error("Erro sintatico: esperado 'new' ou um tipo para alocacao.");
    }
}

//36. NumExpression → Term { + Term | - Term }
void Parser::numExpression()
{
    term();
    while (lToken->name == OP_SUM || lToken->name == OP_SUB)
    {
        advance();
        term();
    }
}

//Helper para RelOp
void Parser::relOp()
{
    switch (lToken->name)
    {
    case OP_LT: advance(); break;
    case OP_GT: advance(); break;
    case OP_LE: advance(); break;
    case OP_GE: advance(); break;
    case OP_EQ: advance(); break;
    case OP_NE: advance(); break;
    default:
        error("Erro sintatico: esperado operador relacional.");
    }
}

//37. Term → UnaryExpression { * UnaryExpression | / UnaryExpression | % UnaryExpression }
void Parser::term()
{
    unaryExpression();
    while (lToken->name == OP_MUL || lToken->name == OP_DIV || lToken->name == OP_MOD)
    {
        advance();
        unaryExpression();
    }
}

//38. UnaryExpression → [ + | - ] Factor
void Parser::unaryExpression()
{
    if (lToken->name == OP_SUM || lToken->name == OP_SUB)
    {
        advance();
    }
    factor();
}

//39. Factor → INTEGER_LITERAL | STRING_LITERAL | LValue | ( Expression )
void Parser::factor()
{
    switch (lToken->name)
    {
    case INTEGER_LITERAL:
        advance();
        break;
    case STRING_LITERAL:
        advance();
        break;
    case ID:
        lValue();
        break;
    case SEP_LPAREN:
        advance();
        expression();
        match(SEP_RPAREN);
        break;
    default:
        error("Erro sintatico: esperado INTEGER_LITERAL, STRING_LITERAL, ID ou (Expression).");
    }
}

//40. ArgListOpt → ArgList | ε
void Parser::argListOpt()
{
    if (lToken->name == OP_SUM || lToken->name == OP_SUB ||
        lToken->name == INTEGER_LITERAL || lToken->name == STRING_LITERAL ||
        lToken->name == ID || lToken->name == SEP_LPAREN)
    {
        argList();
    }
}

// 41. ArgList → Expression { , Expression }
void Parser::argList()
{
    expression();
    while (lToken->name == SEP_COMMA)
    {
        advance();
        expression();
    }
}