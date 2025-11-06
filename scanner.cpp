#include "scanner.h"    
#include <stdexcept> 
#include <string>    
#include <cstdlib>

//Construtor
Scanner::Scanner(string input, SymbolTable* table)
{
    pos = 0;
    line = 1;
    st = table;
    ifstream inputFile(input, ios::in);
    string lineContent;
    if (inputFile.is_open())
    {
        while (getline(inputFile, lineContent) )
        {
            if (!lineContent.empty() && lineContent.back() == '\r') {
                lineContent.pop_back();
            }
            this->input.append(lineContent + '\n');
        }
        inputFile.close();
    }
    else 
        cout << "Unable to open file\n";
}

int
Scanner::getLine()
{
    return line;
}

void Scanner::advance()
{
    if (pos < input.length() && input[pos] == '\n')
    {
        line++;
    }
    pos++;
}

Token* Scanner::nextToken()
{
    Token* tok = nullptr;
    string lexeme;
    
    while (pos < input.length())
    {
        //1. Ignora Espaços em Branco
        if (isspace(input[pos]))
        {
            advance();
            continue;
        }
        
        //2. Trata comentários (// e /*) ou OP_DIV (/)
        if (input[pos] == '/')
        {
            // Comentário de linha (//)
            if (pos + 1 < input.length() && input[pos+1] == '/')
            {
                pos += 2; // Pula "//"
                while (pos < input.length() && input[pos] != '\n')
                {
                    pos++;
                }
                continue; 
            }
            // Comentário de bloco (/*)
            else if (pos + 1 < input.length() && input[pos+1] == '*')
            {
                pos += 2; // Pula "/*"
                bool closed = false;
                while (pos < input.length())
                {
                    if (input[pos] == '*' && pos + 1 < input.length() && input[pos+1] == '/')
                    {
                        pos += 2; // Pula "*/"
                        closed = true;
                        break;
                    }
                    advance(); 
                }
                if (!closed)
                {
                    lexicalError("Comentario de bloco nao fechado");
                }
                continue; 
            }
            else
            {
                break;
            }
        }
        
        break;
    }


    //Fim da entrada
    if (pos >= input.length())
    {
        return new Token(END_OF_FILE);
    }
    
    char current_char = input[pos];
    
    //IDs (Começa com letra ou _)
    if (isalpha(current_char) || current_char == '_')
    {
        lexeme += current_char;
        advance();
        while (pos < input.length() && (isalnum(input[pos]) || input[pos] == '_'))
        {
            lexeme += input[pos];
            advance();
        }
        STEntry* obj = st->get(lexeme);
        if (!obj)
            tok = new Token(ID, lexeme);
        else 
            tok = new Token(obj->token->name, obj->token->lexeme);
    }
    //Números (INTEGER_LITERAL)
    else if (isdigit(current_char))
    {
        lexeme += current_char;
        advance();
        while (pos < input.length() && isdigit(input[pos]))
        {
            lexeme += input[pos];
            advance();
        }
        tok = new Token(INTEGER_LITERAL, lexeme);
    }
    //Strings (STRING_LITERAL)
    else if (current_char == '"')
    {
        advance(); // Pula o " inicial
        while (pos < input.length() && input[pos] != '"' && input[pos] != '\n')
        {
            lexeme += input[pos];
            advance();
        }
        if (pos >= input.length() || input[pos] == '\n')
        {
            lexicalError("Literal de string nao fechado");
        }
        
        advance(); // Pula o " final
        tok = new Token(STRING_LITERAL, lexeme);
    }
    //Operadores e Separadores
    else 
    {
        switch (current_char)
        {
            case '(': advance(); tok = new Token(SEP_LPAREN, "("); break;
            case ')': advance(); tok = new Token(SEP_RPAREN, ")"); break;
            case '[': advance(); tok = new Token(SEP_LBRACKET, "["); break;
            case ']': advance(); tok = new Token(SEP_RBRACKET, "]"); break;
            case '{': advance(); tok = new Token(SEP_LBRACE, "{"); break;
            case '}': advance(); tok = new Token(SEP_RBRACE, "}"); break;
            case ';': advance(); tok = new Token(SEP_SEMICOLON, ";"); break;
            case '.': advance(); tok = new Token(SEP_DOT, "."); break;
            case ',': advance(); tok = new Token(SEP_COMMA, ","); break;
            case '+': advance(); tok = new Token(OP_PLUS, "+"); break;
            case '-': advance(); tok = new Token(OP_MINUS, "-"); break;
            case '*': advance(); tok = new Token(OP_MULT, "*"); break;
            case '/': advance(); tok = new Token(OP_DIV, "/"); break;
            case '%': advance(); tok = new Token(OP_MOD, "%"); break;
            case '<':
                advance();
                if (pos < input.length() && input[pos] == '=') {
                    advance();
                    tok = new Token(OP_LE, "<=");
                } else {
                    tok = new Token(OP_LT, "<");
                }
                break;
            case '>':
                advance();
                if (pos < input.length() && input[pos] == '=') {
                    advance();
                    tok = new Token(OP_GE, ">=");
                } else {
                    tok = new Token(OP_GT, ">");
                }
                break;
            case '=':
                advance();
                if (pos < input.length() && input[pos] == '=') {
                    advance();
                    tok = new Token(OP_EQ, "==");
                } else {
                    tok = new Token(OP_ASSIGN, "="); // Envia 28
                }
                break;
            case '!':
                advance();
                if (pos < input.length() && input[pos] == '=') {
                    advance();
                    tok = new Token(OP_NE, "!=");
                } else {
                    lexicalError("Caractere '!' sozinho nao eh um token valido.");
                }
                break;
            default:
                lexeme += current_char;
                lexicalError("Caractere invalido: '" + lexeme + "'");
                break;
        }
    }
    return tok;
}


void 
Scanner::lexicalError(string msg)
{
    std::string error_msg = "Erro Lexico - Linha " + std::to_string(line) + ": " + msg;
    throw std::runtime_error(error_msg);
}