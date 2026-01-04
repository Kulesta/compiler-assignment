
#include "symbol.h"

void SymbolTable::enterScope() {
    scopes.push_back({});
}

void SymbolTable::exitScope() {
    scopes.pop_back();
}

bool SymbolTable::declare(const std::string& name,
                          const std::string& type) {
    auto& current = scopes.back();
    if (current.count(name)) return false;
    current[name] = type;
    return true;
}

bool SymbolTable::isDeclared(const std::string& name) const {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it)
        if (it->count(name)) return true;
    return false;
}

std::string SymbolTable::getType(const std::string& name) const {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it)
        if (it->count(name)) return it->at(name);
    return "";
}

bool SymbolTable::declareFunction(const std::string& name,
                                 const FunctionInfo& info) {
    if (functions.count(name))
        return false;

    functions[name] = info;
    return true;
}

bool SymbolTable::hasFunction(const std::string& name) const {
    return functions.count(name);
}

FunctionInfo SymbolTable::getFunction(const std::string& name) const {
    return functions.at(name);
}

