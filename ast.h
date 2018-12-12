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
using boost::variant;

	struct Signed_;
	struct Expr;
	struct WhileLoop;
	struct Conditional;
	struct ArrValue;
	struct FunctionDecl;
	struct FunctionCall;
	struct ArrDecl;
	struct VarDecl;

	using argument = x3::variant<Expr, std::string>;
	using argumentDecl = x3::variant<ArrDecl, VarDecl>;

        struct Operand : x3::variant<
              unsigned int
	      , std::string //name of a variable
              , x3::forward_ast<Signed_>
              , x3::forward_ast<Expr>
	      , x3::forward_ast<ArrValue>
	      , x3::forward_ast<FunctionCall>
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

	struct Comparison
	{
		Expr lhs;
		char op;
		Expr rhs;
	};

	struct Assignment
	{
		Assignment() = default;
		Assignment(std::string name, Expr value) : name(name), value(value) {};
		std::string name;
		Expr value;
	};

	struct CopyValue
	{
		CopyValue() = default;
		CopyValue(std::string to, std::string from) : to(to), from(from) {};
		std::string to;
		std::string from;
	};

	struct ArrValue
	{
		std::string name;
		Expr idx;
	};

	struct AssignmentArr
	{
		ArrValue id;
		Expr value;
	};

	struct Print 
	{
		boost::variant<std::string, unsigned int, ArrValue> val;
	};

	struct VarDecl
	{
            std::string type;
            std::string name;
	    optional<Expr> value;
	};

	struct ArrDecl
	{
            std::string type;
            std::string name;
	    //init list
	};

	struct Return
	{
		variant<Expr, std::string> value;
	};

	struct FunctionCall
	{
		std::string name;
		std::vector<argument> args; 
	};

        struct Statement : x3::variant<
	        Return,
		VarDecl,
		ArrDecl,
		x3::forward_ast<FunctionDecl>,
		FunctionCall,
		Print,
		CopyValue,
		Assignment,
		AssignmentArr,
		x3::forward_ast<WhileLoop>,
		x3::forward_ast<Conditional>,
		Expr
	>
	{
		using base_type::base_type;
		using base_type::operator=;
        };

	struct FunctionBody 
	{
		std::vector<Statement> b; //stupid, but will work...
	}; //kind-of-an alias for body to allow proper ADL function overload

	struct FunctionDecl
	{
		std::string type;
		std::string name;
		std::vector<argumentDecl> args;
		FunctionBody body;
		//std::list<Statement> body;
	};

	struct Conditional
	{
		Comparison condition;
		std::list<Statement> tBody;
		optional<std::list<Statement>> fBody;
	};

	struct WhileLoop
	{
		Comparison condition;
		std::list<Statement> body;
	};

	struct Program {
	    std::list<Statement> stmts; 
	};

} 

BOOST_FUSION_ADAPT_STRUCT(ast::Signed_,
    sign, operand_
)

BOOST_FUSION_ADAPT_STRUCT(ast::Operation,
    operator_, operand_
)

BOOST_FUSION_ADAPT_STRUCT(ast::Return, value)

BOOST_FUSION_ADAPT_STRUCT(ast::Expr, 
		first, rest
		)

BOOST_FUSION_ADAPT_STRUCT(ast::Comparison,
		lhs, op, rhs 
)

BOOST_FUSION_ADAPT_STRUCT(ast::WhileLoop,
		condition, body
		)

BOOST_FUSION_ADAPT_STRUCT(ast::VarDecl, 
		type, name, value
		)
	
BOOST_FUSION_ADAPT_STRUCT(ast::ArrDecl, 
		type, name
		)

BOOST_FUSION_ADAPT_STRUCT(ast::FunctionDecl,
	       	type, name, args, body
		)

BOOST_FUSION_ADAPT_STRUCT(ast::FunctionCall,
	       	name, args
		)

BOOST_FUSION_ADAPT_STRUCT(ast::ArrValue, 
		name, idx
		)

BOOST_FUSION_ADAPT_STRUCT(ast::Conditional, 
		condition, tBody, fBody
		)

BOOST_FUSION_ADAPT_STRUCT(ast::Assignment, 
		name, value
		)

BOOST_FUSION_ADAPT_STRUCT(ast::CopyValue, 
		to, from
		)

BOOST_FUSION_ADAPT_STRUCT(ast::AssignmentArr, 
		id, value
		)

BOOST_FUSION_ADAPT_STRUCT(ast::Print, val)


BOOST_FUSION_ADAPT_STRUCT(ast::Program, stmts)

#endif
