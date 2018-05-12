#include <iostream>
#include <iomanip>

#include "ast.h"
#include "astPrinter.cpp"
#include "grammar.cpp"

int main() {
    std::cout << "Type an expression...or [q or Q] to quit\n\n";

    using It = std::string::const_iterator;

    for (std::string str; std::getline(std::cin, str);) {
        if (str.empty() || str[0] == 'q' || str[0] == 'Q')
            break;

        auto &parser = grammar::program;
        ast::program program; // Our program (AST)
	ast::printer print;

        It iter = str.begin(), end = str.end();
        if (phrase_parse(iter, end, parser, x3::space, program)) {
            std::cout << "Parsing succeeded\n";
	    print(program);
        }
        else
            std::cout << "Parsing failed\n";

        if (iter != end)
            std::cout << "Remaining unparsed: " << std::quoted(std::string(iter, end), '\'') << "\n";
    }
}
