#include "eval.h"
#include <string>
#include <iostream>
#include <iterator>
#include "passParams.hpp"
#include "LambdaVisitor.hpp"
#include "safeArithmetic.hpp"


namespace ast{
        basicType Eval::operator()(unsigned int n)  { return n; }
	
	basicType Eval::operator()(int n)  { return n; }

	basicType Eval::operator()(std::string s) 
	{
		return env.getValue(s);
	}

        basicType Eval::operator()(Operation const& x, basicType operand) 
        {
		int rhs, lhs;
		try{
			rhs = get<int>(boost::apply_visitor(*this, x.operand_));
			lhs = get<int>(operand);
		}
		catch(boost::bad_get&)
		{
			throw std::runtime_error("Non-integer cannot be a part of an expression");
		}

		switch (x.operator_)
		{
			case '+': return safeArithmetic::add(lhs, rhs);
			case '-': return safeArithmetic::sub(lhs, rhs);
			case '*': return safeArithmetic::mult(lhs, rhs);
			case '/': return safeArithmetic::div(lhs, rhs);
		}

		BOOST_ASSERT(0);

		std::string errorMsg = "Unknown integer operator ";
		errorMsg.push_back(x.operator_);
		throw std::runtime_error(errorMsg);
        }


        basicType Eval::operator()(Signed_ const& x) 
        {
		int rhs = get<int>(boost::apply_visitor(*this, x.operand_));
		switch (x.sign)
		{
		    case '-': return -rhs;
		    case '+': return +rhs;
		}
		BOOST_ASSERT(0);
		throw std::runtime_error("Unknown sign. Did you mean '-' or '+'?");
		return 0;
        }

	basicType Eval::operator()(Comparison const& x) 
	{
		int lhs = get<int>((*this)(x.lhs));
		int rhs = get<int>((*this)(x.rhs));

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

	basicType Eval::operator()(ArrValue const& x) 
	{
		int idx = get<int>((*this)( x.idx));
		return env.getValue(x.name, idx);
	}

	basicType Eval::operator()(Print const& x) 
	{
		basicType val = (*this)(x.val);
		apply_visitor(LambdaVisitor(
				[](const int val)
				{ std::cout<<val<<'\n';},
				[](const std::vector<int>& val)
				{
					std::copy(val.begin(), val.end(), std::ostream_iterator<int>(std::cout," "));
				}),
				val);
		return 0;
	}

	basicType Eval::operator()(VarDecl const& x)
	{
		optional<int> value = boost::none;
		if(x.value)
			value = get<int>((*this)(*(x.value)));

		env.declare(x.name, value);

		return 0;
	}

	basicType Eval::operator()(ArrDecl const& x)
	{
		optional<std::vector<int>> val;
		if(x.initValue)
		{
			try{
				val = get<std::vector<int>>((*this)(*(x.initValue)));
			}
			catch(boost::bad_get& e)
			{
				std::cout<<"In array declaration: initial value is not of array type.";
			}
		}
		else
			val = boost::none;

		env.declare(x.name, val);

		return 0;
	}

	basicType Eval::operator()(Conditional const& x)
	{
		basicType res;
		env.createScope();

		if(get<int>((*this)(x.condition)))
			res = (*this)(x.tBody);
		else if(x.fBody)
			res = (*this)(*x.fBody);

		env.deleteScope();

		return res;
	}

	basicType Eval::operator()(Assignment const& x)
	{
		basicType value = (*this)(x.value);
		apply_visitor(LambdaVisitor(
				[this, &x](const int v){ env.assignValue(x.name, v);},
				[this, &x](const std::vector<int> v){ env.assignValue(x.name,v);}),
				value);
		return 0;
	}

	basicType Eval::operator()(AssignmentArr const& x)
	{
		int value = get<int>((*this)(x.value));
		int id = get<int>((*this)(x.id.idx));
		env.assignValue(x.id.name, value, id);

		return 0;
	}

	basicType Eval::operator()(Expr const& x) 
	{
		basicType value = apply_visitor(*this, x.first);

		for(const Operation& o: x.rest)
			value = (*this)(o, value); 

		return value;
	}

	basicType Eval::operator()(WhileLoop const& x)
	{
		basicType res;
		while(get<int>((*this)(x.condition)))
		{
			env.createScope();

			res = (*this)(x.body);

			env.deleteScope();
		}

		return res;
	}

	basicType Eval::operator()(Statement const& x) 
	{
		//Should print AST, ask to step/continue and print Env after stmt eval
		//Now it prints AST, prints Env before change and evals stmt.
		if(debugOn)
		{
			printAST(x);
			printEnv(env);
		}


		basicType tmp = apply_visitor(*this, x);

		if(debugOn)
			printEnv(env);

		return tmp;
	}

        //basicType Eval::operator()(Program const& x) 
        //{
        //    for (Statement const& stmt : x.stmts)
	//	(*this)(stmt);

        //    return 0;
        //}

	basicType Eval::operator()(FunctionDecl const& x)
	{
		env.declare(x);
		return 0;
	}

	basicType Eval::operator()(Return const& x)
	{
		returnStatementEvald = true;
		return (*this)(x.value);
	}

	basicType Eval::operator()(const std::list<Statement>& body)
	{
		returnStatementEvald = false;
		basicType returnValue;
		for(auto& stmt: body)
		{
			returnValue = (*this)(stmt);
			if(returnStatementEvald) break;
		}

		return returnValue;
	}

	basicType Eval::operator()(const FunctionCall& x) 
	{
		FunctionDecl f = getFunction(x.name);

		std::vector<basicType> paramVals = evalParams(x.params);

		callStack.push(env); 
		env = std::move(env);

		passParameters(f, paramVals);

		basicType returnValue = (*this)(f.body);

		env = callStack.top();
		callStack.pop();

		return returnValue;
	}

	std::vector<basicType> Eval::evalParams(paramVector paramsOpt)
	{
		std::vector<basicType> paramVals;
		if(paramsOpt)
		{
			auto& params = *(paramsOpt);
			std::transform(params.begin(), params.end(), std::back_inserter(paramVals),
					[this](param a) -> basicType 
					{
						return (*this)(a);
					});
		}
		return paramVals;
	}

	FunctionDecl Eval::getFunction(std::string fName)
	{
		auto fIt = std::find_if(std::begin(env.functions), std::end(env.functions), 
				[&fName](const FunctionDecl& f){return f.name == fName;});

		if(fIt == std::end(env.functions))
			throw std::runtime_error("No function named "+fName);

		return *fIt;
	}

	void Eval::passParameters(FunctionDecl& fun, std::vector<basicType>& params)
	{
		if(fun.args)
		{
			auto& argumentDecls = *(fun.args);
			if(params.size() != argumentDecls.size())
				throw std::runtime_error("Expected "+std::to_string(argumentDecls.size())+" argument(s), got "+std::to_string(params.size()));

			PassParameters passParams(*this);
			for(size_t i = 0; i < argumentDecls.size(); i++) 
			{
				apply_visitor(passParams, argumentDecls[i], params[i]);
			}
		}
		else if(!params.empty())
		{
			throw std::runtime_error("Function "+fun.name+" expects no arguments, " +std::to_string(params.size()) + " provided.");
		}

	}

}
















