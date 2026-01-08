
#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

enum class TokenType {
    // keywords
    INT, BOOL, VOID, RETURN, IF, WHILE, TRUE, FALSE,

    // identifiers & literals
    IDENT, NUMBER,

    // operators
    PLUS, MINUS, STAR, SLASH,
    ASSIGN,
    EQ, NEQ, LT, GT, LE, GE,
    AND, OR, NOT,

    // punctuation
    SEMI, COMMA,
    LPAREN, RPAREN,
    LBRACE, RBRACE,

    END_OF_FILE,
    UNKNOWN
};

struct Token {
    TokenType type;
    string lexeme;
    int line;

    Token(TokenType t, string l, int ln)
        : type(t), lexeme(move(l)), line(ln) {}
};

class Lexer {
public:
    explicit Lexer(const string& src);

    vector<Token> tokenize();

private:
    string source;
    size_t pos = 0;
    int line = 1;

    bool isAtEnd() const;
    char peek() const;
    char advance();
    void skipWhitespace();

    Token identifier();
    Token number();
    Token makeToken(TokenType type, const string& lexeme);

    static unordered_map<string, TokenType> keywords;
};

#endif
