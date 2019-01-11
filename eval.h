#ifndef EVAL_H_
#define EVAL_H_

#include <map>
#include <iostream>
#include <boost/optional.hpp>
#include <stack>
#include "ast.h"
#include "env.hpp"
#include "envPrinter.cpp"

namespace ast{

	using boost::optional;
	using boost::apply_visitor;
	using boost::get;

	struct Eval
	{
		Eval() = default;
		Eval(bool debug) : debugOn(debug) {};

		basicType operator()(unsigned int n);
		basicType operator()(int n) ;
		basicType operator()(std::string s);
		basicType operator()(Operation const& x, basicType lhs);
		basicType operator()(Signed_ const& x);
		basicType operator()(VarDecl const& x);
		basicType operator()(ArrDecl const& x);
		basicType operator()(FunctionDecl const& x);
		basicType operator()(FunctionCall const& x);
		basicType operator()(Return const& x);
		basicType operator()(ArrValue const& x);
		basicType operator()(Assignment const& x);
		basicType operator()(AssignmentArr const& x);
		basicType operator()(Print const& x);
		basicType operator()(Comparison const& x);
		basicType operator()(Expr const& x);
		basicType operator()(Statement const& x);
		basicType operator()(WhileLoop const& x);
		basicType operator()(Conditional const& x);
		basicType operator()(std::list<Statement> const& x);


		//basicType processFBody(std::list<Statement>);
		std::vector<basicType> evalParams(paramVector params);
		FunctionDecl getFunction(std::string fName);
		void passParameters(FunctionDecl&, std::vector<basicType>& argValues);

		bool returnStatementEvald = false;
		bool debugOn = false;


		envPrinter printEnv;
		Printer printAST;

		Environment env;
		std::stack<Environment> callStack;
	};
}

#endif
