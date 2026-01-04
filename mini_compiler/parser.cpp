#include "parser.h"
#include <stdexcept>

// ---------------- constructor ----------------
Parser::Parser(const std::vector<Token>& tokens)
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

const Token& Parser::expect(TokenType type, const std::string& msg) {
    if (peek().type == type)
        return advance();

    throw std::runtime_error(
        "Parser error at line " +
        std::to_string(peek().line) + ": " + msg);
}


// ---------------- entry ----------------
std::vector<StmtPtr> Parser::parse() {
    std::vector<StmtPtr> program;

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

        return std::make_shared<VarDecl>(
            typeToken.lexeme, name.lexeme);
    }

    return statement();
}


// ---------------- function declaration ----------------
StmtPtr Parser::functionDecl(Token returnType, Token name) {

    auto func = std::make_shared<FunctionDecl>();
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
        std::dynamic_pointer_cast<BlockStmt>(block());

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

    return std::make_shared<ExprStmt>(expr);
}

StmtPtr Parser::returnStmt() {
    ExprPtr value = nullptr;

    if (!match(TokenType::SEMI)) {
        value = expression();
        expect(TokenType::SEMI,
            "Expected ';' after return value");
    }

    return std::make_shared<ReturnStmt>(value);
}

StmtPtr Parser::block() {
    auto blk = std::make_shared<BlockStmt>();

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

        if (auto var = std::dynamic_pointer_cast<VarExpr>(expr)) {
            return std::make_shared<AssignExpr>(var->name, value);
        }

        throw std::runtime_error("Invalid assignment target");
    }

    return expr;
}


// ---------------- arithmetic ----------------
ExprPtr Parser::term() {
    ExprPtr expr = factor();

    while (peek().type == TokenType::PLUS ||
           peek().type == TokenType::MINUS) {

        std::string op = advance().lexeme;
        ExprPtr right = factor();
        expr = std::make_shared<BinaryExpr>(op, expr, right);
    }

    return expr;
}

ExprPtr Parser::factor() {
    ExprPtr expr = unary();

    while (peek().type == TokenType::STAR ||
           peek().type == TokenType::SLASH) {

        std::string op = advance().lexeme;
        ExprPtr right = unary();
        expr = std::make_shared<BinaryExpr>(op, expr, right);
    }

    return expr;
}

ExprPtr Parser::unary() {
    if (match(TokenType::MINUS)) {
        return std::make_shared<UnaryExpr>("-", unary());
    }

    return primary();
}


// ---------------- primary & calls ----------------
ExprPtr Parser::primary() {

    if (match(TokenType::TRUE))
        return std::make_shared<BoolExpr>(true);

    if (match(TokenType::FALSE))
        return std::make_shared<BoolExpr>(false);

    if (peek().type == TokenType::NUMBER)
        return std::make_shared<NumberExpr>(advance().lexeme);

    if (peek().type == TokenType::IDENT) {
        Token name = advance();

        if (match(TokenType::LPAREN)) {
            std::vector<ExprPtr> args;

            if (!match(TokenType::RPAREN)) {
                do {
                    args.push_back(expression());
                } while (match(TokenType::COMMA));

                expect(TokenType::RPAREN, "Expected ')'");
            }

            return std::make_shared<CallExpr>(
                name.lexeme, args);
        }

        return std::make_shared<VarExpr>(name.lexeme);
    }

    expect(TokenType::LPAREN, "Expected '('");
    ExprPtr expr = expression();
    expect(TokenType::RPAREN, "Expected ')'");
    return expr;
}
