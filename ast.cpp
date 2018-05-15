#include "ast.h"

namespace ast {
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

	struct varDecl
	{
            std::string name;
	    Expr value;
	};

        struct statement : x3::variant<
		varDecl,
		Expr	
	>
	{
		using base_type::base_type;
		using base_type::operator=;
        };

    struct program {
        std::list<statement> stmts;
    };
} 

BOOST_FUSION_ADAPT_STRUCT(ast::signed_,
    sign, operand_
)

BOOST_FUSION_ADAPT_STRUCT(ast::operation,
    operator_, operand_
)

BOOST_FUSION_ADAPT_STRUCT(ast::Expr, 
		first, rest
		)

BOOST_FUSION_ADAPT_STRUCT(ast::varDecl, 
		name, value
		)

BOOST_FUSION_ADAPT_STRUCT(ast::program, stmts)
