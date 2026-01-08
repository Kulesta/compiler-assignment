#include "semantic.h"
#include <stdexcept>

using namespace std;

void SemanticAnalyzer::analyze(
    const vector<StmtPtr>& program) {

    symbols.enterScope();

    for (auto& stmt : program)
        analyzeStmt(stmt);

    symbols.exitScope();
}

void SemanticAnalyzer::analyzeStmt(const StmtPtr& stmt) {

    // ---------------- Variable Declaration ----------------
    if (auto v = dynamic_pointer_cast<VarDecl>(stmt)) {
        if (!symbols.declare(v->name, v->type))
            throw runtime_error(
                "Variable redeclared: " + v->name);
    }

    // ---------------- Function Declaration ----------------
    else if (auto f = dynamic_pointer_cast<FunctionDecl>(stmt)) {

        // Register function signature
        FunctionInfo info;
        info.returnType = f->returnType;

        for (auto& p : f->params)
            info.paramTypes.push_back(p.type);

        if (!symbols.declareFunction(f->name, info))
            throw runtime_error(
                "Function redeclared: " + f->name);

        // Save previous function context
        string prevReturn = currentReturnType;
        bool prevHasReturn = hasReturn;

        currentReturnType = f->returnType;
        hasReturn = false;

        // Enter function scope
        symbols.enterScope();
        for (auto& p : f->params)
            symbols.declare(p.name, p.type);

        analyzeStmt(f->body);

        symbols.exitScope();

        // Enforce return rule
        if (currentReturnType != "void" && !hasReturn)
            throw runtime_error(
                "Function '" + f->name + "' must return a value");

        // Restore context
        currentReturnType = prevReturn;
        hasReturn = prevHasReturn;
    }

    // ---------------- Block ----------------
    else if (auto b = dynamic_pointer_cast<BlockStmt>(stmt)) {
        symbols.enterScope();
        for (auto& s : b->statements)
            analyzeStmt(s);
        symbols.exitScope();
    }

    // ---------------- Return Statement ----------------
    else if (auto r = dynamic_pointer_cast<ReturnStmt>(stmt)) {

        hasReturn = true;

        if (currentReturnType == "void") {
            if (r->expr)
                throw runtime_error(
                    "Void function should not return a value");
        }
        else {
            if (!r->expr)
                throw runtime_error(
                    "Non-void function must return a value");

            string exprType = analyzeExpr(r->expr);
            if (exprType != currentReturnType)
                throw runtime_error(
                    "Return type mismatch: expected " +
                    currentReturnType + ", got " + exprType);
        }
    }
}

string SemanticAnalyzer::analyzeExpr(const ExprPtr& expr) {

    // ---------------- Variable Expression ----------------
    if (auto v = dynamic_pointer_cast<VarExpr>(expr)) {
        if (!symbols.isDeclared(v->name))
            throw runtime_error(
                "Undefined variable: " + v->name);

        return symbols.getType(v->name);
    }

    // ---------------- Binary Expression ----------------
    else if (auto b = dynamic_pointer_cast<BinaryExpr>(expr)) {
        string left = analyzeExpr(b->left);
        string right = analyzeExpr(b->right);

        if (left != "int" || right != "int")
            throw runtime_error(
                "Binary operator requires int operands");

        return "int";
    }

    // ---------------- Function Call ----------------
    else if (auto call = dynamic_pointer_cast<CallExpr>(expr)) {

    if (!symbols.hasFunction(call->callee))
        throw runtime_error(
            "Undefined function: " + call->callee);

    auto fn = symbols.getFunction(call->callee);

    if (call->args.size() != fn.paramTypes.size())
        throw runtime_error(
            "Function '" + call->callee +
            "' called with wrong number of arguments");

    for (size_t i = 0; i < call->args.size(); ++i) {
        string argType = analyzeExpr(call->args[i]);
        if (argType != fn.paramTypes[i])
            throw runtime_error(
                "Argument type mismatch in call to '" +
                call->callee + "'");
    }

    return fn.returnType;
}


    throw runtime_error("Unknown expression type");
}
