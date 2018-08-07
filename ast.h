#ifndef AST_H_
#define AST_H_


#include <list>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/adapted.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/optional.hpp>


namespace x3 = boost::spirit::x3;
using boost::optional;

namespace ast {

	struct signed_;
	struct Expr;
	struct whileLoop;

        struct operand : x3::variant<
              unsigned int
	      , std::string //name of a variable
              , x3::forward_ast<signed_>
              , x3::forward_ast<Expr>
            >
        {
            using base_type::base_type;
            using base_type::operator=;
        };

        struct signed_
        {
            char sign;
            operand operand_;
        };

        struct operation
        {
            char operator_;
            operand operand_;
        };

	struct Expr
	{
		operand first; 
		std::list<operation> rest;
	};

	struct assignment
	{
		std::string name;
		Expr value;
	};

	struct print 
	{
		std::string name;
	};

	struct varDecl
	{
            std::string type;
            std::string name;
	    optional<Expr> value;
	};

        struct statement : x3::variant<
		varDecl,
		print,
		assignment,
		x3::forward_ast<whileLoop>,
		Expr	
	>
	{
		using base_type::base_type;
		using base_type::operator=;
        };

	struct whileLoop
	{
		Expr condition;
		std::list<statement> body;
	};

	struct program {
	    std::list<statement> stmts; 
	};

} 

BOOST_FUSION_ADAPT_STRUCT(ast::signed_,
    sign, operand_
)

BOOST_FUSION_ADAPT_STRUCT(ast::whileLoop,
		condition, body
		)

BOOST_FUSION_ADAPT_STRUCT(ast::operation,
    operator_, operand_
)

BOOST_FUSION_ADAPT_STRUCT(ast::Expr, 
		first, rest
		)

BOOST_FUSION_ADAPT_STRUCT(ast::varDecl, 
		type, name, value
		)

BOOST_FUSION_ADAPT_STRUCT(ast::assignment, 
		name, value
		)

BOOST_FUSION_ADAPT_STRUCT(ast::print, name)


BOOST_FUSION_ADAPT_STRUCT(ast::program, stmts)

#endif
