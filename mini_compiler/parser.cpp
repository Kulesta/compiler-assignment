#include "parser.h"
#include <stdexcept>

using namespace std;

// ---------------- constructor ----------------
Parser::Parser(const vector<Token>& tokens)
    : tokens(tokens), pos(0) {}


// ---------------- utilities ----------------
bool Parser::isAtEnd() const {
    return peek().type == TokenType::END_OF_FILE;
}

const Token& Parser::peek() const {
    return tokens[pos];
}

const Token& Parser::advance() {
    if (!isAtEnd()) pos++;
    return tokens[pos - 1];
}

bool Parser::match(TokenType type) {
    if (peek().type == type) {
        advance();
        return true;
    }
    return false;
}

const Token& Parser::expect(TokenType type, const string& msg) {
    if (peek().type == type)
        return advance();

    throw runtime_error(
        "Parser error at line " +
        to_string(peek().line) + ": " + msg);
}


// ---------------- entry ----------------
vector<StmtPtr> Parser::parse() {
    vector<StmtPtr> program;

    while (!isAtEnd()) {
        program.push_back(declaration());
    }

    return program;
}


// ---------------- declarations ----------------
StmtPtr Parser::declaration() {

    if (match(TokenType::INT) ||
        match(TokenType::BOOL) ||
        match(TokenType::VOID)) {

        Token typeToken = tokens[pos - 1];
        Token name = expect(TokenType::IDENT,
            "Expected identifier after type");

        // function declaration
        if (peek().type == TokenType::LPAREN) {
            return functionDecl(typeToken, name);
        }

        // variable declaration
        expect(TokenType::SEMI,
            "Expected ';' after variable declaration");

        return make_shared<VarDecl>(
            typeToken.lexeme, name.lexeme);
    }

    return statement();
}


// ---------------- function declaration ----------------
StmtPtr Parser::functionDecl(Token returnType, Token name) {

    auto func = make_shared<FunctionDecl>();
    func->returnType = returnType.lexeme;
    func->name = name.lexeme;

    expect(TokenType::LPAREN, "Expected '(' after function name");

    if (!match(TokenType::RPAREN)) {
        do {
            Token t = expect(TokenType::INT,
                "Only int parameters supported");

            Token n = expect(TokenType::IDENT,
                "Expected parameter name");

            func->params.push_back({ t.lexeme, n.lexeme });

        } while (match(TokenType::COMMA));

        expect(TokenType::RPAREN, "Expected ')'");
    }

    expect(TokenType::LBRACE,
        "Expected '{' before function body");

    func->body =
        dynamic_pointer_cast<BlockStmt>(block());

    return func;
}


// ---------------- statements ----------------
StmtPtr Parser::statement() {

    if (match(TokenType::RETURN))
        return returnStmt();

    if (match(TokenType::LBRACE))
        return block();

    ExprPtr expr = expression();
    expect(TokenType::SEMI, "Expected ';' after expression");

    return make_shared<ExprStmt>(expr);
}

StmtPtr Parser::returnStmt() {
    ExprPtr value = nullptr;

    if (!match(TokenType::SEMI)) {
        value = expression();
        expect(TokenType::SEMI,
            "Expected ';' after return value");
    }

    return make_shared<ReturnStmt>(value);
}

StmtPtr Parser::block() {
    auto blk = make_shared<BlockStmt>();

    while (!isAtEnd() && !match(TokenType::RBRACE)) {
        blk->statements.push_back(declaration());
    }

    return blk;
}


// ---------------- expressions ----------------
ExprPtr Parser::expression() {
    return assignment();
}

ExprPtr Parser::assignment() {
    ExprPtr expr = term();

    if (match(TokenType::ASSIGN)) {
        ExprPtr value = assignment();

        if (auto var = dynamic_pointer_cast<VarExpr>(expr)) {
            return make_shared<AssignExpr>(var->name, value);
        }

        throw runtime_error("Invalid assignment target");
    }

    return expr;
}


// ---------------- arithmetic ----------------
ExprPtr Parser::term() {
    ExprPtr expr = factor();

    while (peek().type == TokenType::PLUS ||
           peek().type == TokenType::MINUS) {

        string op = advance().lexeme;
        ExprPtr right = factor();
        expr = make_shared<BinaryExpr>(op, expr, right);
    }

    return expr;
}

ExprPtr Parser::factor() {
    ExprPtr expr = unary();

    while (peek().type == TokenType::STAR ||
           peek().type == TokenType::SLASH) {

        string op = advance().lexeme;
        ExprPtr right = unary();
        expr = make_shared<BinaryExpr>(op, expr, right);
    }

    return expr;
}

ExprPtr Parser::unary() {
    if (match(TokenType::MINUS)) {
        return make_shared<UnaryExpr>("-", unary());
    }

    return primary();
}


// ---------------- primary & calls ----------------
ExprPtr Parser::primary() {

    if (match(TokenType::TRUE))
        return make_shared<BoolExpr>(true);

    if (match(TokenType::FALSE))
        return make_shared<BoolExpr>(false);

    if (peek().type == TokenType::NUMBER)
        return make_shared<NumberExpr>(advance().lexeme);

    if (peek().type == TokenType::IDENT) {
        Token name = advance();

        if (match(TokenType::LPAREN)) {
            vector<ExprPtr> args;

            if (!match(TokenType::RPAREN)) {
                do {
                    args.push_back(expression());
                } while (match(TokenType::COMMA));

                expect(TokenType::RPAREN, "Expected ')'");
            }

            return make_shared<CallExpr>(
                name.lexeme, args);
        }

        return make_shared<VarExpr>(name.lexeme);
    }

    expect(TokenType::LPAREN, "Expected '('");
    ExprPtr expr = expression();
    expect(TokenType::RPAREN, "Expected ')'");
    return expr;
}
