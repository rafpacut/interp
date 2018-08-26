#ifndef EVAL_H_
#define EVAL_H_

#include <map>
#include "ast.h"
#include "env.hpp"
#include <iostream>
#include <boost/optional.hpp>

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
		int operator()(Assignment const& x);
		int operator()(Print const& x) const;
		int operator()(Expr const& x) const;
		int operator()(Statement const& x);
		int operator()(Program const& x);
		int operator()(WhileLoop const& x);
		int operator()(Conditional const& x);

		Environment env;
	};
}

#endif
