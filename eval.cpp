#include "eval.h"
#include <string>
#include <iostream>
#include <iterator>
#include "LambdaVisitor.hpp"


namespace ast{
        basicType Eval::operator()(unsigned int n)  { return n; }
	
	basicType Eval::operator()(int n)  { return n; }

	basicType Eval::operator()(std::string s) 
	{
		return env.getValue(s);
	}

        basicType Eval::operator()(Operation const& x, basicType operand) 
        {
	    int rhs = get<int>(boost::apply_visitor(*this, x.operand_));
	    int lhs = get<int>(operand);
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

        basicType Eval::operator()(Signed_ const& x) 
        {
		int rhs = get<int>(boost::apply_visitor(*this, x.operand_));
		switch (x.sign)
		{
		    case '-': return -rhs;
		    case '+': return +rhs;
		}
		BOOST_ASSERT(0);
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
					std::copy(val.begin(), val.end(), std::ostream_iterator<int>(std::cout,", "));
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
				std::cout<<"bad get. Did not retrieve an array.?!\n";
			}
		}
		else
			val = boost::none;

		env.declare(x.name, val);

		return 0;
	}

	basicType Eval::operator()(const std::list<Statement>& body)
	{
		env.createScope();
		for(Statement const& stmt: body)
			(*this)(stmt);
		env.deleteScope();

		return 0;
	}

	basicType Eval::operator()(Conditional const& x)
	{
		if(get<int>((*this)(x.condition)))
			(*this)(x.tBody);
		else if(x.fBody)
			(*this)(*x.fBody);

		return 0;
	}

	basicType Eval::operator()(Assignment const& x)
	{
		int value = get<int>((*this)(x.value));
		env.assignValue(x.name, value);

		return 0;
	}

	basicType Eval::operator()(CopyValue const& x)
	{
		env.copyValue(x.from, x.to);
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
		while(get<int>((*this)(x.condition)))
		{
			(*this)(x.body);
		}

		return 0;
	}

	basicType Eval::operator()(Statement const& x) 
	{
		if(debugOn)
		{
			printAST(x);
			printEnv(env);
		}

		return apply_visitor(*this, x);
	}

        basicType Eval::operator()(Program const& x) 
        {
            for (Statement const& stmt : x.stmts)
		(*this)(stmt);

            return 0;
        }

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

	basicType Eval::processFBody(std::list<Statement> body)
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

		std::vector<basicType> paramVals;
		std::transform(x.params.begin(), x.params.end(), std::back_inserter(paramVals),
				[this](ast::param a) -> basicType 
				{
					return boost::apply_visitor(*this,a);
				});


		callStack.push(env); 
		env = Environment(env);

		passParameters(f.args, paramVals);

		basicType returnValue = processFBody(f.body);

		env = callStack.top();
		callStack.pop();

		return returnValue;
	}

	FunctionDecl Eval::getFunction(std::string fName)
	{
		auto fIt = std::find_if(std::begin(env.functions), std::end(env.functions), 
				[&fName](const FunctionDecl& f){return f.name == fName;});

		if(fIt == std::end(env.functions))
			throw std::runtime_error("No function named "+fName);

		return *fIt;
	}

	void Eval::passParameters(std::vector<ast::argument>& argDecls, std::vector<basicType>& params)
	{
		if(params.size() != argDecls.size())
			throw std::runtime_error("Expected "+std::to_string(argDecls.size())+" arguments, got "+std::to_string(params.size()));

		for(size_t i = 0; i < argDecls.size(); i++) 
		{
			boost::apply_visitor(LambdaVisitor(
						[this, &params, i](VarDecl& a)
						{
							int value;
							try{
								value = get<int>(params[i]);
							}catch(boost::bad_get& e)
							{
								throw std::runtime_error("In functionCall {name}: expects an integer parameter");
							}
							(*this)(a);
							env.assignValue(a.name, value);
						},
						[this, &params, i](const ArrDecl& a)
						{
							std::vector<int> value;
							try{
								 value = get<std::vector<int>>(params[i]);
							}catch(boost::bad_get& e)
							{
								throw std::runtime_error("In functionCall {name}: expects an array parameter");
							}
							(*this)(a);
							env.assignValue(a.name, value);
						}),
					argDecls[i]);
		}
	}

}
















