
#include "symbol.h"

using namespace std;

void SymbolTable::enterScope() {
    scopes.push_back({});
}

void SymbolTable::exitScope() {
    scopes.pop_back();
}

bool SymbolTable::declare(const string& name,
                          const string& type) {
    auto& current = scopes.back();
    if (current.count(name)) return false;
    current[name] = type;
    return true;
}

bool SymbolTable::isDeclared(const string& name) const {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it)
        if (it->count(name)) return true;
    return false;
}

string SymbolTable::getType(const string& name) const {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it)
        if (it->count(name)) return it->at(name);
    return "";
}

bool SymbolTable::declareFunction(const string& name,
                                 const FunctionInfo& info) {
    if (functions.count(name))
        return false;

    functions[name] = info;
    return true;
}

bool SymbolTable::hasFunction(const string& name) const {
    return functions.count(name);
}

FunctionInfo SymbolTable::getFunction(const string& name) const {
    return functions.at(name);
}

