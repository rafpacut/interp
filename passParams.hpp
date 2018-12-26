#ifndef PASS_PARAMS_H
#define PASS_PARAMS_H

#include "eval.h"
#include "ast.h"
#include <vector>

namespace ast{

class PassParameters
{
	public:
	PassParameters(Eval& eval) : eval(eval) {};

	void operator()(VarDecl& a, int value)
	{
		(eval)(a);
		eval.env.assignValue(a.name, value);
	}

	void operator()(ArrDecl& a, std::vector<int>& value)
	{
		(eval)(a);
		eval.env.assignValue(a.name, value);
	}

	void operator()(VarDecl&, std::vector<int>&)
	{
		throw std::runtime_error("In function call: initializing an int with an array.");
	}

	void operator()(ArrDecl&, int&)
	{
		throw std::runtime_error("In function call: initializing an array with an int.");
	}

	private:
	Eval& eval;
};
}

#endif
