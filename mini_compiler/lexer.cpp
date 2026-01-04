
#include "lexer.h"
#include <cctype>

std::unordered_map<std::string, TokenType> Lexer::keywords = {
    {"int", TokenType::INT},
    {"bool", TokenType::BOOL},
    {"void", TokenType::VOID},
    {"return", TokenType::RETURN},
    {"if", TokenType::IF},
    {"while", TokenType::WHILE},
    {"true", TokenType::TRUE},
    {"false", TokenType::FALSE}
};

Lexer::Lexer(const std::string& src) : source(src) {}

bool Lexer::isAtEnd() const {
    return pos >= source.length();
}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return source[pos];
}

char Lexer::advance() {
    return source[pos++];
}

Token Lexer::makeToken(TokenType type, const std::string& lexeme) {
    return Token(type, lexeme, line);
}

void Lexer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        if (c == ' ' || c == '\r' || c == '\t')
            advance();
        else if (c == '\n') {
            line++;
            advance();
        }
        else if (c == '/' && pos + 1 < source.length() && source[pos+1] == '/') {
            // comment until end of line
            while (!isAtEnd() && peek() != '\n') advance();
        }
        else break;
    }
}

Token Lexer::identifier() {
    size_t start = pos - 1;
    while (!isAtEnd() && (isalnum(peek()) || peek() == '_'))
        advance();

    std::string text = source.substr(start, pos - start);

    if (keywords.count(text))
        return makeToken(keywords[text], text);

    return makeToken(TokenType::IDENT, text);
}

Token Lexer::number() {
    size_t start = pos - 1;
    while (!isAtEnd() && isdigit(peek()))
        advance();

    std::string text = source.substr(start, pos - start);
    return makeToken(TokenType::NUMBER, text);
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (!isAtEnd()) {
        skipWhitespace();
        if (isAtEnd()) break;

        char c = advance();

        switch (c) {

        case '+': tokens.push_back(makeToken(TokenType::PLUS, "+")); break;
        case '-': tokens.push_back(makeToken(TokenType::MINUS, "-")); break;
        case '*': tokens.push_back(makeToken(TokenType::STAR, "*")); break;
        case '/': tokens.push_back(makeToken(TokenType::SLASH, "/")); break;
        case ';': tokens.push_back(makeToken(TokenType::SEMI, ";")); break;
        case ',': tokens.push_back(makeToken(TokenType::COMMA, ",")); break;
        case '(': tokens.push_back(makeToken(TokenType::LPAREN, "(")); break;
        case ')': tokens.push_back(makeToken(TokenType::RPAREN, ")")); break;
        case '{': tokens.push_back(makeToken(TokenType::LBRACE, "{")); break;
        case '}': tokens.push_back(makeToken(TokenType::RBRACE, "}")); break;

        case '!':
            if (peek() == '=') { advance(); tokens.push_back(makeToken(TokenType::NEQ, "!=")); }
            else tokens.push_back(makeToken(TokenType::NOT, "!"));
            break;

        case '=':
            if (peek() == '=') { advance(); tokens.push_back(makeToken(TokenType::EQ, "==")); }
            else tokens.push_back(makeToken(TokenType::ASSIGN, "="));
            break;

        case '<':
            if (peek() == '=') { advance(); tokens.push_back(makeToken(TokenType::LE, "<=")); }
            else tokens.push_back(makeToken(TokenType::LT, "<"));
            break;

        case '>':
            if (peek() == '=') { advance(); tokens.push_back(makeToken(TokenType::GE, ">=")); }
            else tokens.push_back(makeToken(TokenType::GT, ">"));
            break;

        case '&':
            if (peek() == '&') { advance(); tokens.push_back(makeToken(TokenType::AND, "&&")); }
            break;

        case '|':
            if (peek() == '|') { advance(); tokens.push_back(makeToken(TokenType::OR, "||")); }
            break;

        default:
            if (isalpha(c) || c == '_')
                tokens.push_back(identifier());
            else if (isdigit(c))
                tokens.push_back(number());
            else
                tokens.push_back(makeToken(TokenType::UNKNOWN, std::string(1, c)));
        }
    }

    tokens.push_back(makeToken(TokenType::END_OF_FILE, ""));
    return tokens;
}
