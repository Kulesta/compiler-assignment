
/*
Assign-3 Semantic Analysis
9.	Type-check function parameters and return types.
You will implement semantic checks that validate function parameter types,
argument matching during calls, and correct return type usage. The goal is to detect mismatched or missing returns.

 */

#include <iostream>
#include <string>
#include "lexer.h"
#include "parser.h"
#include "semantic.h"

void runTest(const std::string& title, const std::string& source) {
    std::cout << "\n=====================================\n";
    std::cout << title << "\n";
    std::cout << "-------------------------------------\n";
    std::cout << source << "\n";
    std::cout << "-------------------------------------\n";

    try {
        // LEXER
        Lexer lexer(source);
        auto tokens = lexer.tokenize();
        std::cout << "Lexer: PASSED\n";

        // PARSER
        Parser parser(tokens);
        auto ast = parser.parse();
        std::cout << "Parser: PASSED\n";

        // SEMANTIC
        SemanticAnalyzer semantic;
        semantic.analyze(ast);
        std::cout << "Semantic: PASSED\n";
    }
    catch (const std::exception& e) {
        std::cout << "❌ ERROR: " << e.what() << "\n";
    }
}

int main() {

    // =====================================================
    // VALID PROGRAM
    // =====================================================
    runTest("VALID PROGRAM",
        R"(
        int add(int a, int b) {
            int c;
            c = a + b;
            return c;
        }

        int main() {
            int x;
            x = add(10, 20);
            return x;
        }
    )");

    // =====================================================
    // UNDEFINED VARIABLE
    // =====================================================
    runTest("UNDEFINED VARIABLE",
        R"(
        int main() {
            return y;
        }
    )");

    // =====================================================
    // WRONG ARGUMENT COUNT
    // =====================================================
    runTest("WRONG ARGUMENT COUNT",
        R"(
        int add(int a, int b) {
            return a + b;
        }

        int main() {
            return add(5);
        }
    )");

    // =====================================================
    // ARGUMENT TYPE MISMATCH
    // =====================================================
    runTest("ARGUMENT TYPE MISMATCH",
        R"(
        int add(int a, int b) {
            return a + b;
        }

        int main() {
            return add(1, true);
        }
    )");

    // =====================================================
    // MISSING RETURN STATEMENT
    // =====================================================
    runTest("MISSING RETURN",
        R"(
        int bad(int x) {
            int y;
            y = x + 1;
        }
    )");

    // =====================================================
    // VOID FUNCTION RETURNS VALUE
    // =====================================================
    runTest("VOID RETURN WITH VALUE",
        R"(
        void f() {
            return 5;
        }
    )");

    // =====================================================
    // ASSIGNMENT TO NON-VARIABLE
    // =====================================================
    runTest("INVALID ASSIGNMENT TARGET",
        R"(
        int main() {
            5 = 10;
            return 0;
        }
    )");

    // =====================================================
    // FUNCTION CALL USED AS VALUE
    // =====================================================
    runTest("FUNCTION CALL EXPRESSION",
        R"(
        int square(int x) {
            return x * x;
        }

        int main() {
            int y;
            y = square(5);
            return y;
        }
    )");

    return 0;
}
