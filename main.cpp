#include <iostream>
#include <iomanip>

#include "ast.h"
#include "astPrinter.cpp"
//#include "eval.cpp"

#include <map>
///////////////////////////////////////////////////////////////////////////
//  The AST evaluator
///////////////////////////////////////////////////////////////////////////
namespace ast{

    struct eval
    {
        typedef int result_type;


        int operator()(unsigned int n) const { return n; }
	
	int operator()(int n) const { return n; }

	int operator()(std::string s) const
	{
		int val;
		try{
			val = vars.at(s);
		}
		catch(const std::out_of_range& e)
		{
			std::cout<<"Using undeclared variable "<<s;
		}
		return val;
	}

        int operator()(operation const& x, int lhs) const
        {
            int rhs = boost::apply_visitor(*this, x.operand_);
            switch (x.operator_)
            {
                case '+': return lhs + rhs;
                case '-': return lhs - rhs;
                case '*': return lhs * rhs;
                case '/': return lhs / rhs;
            }
            BOOST_ASSERT(0);
            return 0;
        }

        int operator()(signed_ const& x) const
        {
            int rhs = boost::apply_visitor(*this, x.operand_);
            switch (x.sign)
            {
                case '-': return -rhs;
                case '+': return +rhs;
            }
            BOOST_ASSERT(0);
            return 0;
        }

	int operator()(varDecl const& x) const
	{
		int value = (*this)(x.value);
		vars.emplace(x.name, value);

		return 0;
	}

	int operator()(Expr const& x) const
	{
		int value = boost::apply_visitor(*this, x.first);
		for(const operation& o: x.rest)
		{
			value = (*this)(o, value);
		}
		return value;
	}

	int operator()(statement const& x) const
	{
		return boost::apply_visitor(*this, x);
	}

        int operator()(program const& x) const
        {
	    int state = 1;
            for (statement const& stmt : x.stmts)
            {
                state = (*this)(stmt);
            }
            return state;
        }
    };
}

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
		|   (char_('+') > name)
		|   (char_('-') > name)
                )
            ;
 
        const auto factor_def 
        =
                x3::uint_
            |   name		
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
	ast::eval eval;

        It iter = str.begin(), end = str.end();
        if (phrase_parse(iter, end, parser, x3::space, program)) {
            std::cout << "Parsing succeeded\n";
	    print(program);
	    std::cout<<'\n'<<eval(program)<<'\n';
        }
        else
            std::cout << "Parsing failed\n";

        if (iter != end)
            std::cout << "Remaining unparsed: " << std::quoted(std::string(iter, end), '\'') << "\n";
    }
}
