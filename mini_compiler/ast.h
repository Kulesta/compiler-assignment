
#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>

// Forward declarations
struct Expr;
struct Stmt;

using ExprPtr = std::shared_ptr<Expr>;
using StmtPtr = std::shared_ptr<Stmt>;

//
// -------- EXPRESSIONS --------
//
struct Expr {
    virtual ~Expr() = default;
};

struct NumberExpr : Expr {
    std::string value;
    explicit NumberExpr(std::string v) : value(std::move(v)) {}
};

struct BoolExpr : Expr {
    bool value;
    explicit BoolExpr(bool v) : value(v) {}
};

struct VarExpr : Expr {
    std::string name;
    explicit VarExpr(std::string n) : name(std::move(n)) {}
};

struct AssignExpr : Expr {
    std::string name;
    ExprPtr value;

    AssignExpr(std::string n, ExprPtr v)
        : name(std::move(n)), value(std::move(v)) {}
};



struct BinaryExpr : Expr {
    std::string op;
    ExprPtr left;
    ExprPtr right;

    BinaryExpr(std::string o, ExprPtr l, ExprPtr r)
        : op(std::move(o)), left(std::move(l)), right(std::move(r)) {}
};

struct UnaryExpr : Expr {
    std::string op;
    ExprPtr expr;

    UnaryExpr(std::string o, ExprPtr e)
        : op(std::move(o)), expr(std::move(e)) {}
};

struct CallExpr : Expr {
    std::string callee;
    std::vector<ExprPtr> args;

    CallExpr(std::string c, std::vector<ExprPtr> a)
        : callee(std::move(c)), args(std::move(a)) {}
};

//
// -------- STATEMENTS --------
//
struct Stmt {
    virtual ~Stmt() = default;
};

struct VarDecl : Stmt {
    std::string type;
    std::string name;

    VarDecl(std::string t, std::string n)
        : type(std::move(t)), name(std::move(n)) {}
};

struct ExprStmt : Stmt {
    ExprPtr expr;
    explicit ExprStmt(ExprPtr e) : expr(std::move(e)) {}
};

struct ReturnStmt : Stmt {
    ExprPtr expr;   // may be null for void return

    explicit ReturnStmt(ExprPtr e)
        : expr(std::move(e)) {}
};

struct BlockStmt : Stmt {
    std::vector<StmtPtr> statements;
};

struct FunctionDecl : Stmt {
    std::string returnType;
    std::string name;

    struct Param {
        std::string type;
        std::string name;
    };

    std::vector<Param> params;
    std::shared_ptr<BlockStmt> body;
};

#endif
