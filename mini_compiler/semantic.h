#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include "symbol.h"

class SemanticAnalyzer {
public:
    void analyze(const std::vector<StmtPtr>& program);

private:
    SymbolTable symbols;

    // Track current function context
    std::string currentFunctionName;
    std::string currentReturnType;
    bool hasReturn = false;

    void analyzeStmt(const StmtPtr& stmt);
    std::string analyzeExpr(const ExprPtr& expr);


};

#endif
