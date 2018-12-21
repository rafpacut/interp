#include "eval.h"
#include <string>
#include <iterator>
#include "LambdaVisitor.hpp"


namespace ast{
        int Eval::operator()(unsigned int n)  { return n; }
	
	int Eval::operator()(int n)  { return n; }

	int Eval::operator()(std::string s) 
	{
		return env.getValue(s);
	}

        int Eval::operator()(Operation const& x, int lhs) 
        {
	    int rhs = boost::apply_visitor(*this, x.operand_);
            switch (x.operator_)
            {
                case '+': return lhs + rhs;
                case '-': return lhs - rhs;
                case '*': return lhs * rhs;
                case '/': return lhs / rhs;
            }
            BOOST_ASSERT(0);
            return 0;
        }

        int Eval::operator()(Signed_ const& x) 
        {
		int rhs = boost::apply_visitor(*this, x.operand_);
		switch (x.sign)
		{
		    case '-': return -rhs;
		    case '+': return +rhs;
		}
		BOOST_ASSERT(0);
		return 0;
        }

	int Eval::operator()(Comparison const& x) 
	{
		int lhs = (*this)(x.lhs);
		int rhs = (*this)(x.rhs);

		if(x.op == "<")
			return lhs < rhs;
		if(x.op == "<=")
			return lhs <= rhs;
		if(x.op == ">")
			return lhs > rhs;
		if(x.op == ">=")
			return lhs >= rhs;
		if(x.op == "==")
			return lhs == rhs;
		if(x.op == "!=")
			return lhs != rhs;

		throw std::runtime_error("Expected logical comparison operator, got " + x.op);
	}

	int Eval::operator()(ArrValue const& x) 
	{
		int idx = (*this)( x.idx);
		return env.getValue(x.name, idx);
	}

	int Eval::operator()(Print const& x) 
	{
		int val = (*this)(x.val);
		std::cout<<val<<std::endl;

		return 0;
	}

	int Eval::operator()(VarDecl const& x)
	{
		optional<int> value = boost::none;
		if(x.value)
			value = (*this)(*(x.value));

		env.declare(x.name, value);

		return 0;
	}

	int Eval::operator()(ArrDecl const& x)
	{
		if(x.initValue)
		{
			apply_visitor(LambdaVisitor(
						[this, &x](const FunctionCall& fCall) 
						{
							std::vector<int> val;
							(*this)(fCall);
							env.getReturn(val);
							env.declare(x.name, val);
						},
						[this, &x](const std::string& name)
						{
							env.declare(x.name, optional<std::vector<int>>(boost::none));
							(*this)(CopyValue(x.name, name));
						}),
						*x.initValue);
		}
		else
			env.declare(x.name, optional<std::vector<int>>(boost::none));

		return 0;
	}

	int Eval::operator()(const std::list<Statement>& body)
	{
		int state = 0;
		env.createScope();
		for(Statement const& stmt: body)
		{
			state += (*this)(stmt);
		}
		env.deleteScope();

		return state;
	}

	int Eval::operator()(Conditional const& x)
	{
		if((*this)(x.condition))
			(*this)(x.tBody);
		else if(x.fBody)
			(*this)(*x.fBody);

		return 0;
	}

	int Eval::operator()(Assignment const& x)
	{
		int value = (*this)(x.value);
		env.assignValue(x.name, value);

		return 0;
	}

	int Eval::operator()(CopyValue const& x)
	{
		env.copyValue(x.from, x.to);
		return 0;
	}

	int Eval::operator()(AssignmentArr const& x)
	{
		int value = (*this)(x.value);
		int id = (*this)(x.id.idx);
		env.assignValue(x.id.name, value, id);

		return 0;
	}

	int Eval::operator()(Expr const& x) 
	{
		int value;
		value = boost::apply_visitor(*this, x.first);

		for(const Operation& o: x.rest)
			value = (*this)(o, value);

		return value;
	}

	int Eval::operator()(WhileLoop const& x)
	{
		int state=0;
		while((*this)(x.condition))
		{
			state = (*this)(x.body);
		}

		return state;
	}

	int Eval::operator()(Statement const& x) 
	{
		if(debugOn)
		{
			printAST(x);
			printEnv(env);
		}

		int value = 0;
		value = boost::apply_visitor(*this, x);

		return value;
	}

        int Eval::operator()(Program const& x) 
        {
	    int state = 0;
            for (Statement const& stmt : x.stmts)
            {
		state = (*this)(stmt);
            }

            return state;
        }

	int Eval::operator()(FunctionDecl const& x)
	{
		env.declare(x);
		return 0;
	}

	int Eval::operator()(Return const& x)
	{
		returnStatementEvald = true;
		apply_visitor(LambdaVisitor(
					[this](const std::string& varName)
					{ env.markReturnedValue(varName); },
					[this](ast::Expr const& e)
					{
						int value = (*this)(e);
						env.markReturnedValue(value);
					}
				),
				x.value);

		return 0;
	}

	int Eval::processFBody(std::list<Statement> body)
	{
		returnStatementEvald = false;
		for(auto& stmt: body)
		{
			(*this)(stmt);
			if(returnStatementEvald) break;
		}

		return 0;
	}

	int Eval::operator()(const FunctionCall& x) 
	{
		FunctionDecl f = getFunction(x.name);

		std::vector<int> paramVals;
		std::transform(x.params.begin(), x.params.end(), std::back_inserter(paramVals),
				[this](ast::param a) -> int 
				{
					return boost::apply_visitor(*this,a);
				});


		callStack.push(env); 
		env = Environment(env);

		passParameters(f.args, paramVals);

		processFBody(f.body);

		//GET returned values from Environment
		//--------------------------------------------------
		//what if there was no return? Throw in env...
		int state;
	        env.getReturn(state);

		std::vector<int> retValue;
		env.getReturn(retValue);

		//Remove function Environment and use old one.
		env = callStack.top();
		callStack.pop();

		env.markReturnedValue(retValue);

		return state;
	}

	FunctionDecl Eval::getFunction(std::string fName)
	{
		auto fIt = std::find_if(std::begin(env.functions), std::end(env.functions), 
				[&fName](const FunctionDecl& f){return f.name == fName;});

		if(fIt == std::end(env.functions))
			throw std::runtime_error("No function named "+fName);

		return *fIt;
	}

	void Eval::passParameters(const std::vector<ast::argument>& argDecls, const std::vector<int>& params)
	{
		if(params.size() != argDecls.size())
			throw std::runtime_error("Expected "+std::to_string(argDecls.size())+" arguments, got "+std::to_string(params.size()));

		for(size_t i = 0; i < argDecls.size(); i++) 
		{
			boost::apply_visitor(LambdaVisitor(
						[this, &params, i](const VarDecl& a)
						{
							(*this)(a);
							this->env.assignValue(a.name, params[i]);
						},
						[](const ArrDecl&)
						{
							throw std::runtime_error("array passing not yet implemented");
						}),
					argDecls[i]);
		}
	}

}
















