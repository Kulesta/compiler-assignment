
#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

using namespace std;

class Parser {
public:
    explicit Parser(const vector<Token>& tokens);

    vector<StmtPtr> parse();

private:
    const vector<Token>& tokens;
    size_t pos = 0;

    bool isAtEnd() const;
    const Token& peek() const;
    const Token& advance();
    bool match(TokenType type);
    const Token& expect(TokenType type, const string& msg);

    // declarations
    StmtPtr declaration();
    StmtPtr varDecl();
    StmtPtr functionDecl(Token returnType, Token name);

    // statements
    StmtPtr statement();
    StmtPtr returnStmt();
    StmtPtr block();

    // expressions
    ExprPtr expression();
    ExprPtr assignment();
    ExprPtr logicOr();
    ExprPtr logicAnd();
    ExprPtr equality();
    ExprPtr comparison();
    ExprPtr term();
    ExprPtr factor();
    ExprPtr unary();
    ExprPtr primary();
};

#endif
