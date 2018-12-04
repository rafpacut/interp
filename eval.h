#ifndef EVAL_H_
#define EVAL_H_

#include <map>
#include <iostream>
#include <boost/optional.hpp>
#include "ast.h"
#include "env.hpp"
#include "envPrinter.cpp"

namespace ast{

	using boost::optional;

	struct Eval
	{
		int operator()(unsigned int n);
		int operator()(int n) ;
		int operator()(std::string s);
		int operator()(Operation const& x, int lhs);
		int operator()(Signed_ const& x);
		int operator()(VarDecl const& x);
		int operator()(ArrDecl const& x);
		int operator()(FunctionDecl const& x);
		int operator()(ArrValue const& x) const;
		int operator()(Assignment const& x);
		int operator()(CopyValue const& x);
		int operator()(AssignmentArr const& x);
		int operator()(Print const& x);
		int operator()(Comparison const& x);
		int operator()(Expr const& x);
		int operator()(Statement const& x);
		int operator()(Program const& x);
		int operator()(WhileLoop const& x);
		int operator()(Conditional const& x);
		int operator()(std::list<Statement> const& x);

		Environment env;
		envPrinter printEnv;
	};
}

#endif
