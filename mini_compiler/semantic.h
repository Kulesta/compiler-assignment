#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include "symbol.h"

using namespace std;

class SemanticAnalyzer {
public:
    void analyze(const vector<StmtPtr>& program);

private:
    SymbolTable symbols;

    // Track current function context
    string currentFunctionName;
    string currentReturnType;
    bool hasReturn = false;

    void analyzeStmt(const StmtPtr& stmt);
    string analyzeExpr(const ExprPtr& expr);


};

#endif
