#include "parser.h"
#include <stdexcept> 
#include <string>    

Parser::Parser(string input)
{
	currentST = globalST = new SymbolTable();
	initSimbolTable();

	scanner = new Scanner(input, globalST);
}

void
Parser::advance()
{
	lToken = scanner->nextToken();
}

void
Parser::match(int t)
{
    if (lToken->name == t) 
    {
		advance();
    }
	else
    {
        string expected = "Token esperado '" + to_string(t) + "'";
        string found = "'" + lToken->lexeme + "' (Token " + to_string(lToken->name) + ")";
		error(expected + ", mas encontrou " + found);
    }
}

void
Parser::run()
{
	advance();	
	program();
    if (lToken->name != END_OF_FILE)
    {
        error("Tokens inesperados apos o fim do programa.");
    }
}

//1. Program -> ClassList
void
Parser::program()
{
	if (lToken->name == CLASS)
		classList();
	else
		error("Esperado 'class' no inicio do programa.");
}

//2. ClassList -> ClassDecl+
void
Parser::classList()
{
	do
	{
		classDecl();
	}
	while(lToken->name == CLASS);
}

// 3.ClassDecl -> class ID 
void
Parser::classDecl()
{
	match(CLASS);
	match(ID);

	if (lToken->name == EXTENDS)
	{
		advance(); 
		match(ID);
	}

	match(SEP_LBRACE); 
	classBody();
	match(SEP_RBRACE); 
}

//4. ClassBody -> VarDeclListOpt ConstructDeclListOpt MethodDeclListOpt
void
Parser::classBody()
{
	varDeclListOpt();
	constructDeclListOpt();
	methodDeclListOpt();
}

//5. VarDeclListOpt -> VarDeclList | epsilon
void
Parser::varDeclListOpt()
{
	if (lToken->name == INT || lToken->name == STRING || lToken->name == ID)
	{
		varDeclList();
	}
}

//6. VarDeclList -> VarDecl+
void
Parser::varDeclList()
{
	do {
		varDecl();
	}
	while(lToken->name == INT || lToken->name == STRING || lToken->name == ID);
}

//7. VarDecl -> Type ID VarDeclOpt ;
void
Parser::varDecl()
{
	type();
	match(ID);
	varDeclOpt();
	match(SEP_SEMICOLON); 
}

//8. VarDeclOpt -> , ID VarDeclOpt | epsilon
void
Parser::varDeclOpt()
{
	if (lToken->name == SEP_COMMA) 
	{
		advance(); 
		match(ID);
		varDeclOpt();
	}
}

//9. Type -> int | string | ID
void
Parser::type()
{
	if (lToken->name == INT)
		match(INT);
	else if (lToken->name == STRING)
		match(STRING);
	else if (lToken->name == ID)
		match(ID);
	else
		error("Tipo (int, string, ou ID de classe) esperado.");
}

//10. ConstructDeclListOpt -> ConstructDeclList | epsilon
//11. ConstructDeclList -> ConstructDecl+
void
Parser::constructDeclListOpt()
{
    if (lToken->name == CONSTRUCTOR)
    {
        do {
            constructDecl();
        } while (lToken->name == CONSTRUCTOR);
    }
}

//12. ConstructDecl -> constructor MethodBody
void
Parser::constructDecl()
{
    match(CONSTRUCTOR);
    methodBody();
}

//13. MethodDeclListOpt -> MethodDeclList | epsilon
//14. MethodDeclList -> MethodDecl+
void
Parser::methodDeclListOpt()
{
    if (lToken->name == INT || lToken->name == STRING || lToken->name == ID)
    {
        do {
            methodDecl();
        } while (lToken->name == INT || lToken->name == STRING || lToken->name == ID);
    }
}

//15. MethodDecl -> Type ID MethodBody
void
Parser::methodDecl()
{
    type();
    match(ID);
    methodBody();
}

//16. MethodBody -> ( ParamListOpt ) { StatementsOpt }
void
Parser::methodBody()
{
    match(SEP_LPAREN); 
    paramListOpt();
    match(SEP_RPAREN); 
    match(SEP_LBRACE); 
    statementsOpt();
    match(SEP_RBRACE); 
}

//17. ParamListOpt -> ParamList | epsilon
void
Parser::paramListOpt()
{
    if (lToken->name == INT || lToken->name == STRING || lToken->name == ID)
    {
        paramList();
    }
}

//18. ParamList -> Param ( , Param )*
void
Parser::paramList()
{
    param();
    while (lToken->name == SEP_COMMA)
    {
        advance();
        param();
    }
}

//19. Param -> Type ID
void
Parser::param()
{
    type();
    match(ID);
}

//20. StatementsOpt -> Statements | epsilon
void
Parser::statementsOpt()
{
    if (lToken->name == INT || lToken->name == STRING || lToken->name == ID ||
        lToken->name == PRINT || lToken->name == READ || lToken->name == RETURN ||
        lToken->name == SUPER || lToken->name == IF || lToken->name == FOR ||
        lToken->name == BREAK || lToken->name == SEP_SEMICOLON)
    {
        statements();
    }
}

//21. Statements -> Statement+
void
Parser::statements()
{
    do {
        statement();
    } 
    while (lToken->name == INT || lToken->name == STRING || lToken->name == ID ||
           lToken->name == PRINT || lToken->name == READ || lToken->name == RETURN ||
           lToken->name == SUPER || lToken->name == IF || lToken->name == FOR ||
           lToken->name == BREAK || lToken->name == SEP_SEMICOLON);
}

//22. Statement -> ... (várias regras)
void
Parser::statement()
{
    switch (lToken->name)
    {
        case INT:
        case STRING:
            varDeclList(); 
            break;
        
        case ID:
        {
            match(ID); 
            lValueComp(); 

            if (lToken->name == OP_ASSIGN)
            {
                match(OP_ASSIGN);
                if (lToken->name == NEW)
                {
                    allocExpression();
                }
                else
                {
                    expression();
                }
                match(SEP_SEMICOLON); 
            }
            else if (lToken->name == ID)
            {
                match(ID); 
                varDeclOpt(); 
                match(SEP_SEMICOLON); 
            }
            else if (lToken->name == SEP_SEMICOLON) 
            {
                match(SEP_SEMICOLON);
            }
            else
            {
                error("Esperado '=', ID, ou ';' apos o LValue/Type.");
            }
            break;
        }

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
            error("Statement inesperado.");
            break;
    }
}

//23. AtribStat -> LValue = Expression | LValue = AllocExpression
void
Parser::atribStat()
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

//24. PrintStat -> print Expression
void
Parser::printStat()
{
    match(PRINT);
    expression();
}

//25. ReadStat -> read LValue
void
Parser::readStat()
{
    match(READ);
    lValue();
}

//26. ReturnStat -> return Expression
void
Parser::returnStat()
{
    match(RETURN);
    expression();
}

//27. SuperStat -> super ( ArgListOpt )
void
Parser::superStat()
{
    match(SUPER);
    match(SEP_LPAREN);
    argListOpt();
    match(SEP_RPAREN);
}

//28. IfStat -> if ( Expression ) { Statements } [ else { Statements } ]
void
Parser::ifStat()
{
    match(IF);
    match(SEP_LPAREN);
    expression();
    match(SEP_RPAREN);
    match(SEP_LBRACE);
    statementsOpt(); 
    match(SEP_RBRACE);
    
    if (lToken->name == ELSE)
    {
        advance();
        match(SEP_LBRACE);
        statementsOpt(); 
        match(SEP_RBRACE);
    }
}

//29. ForStat -> for ( AtribStatOpt ExpressionOpt AtribStatOpt ) { Statements }
void
Parser::forStat()
{
    match(FOR);
    match(SEP_LPAREN);
    atribStatOpt();
    expressionOpt();
    atribStatOpt();
    match(SEP_RPAREN);
    match(SEP_LBRACE);
    statementsOpt(); 
    match(SEP_RBRACE);
}

//30. AtribStatOpt -> AtribStat | epsilon
void
Parser::atribStatOpt()
{
    if (lToken->name == ID)
    {
        lValue();
        match(OP_ASSIGN); 
        if (lToken->name == NEW) {
            allocExpression();
        } else {
            expression();
        }
    }
}

//31. ExpressionOpt -> Expression | epsilon
void
Parser::expressionOpt()
{
    // CORREÇÃO 3: Removido '|| lToken->name == NEW'
    if (lToken->name == INTEGER_LITERAL || lToken->name == STRING_LITERAL ||
        lToken->name == ID || lToken->name == SEP_LPAREN ||
        lToken->name == OP_PLUS || lToken->name == OP_MINUS) 
    {
        expression();
    }
}

//32. LValue -> ID LValueComp
void
Parser::lValue()
{
    match(ID);
    lValueComp();
}

//33. LValueComp -> . ID LValueComp | . ID ( ArgListOpt ) LValueComp | [ Expression ] LValueComp | epsilon
void
Parser::lValueComp()
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

//34. Expression -> NumExpression [ RelOp NumExpression ] | AllocExpression
void
Parser::expression()
{
    numExpression();
    
    if (lToken->name == OP_LT || lToken->name == OP_GT || lToken->name == OP_LE ||
        lToken->name == OP_GE || lToken->name == OP_EQ || lToken->name == OP_NE)
    {
        relOp();
        numExpression();
    }
}

//35. AllocExpression -> new ID ( ArgListOpt ) | Type [ Expression ]
void
Parser::allocExpression()
{
    match(NEW);
    
    if (lToken->name == INT || lToken->name == STRING)
    {
        type(); 
        match(SEP_LBRACKET);
        expression();
        match(SEP_RBRACKET);
    }
    else if (lToken->name == ID)
    {
        match(ID); 
        
        if (lToken->name == SEP_LPAREN)
        {
            match(SEP_LPAREN);
            argListOpt();
            match(SEP_RPAREN);
        }
        else if (lToken->name == SEP_LBRACKET)
        {
            match(SEP_LBRACKET);
            expression();
            match(SEP_RBRACKET);
        }
        else
        {
            error("Esperado '(' ou '[' apos 'new ID'");
        }
    }
    else
    {
        error("Alocacao invalida. Esperado ID ou tipo (int/string).");
    }
}

//36. NumExpression -> Term ( ( + | - ) Term )*
void
Parser::numExpression()
{
    term();
    while (lToken->name == OP_PLUS || lToken->name == OP_MINUS)
    {
        advance();
        term();
    }
}

void
Parser::relOp()
{
    if (lToken->name == OP_LT || lToken->name == OP_GT || lToken->name == OP_LE ||
        lToken->name == OP_GE || lToken->name == OP_EQ || lToken->name == OP_NE)
    {
        advance();
    }
    else
    {
        error("Operador relacional (==, !=, <, <=, >, >=) esperado.");
    }
}

//37. Term -> UnaryExpression ( ( * | / | % ) UnaryExpression )*
void
Parser::term()
{
    unaryExpression();
    while (lToken->name == OP_MULT || lToken->name == OP_DIV || lToken->name == OP_MOD)
    {
        advance();
        unaryExpression();
    }
}

//38. UnaryExpression -> [ + | - ] Factor
void
Parser::unaryExpression()
{
    if (lToken->name == OP_PLUS || lToken->name == OP_MINUS)
    {
        advance();
    }
    factor();
}

//39. Factor -> INTEGER_LITERAL | STRING_LITERAL | LValue | ( Expression )
void
Parser::factor()
{
    switch (lToken->name)
    {
        case INTEGER_LITERAL:
            match(INTEGER_LITERAL);
            break;
        case STRING_LITERAL:
            match(STRING_LITERAL);
            break;
        case ID: // FIRST(LValue)
            lValue();
            break;
        case SEP_LPAREN: // (
            advance();
            expression();
            match(SEP_RPAREN);
            break;
        default:
            error("Fator inesperado (esperado numero, string, ID ou '( )').");
            break;
    }
}

//40. ArgListOpt -> ArgList | epsilon
void
Parser::argListOpt()
{
    // CORREÇÃO 4: Removido '|| lToken->name == NEW'
    if (lToken->name == INTEGER_LITERAL || lToken->name == STRING_LITERAL ||
        lToken->name == ID || lToken->name == SEP_LPAREN ||
        lToken->name == OP_PLUS || lToken->name == OP_MINUS) 
    {
        argList();
    }
}

//41. ArgList -> Expression ( , Expression )*
void
Parser::argList()
{
    expression();
    while (lToken->name == SEP_COMMA)
    {
        advance();
        expression();
    }
}

void
Parser::initSimbolTable()
{
	Token* t;
        
	t = new Token(CLASS, "class");
	globalST->add(new STEntry(t, true));
	t = new Token(EXTENDS, "extends");
    globalST->add(new STEntry(t, true));
    t = new Token(PUBLIC, "public"); 
    globalST->add(new STEntry(t, true));
	t = new Token(INT, "int");
    globalST->add(new STEntry(t, true));
	t = new Token(STRING, "string");
    globalST->add(new STEntry(t, true));
	t = new Token(BREAK, "break");
    globalST->add(new STEntry(t, true));
	t = new Token(PRINT, "print");
    globalST->add(new STEntry(t, true));
	t = new Token(READ, "read");
    globalST->add(new STEntry(t, true));
	t = new Token(RETURN, "return");
    globalST->add(new STEntry(t, true));
	t = new Token(SUPER, "super");
    globalST->add(new STEntry(t, true));
	t = new Token(IF, "if");
    globalST->add(new STEntry(t, true));
	t = new Token(ELSE, "else");
    globalST->add(new STEntry(t, true));
	t = new Token(FOR, "for");
    globalST->add(new STEntry(t, true));
	t = new Token(NEW, "new");
    globalST->add(new STEntry(t, true));
	t = new Token(CONSTRUCTOR, "constructor");
    globalST->add(new STEntry(t, true));
}

void
Parser::error(string str)
{
    std::string error_msg = "Erro Sintatico - Linha " + std::to_string(scanner->getLine()) + ": " + str;
    throw std::runtime_error(error_msg);
}