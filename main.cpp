#include <iostream>
#include <iomanip>

#include "ast.h"
#include "astPrinter.cpp"
namespace grammar 
{
	using namespace x3;

        x3::rule<class expression_, ast::Expr> const expression("expression");
        x3::rule<class term_, ast::Expr> const term("term");
        x3::rule<class factor_, ast::operand> const factor("factor");
	x3::rule<class statement_, ast::statement> const statement("statement");
	x3::rule<class program_, std::list<ast::statement> > const program("program");
	x3::rule<class name_, std::string> const name("name");
	x3::rule<class varDecl_, ast::varDecl> const varDecl("varDecl");

	const auto name_def 
	= x3::alpha >> *x3::alnum;

	const auto varDecl_def
	= name >> '=' >> expression;

        const auto expression_def 
        =
            term
            >> *(   (char_('+') > term)
                |   (char_('-') > term)
                )
            ;
 
        const auto factor_def 
        =
                x3::uint_
            |   ('(' > expression > ')')
            |   (char_('-') > factor)
            |   (char_('+') > factor)
            ;
 
        const auto term_def 
        =
            factor
            >> *(   (char_('*') > factor)
                |   (char_('/') > factor)
                )
            ;
 
	const auto statement_def 
	= 
	    varDecl
	    | expression
	    ;

	const auto program_def 
	= +(statement >> ';');


        BOOST_SPIRIT_DEFINE(
            expression
          , term
          , factor
	  , name
	  , varDecl
	  , statement
	  , program
        );

}


int main() {
    std::cout << "Type an expression...or [q or Q] to quit\n\n";

    using It = std::string::const_iterator;

    for (std::string str; std::getline(std::cin, str);) {
        if (str.empty() || str[0] == 'q' || str[0] == 'Q')
            break;

        auto &parser = grammar::program;
        ast::program program; 
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
