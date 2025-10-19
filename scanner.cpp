#include "scanner.h"

//Construtor que recebe uma string com o nome do arquivo 
//de entrada e preenche input com seu conteúdo.
Scanner::Scanner(string input_file)
{
    pos = 0;
    line = 1;

    //Preenche o mapa de palavras reservadas
    reservedKeywords["class"] =       CLASS;
    reservedKeywords["extends"] =     EXTENDS;
    reservedKeywords["int"] =         INT;
    reservedKeywords["string"] =      STRING;
    reservedKeywords["break"] =       BREAK;
    reservedKeywords["print"] =       PRINT;
    reservedKeywords["read"] =        READ;
    reservedKeywords["return"] =      RETURN;
    reservedKeywords["super"] =       SUPER;
    reservedKeywords["if"] =          IF;
    reservedKeywords["else"] =        ELSE;
    reservedKeywords["for"] =         FOR;
    reservedKeywords["new"] =         NEW;
    reservedKeywords["constructor"] = CONSTRUCTOR;


    ifstream inputFile(input_file, ios::in);
    string file_line;

    if (inputFile.is_open())
    {
        while (getline(inputFile, file_line))
        {
            this->input.append(file_line + '\n');
        }
        inputFile.close();
    }
    else
        cout << "Unable to open file\n";
}

int Scanner::getLine()
{
    return line;
}

//Funções auxiliares para o Parser
int Scanner::getPos() { return pos; }
void Scanner::setPos(int p) { pos = p; }
int Scanner::getLineInternal() { return line; }
void Scanner::setLine(int l) { line = l; }


//Método que retorna o próximo token da entrada
Token *
Scanner::nextToken()
{
    string lexeme;

    //Loop para ignorar espaços em branco e comentários
    while (pos < input.length())
    {
        //Ignora espaços em branco e tabulações
        if (input[pos] == ' ' || input[pos] == '\t' || input[pos] == '\r')
        {
            pos++;
            continue;
        }
        //Trata nova linha
        else if (input[pos] == '\n')
        {
            line++;
            pos++;
            continue;
        }
        //Trata comentários
        else if (input[pos] == '/')
        {
            //Comentário de linha
            if (pos + 1 < input.length() && input[pos + 1] == '/')
            {
                while (pos < input.length() && input[pos] != '\n')
                {
                    pos++;
                }
                continue; 
            }
            //Comentário de bloco
            else if (pos + 1 < input.length() && input[pos + 1] == '*')
            {
                pos += 2; 
                while (pos + 1 < input.length() && (input[pos] != '*' || input[pos + 1] != '/'))
                {
                    if (input[pos] == '\n')
                    {
                        line++;
                    }
                    pos++;
                }
                if (pos + 1 >= input.length())
                {
                    lexicalError("Comentario de bloco nao fechado.");
                }
                pos += 2; 
                continue;
            }
            // Se for apenas '/', é o operador de divisão
            else
            {
                break;
            }
        }
        else
        {
            break; 
        }
    }

    //Verifica fim de arquivo
    if (pos >= input.length())
    {
        return new Token(END_OF_FILE);
    }

    //Reconhecimento de ID e Palavras Reservadas
    if (isalpha(input[pos]) || input[pos] == '_')
    {
        lexeme += input[pos++];
        while (pos < input.length() && (isalnum(input[pos]) || input[pos] == '_'))
        {
            lexeme += input[pos++];
        }

        //Verifica se o lexema é uma palavra reservada
        if (reservedKeywords.count(lexeme))
        {
            return new Token(reservedKeywords[lexeme], lexeme);
        }
        
        return new Token(ID, lexeme);
    }

    //Reconhecimento de Números Inteiros
    if (isdigit(input[pos]))
    {
        lexeme += input[pos++];
        while (pos < input.length() && isdigit(input[pos]))
        {
            lexeme += input[pos++];
        }
        return new Token(INTEGER_LITERAL, lexeme);
    }

    //Reconhecimento de Literais String
    if (input[pos] == '"')
    {
        pos++; 
        while (pos < input.length() && input[pos] != '"')
        {
            if (input[pos] == '\n')
            { 
                lexicalError("String literal nao fechada na mesma linha.");
            }
            lexeme += input[pos++];
        }
        if (pos >= input.length())
        {
            lexicalError("String literal nao fechada.");
        }
        pos++; 
        return new Token(STRING_LITERAL, lexeme);
    }

    //Reconhecimento de Operadores e Separadores
    switch (input[pos++])
    {
    //Operadores de dois caracteres
    case '<':
        if (pos < input.length() && input[pos] == '=') { pos++; return new Token(OP_LE, "<="); } else return new Token(OP_LT, "<");
    case '>':
        if (pos < input.length() && input[pos] == '=') { pos++; return new Token(OP_GE, ">="); } else return new Token(OP_GT, ">");
    case '=':
        if (pos < input.length() && input[pos] == '=') { pos++; return new Token(OP_EQ, "=="); } else return new Token(OP_ASSIGN, "=");
    case '!':
        if (pos < input.length() && input[pos] == '=') { pos++; return new Token(OP_NE, "!="); } else { lexicalError("Caractere '!' invalido."); return new Token(UNDEF); }

    //Operadores de um caractere
    case '+': return new Token(OP_SUM, "+");
    case '-': return new Token(OP_SUB, "-");
    case '*': return new Token(OP_MUL, "*");
    case '/': return new Token(OP_DIV, "/");
    case '%': return new Token(OP_MOD, "%");

    //Separadores
    case '(': return new Token(SEP_LPAREN, "(");
    case ')': return new Token(SEP_RPAREN, ")");
    case '[': return new Token(SEP_LBRACKET, "[");
    case ']': return new Token(SEP_RBRACKET, "]");
    case '{': return new Token(SEP_LBRACE, "{");
    case '}': return new Token(SEP_RBRACE, "}");
    case ',': return new Token(SEP_COMMA, ",");
    case '.': return new Token(SEP_DOT, ".");
    case ';': return new Token(SEP_SEMICOLON, ";");

    default:
        lexicalError("Caractere invalido: '" + string(1, input[pos - 1]) + "'");
    }

    return new Token(UNDEF); 
}

void
Scanner::lexicalError(string msg)
{
    cout << "Linha " << line << ": " << msg << endl;

    exit(EXIT_FAILURE);
}