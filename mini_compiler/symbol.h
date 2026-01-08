#ifndef SYMBOL_H
#define SYMBOL_H

#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

// Function metadata
struct FunctionInfo {
    string returnType;
    vector<string> paramTypes;
};

class SymbolTable {
public:
    // Scope handling
    void enterScope();
    void exitScope();

    // Variables
    bool declare(const string& name, const string& type);
    bool isDeclared(const string& name) const;
    string getType(const string& name) const;

    // Functions (NEW)
    bool declareFunction(const string& name,
                         const FunctionInfo& info);
    bool hasFunction(const string& name) const;
    FunctionInfo getFunction(const string& name) const;

private:
    // Variable scopes
    vector<
        unordered_map<string, string>
    > scopes;

    // Function table (global)
    unordered_map<string, FunctionInfo> functions;
};

#endif
