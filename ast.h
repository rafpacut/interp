#ifndef AST_H_
#define AST_H_


#include <list>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
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
	struct ArraySize;
	struct VarDecl;

	using param = Expr;
	using paramVector = optional<std::vector<param>>;
	using argument = x3::variant<ArrDecl, VarDecl>;
	using basicType = variant<int,std::vector<int>>;

        struct Operand : x3::variant<
              unsigned int
	      , std::string //name of a variable
              , x3::forward_ast<Signed_>
              , x3::forward_ast<Expr>
	      , x3::forward_ast<ArrValue>
	      , x3::forward_ast<FunctionCall>
	      , x3::forward_ast<ArraySize>
            >, x3::position_tagged
        {
            using base_type::base_type;
            using base_type::operator=;
        };

        struct Signed_ : x3::position_tagged
        {
            char sign;
            Operand operand_;
        };

        struct Operation : x3::position_tagged
        {
            char operator_;
            Operand operand_;
        };

	struct Expr : x3::position_tagged
	{
		Operand first; 
		std::list<Operation> rest;
	};

	struct Comparison : x3::position_tagged
	{
		Expr lhs;
		std::string op;
		Expr rhs;
	};

	struct Assignment : x3::position_tagged
	{
		std::string name;
		Expr value;
	};

	struct ArraySize : x3::position_tagged
	{
		std::string name;
	};

	struct PushBack : x3::position_tagged
	{
		std::string name;
		Expr value;
	};

	struct ArrValue : x3::position_tagged
	{
		std::string name;
		Expr idx;
	};

	struct AssignmentArr : x3::position_tagged
	{
		ArrValue id;
		Expr value;
	};

	struct Print  : x3::position_tagged
	{
		Expr val;
	};

	struct VarDecl : x3::position_tagged
	{
            std::string type;
            std::string name;
	    optional<Expr> value;
	};

	struct Return : x3::position_tagged
	{
		Expr value;
	};

	struct FunctionCall : x3::position_tagged
	{
		std::string name;
		paramVector params; 
	};

	struct ArrDecl : x3::position_tagged
	{
            std::string type;
            std::string name;
	    optional<Expr> initValue;
	};

        struct Statement : x3::variant<
	        Return,
		VarDecl,
		ArrDecl,
		x3::forward_ast<FunctionDecl>,
		FunctionCall,
		Print,
		PushBack,
		ArraySize,
		Assignment,
		AssignmentArr,
		x3::forward_ast<WhileLoop>,
		x3::forward_ast<Conditional>,
		Expr
	>, x3::position_tagged
	{
		using base_type::base_type;
		using base_type::operator=;
        };

	struct FunctionDecl : x3::position_tagged
	{
		std::string type;
		std::string name;
		optional<std::vector<argument>> args;
		std::list<Statement> body;

		bool operator<(const FunctionDecl& rhs) const
		{
			return this->name < rhs.name;
		}
	};

	struct Conditional : x3::position_tagged
	{
		Comparison condition;
		std::list<Statement> tBody;
		optional<std::list<Statement>> fBody;
	};

	struct WhileLoop : x3::position_tagged
	{
		Comparison condition;
		std::list<Statement> body;
	};
} 
#endif
