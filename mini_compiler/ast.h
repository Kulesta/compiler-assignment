
#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>

// Forward declarations
struct Expr;
struct Stmt;

using namespace std;

using ExprPtr = shared_ptr<Expr>;
using StmtPtr = shared_ptr<Stmt>;

//
// -------- EXPRESSIONS --------
//
struct Expr {
    virtual ~Expr() = default;
};

struct NumberExpr : Expr {
    string value;
    explicit NumberExpr(string v) : value(move(v)) {}
};

struct BoolExpr : Expr {
    bool value;
    explicit BoolExpr(bool v) : value(v) {}
};

struct VarExpr : Expr {
    string name;
    explicit VarExpr(string n) : name(move(n)) {}
};

struct AssignExpr : Expr {
    string name;
    ExprPtr value;

    AssignExpr(string n, ExprPtr v)
        : name(move(n)), value(move(v)) {}
};



struct BinaryExpr : Expr {
    string op;
    ExprPtr left;
    ExprPtr right;

    BinaryExpr(string o, ExprPtr l, ExprPtr r)
        : op(move(o)), left(move(l)), right(move(r)) {}
};

struct UnaryExpr : Expr {
    string op;
    ExprPtr expr;

    UnaryExpr(string o, ExprPtr e)
        : op(move(o)), expr(move(e)) {}
};

struct CallExpr : Expr {
    string callee;
    vector<ExprPtr> args;

    CallExpr(string c, vector<ExprPtr> a)
        : callee(move(c)), args(move(a)) {}
};

//
// -------- STATEMENTS --------
//
struct Stmt {
    virtual ~Stmt() = default;
};

struct VarDecl : Stmt {
    string type;
    string name;

    VarDecl(string t, string n)
        : type(move(t)), name(move(n)) {}
};

struct ExprStmt : Stmt {
    ExprPtr expr;
    explicit ExprStmt(ExprPtr e) : expr(move(e)) {}
};

struct ReturnStmt : Stmt {
    ExprPtr expr;   // may be null for void return

    explicit ReturnStmt(ExprPtr e)
        : expr(move(e)) {}
};

struct BlockStmt : Stmt {
    vector<StmtPtr> statements;
};

struct FunctionDecl : Stmt {
    string returnType;
    string name;

    struct Param {
        string type;
        string name;
    };

    vector<Param> params;
    shared_ptr<BlockStmt> body;
};

#endif
