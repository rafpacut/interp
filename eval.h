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
		int operator()(unsigned int n) const;
		int operator()(int n) const ;
		int operator()(std::string s) const;
		int operator()(Operation const& x, int lhs) const;
		int operator()(Signed_ const& x) const;
		int operator()(VarDecl const& x);
		int operator()(ArrDecl const& x);
		int operator()(FunctionDecl const& x);
		int operator()(ArrValue const& x) const;
		int operator()(Assignment const& x);
		int operator()(AssignmentArr const& x);
		int operator()(Print const& x) const;
		int operator()(Comparison const& x) const;
		int operator()(Expr const& x) const;
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
