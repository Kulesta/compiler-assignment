#ifndef SYMBOL_H
#define SYMBOL_H

#include <unordered_map>
#include <vector>
#include <string>

// Function metadata
struct FunctionInfo {
    std::string returnType;
    std::vector<std::string> paramTypes;
};

class SymbolTable {
public:
    // Scope handling
    void enterScope();
    void exitScope();

    // Variables
    bool declare(const std::string& name, const std::string& type);
    bool isDeclared(const std::string& name) const;
    std::string getType(const std::string& name) const;

    // Functions (NEW)
    bool declareFunction(const std::string& name,
                         const FunctionInfo& info);
    bool hasFunction(const std::string& name) const;
    FunctionInfo getFunction(const std::string& name) const;

private:
    // Variable scopes
    std::vector<
        std::unordered_map<std::string, std::string>
    > scopes;

    // Function table (global)
    std::unordered_map<std::string, FunctionInfo> functions;
};

#endif
