#include "eval.h"
#include <string>
#include <iterator>
#include "FParamPass.cpp"
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
		switch (x.op)
		{
			case '<': return lhs < rhs;
			case '>': return lhs > rhs;
		}
		return 0;
	}

	int Eval::operator()(ArrValue const& x) 
	{
		int idx = (*this)( x.idx);
		return env.getValue(x.name, idx);
	}

	int Eval::operator()(Print const& x) 
	{
		int val = boost::apply_visitor(*this, x.val);
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
		{
			value = (*this)(o, value);
		}
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
		int value = 0;
		value = boost::apply_visitor(*this, x);
		printEnv(env);
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
					[this](ast::Expr const& e)
					{
						int value = (*this)(e);
						env.markReturnedValue(value);
					},
					[this](const std::string& varName)
					{ env.markReturnedValue(varName); }
				),
				x.value);

		return 0;
	}

	int Eval::processFBody(std::list<Statement> body)
	{
		for(auto& stmt: body)
		{
			(*this)(stmt);
			if(returnStatementEvald) break;
		}

		returnStatementEvald = false;
		return 0;
	}


	int Eval::operator()(const FunctionCall& x) 
	{
		FunctionDecl f = getFunction(x.name);

		callStack.push(env); 
		env = Environment(env);

		passParameters(f.args, x.args);

		processFBody(x.body);

		//what if there was no return? Throw in env...
		int state = env.getReturnedValue();

		env = callStack.top();
		callStack.pop();

		//uh-oh: no can do! FunctionCall can either
		// return array and be a part of arrayAssignment, or
		// return an int

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

	void Eval::passParameters(const std::vector<ast::argumentDecl>& argDecls, const std::vector<ast::argument>& argValues)
	{
		if(argValues.size() != argDecls.size())
			throw std::runtime_error("Expected "+std::to_string(argDecls.size())+" arguments, got "+std::to_string(argValues.size()));

		FunctionPassParamsVisitor passParams(*this);
		
		for(size_t i = 0; i < argValues.size(); i++) 
			boost::apply_visitor(passParams, argDecls[i], argValues[i]);
	}

}
















