#ifndef AST_H_
#define AST_H_


#include <list>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/adapted.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/optional.hpp>



namespace ast {

namespace x3 = boost::spirit::x3;
using boost::optional;

	struct Signed_;
	struct Expr;
	struct WhileLoop;
	struct Conditional;

        struct Operand : x3::variant<
              unsigned int
	      , std::string //name of a variable
              , x3::forward_ast<Signed_>
              , x3::forward_ast<Expr>
            >
        {
            using base_type::base_type;
            using base_type::operator=;
        };

        struct Signed_
        {
            char sign;
            Operand operand_;
        };

        struct Operation
        {
            char operator_;
            Operand operand_;
        };

	struct Expr
	{
		Operand first; 
		std::list<Operation> rest;
	};

	struct Assignment
	{
		std::string name;
		Expr value;
	};

	struct Print 
	{
		std::string name;
	};

	struct VarDecl
	{
            std::string type;
            std::string name;
	    optional<Expr> value;
	};

        struct Statement : x3::variant<
		VarDecl,
		Print,
		Assignment,
		x3::forward_ast<WhileLoop>,
		x3::forward_ast<Conditional>,
		Expr	
	>
	{
		using base_type::base_type;
		using base_type::operator=;
        };

	struct Conditional
	{
		Expr condition;
		std::list<Statement> tBody;
		optional<std::list<Statement>> fBody;
	};

	struct WhileLoop
	{
		Expr condition;
		std::list<Statement> body;
	};

	struct Program {
	    std::list<Statement> stmts; 
	};

} 

BOOST_FUSION_ADAPT_STRUCT(ast::Signed_,
    sign, operand_
)

BOOST_FUSION_ADAPT_STRUCT(ast::WhileLoop,
		condition, body
		)

BOOST_FUSION_ADAPT_STRUCT(ast::Operation,
    operator_, operand_
)

BOOST_FUSION_ADAPT_STRUCT(ast::Expr, 
		first, rest
		)

BOOST_FUSION_ADAPT_STRUCT(ast::VarDecl, 
		type, name, value
		)

BOOST_FUSION_ADAPT_STRUCT(ast::Conditional, 
		condition, tBody, fBody
		)

BOOST_FUSION_ADAPT_STRUCT(ast::Assignment, 
		name, value
		)

BOOST_FUSION_ADAPT_STRUCT(ast::Print, name)


BOOST_FUSION_ADAPT_STRUCT(ast::Program, stmts)

#endif
