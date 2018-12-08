#include "eval.h"
#include <string>
#include <iterator>
#include "FParamPass.cpp"


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

	int Eval::operator()(const FunctionCall& x) 
	{
		//obtain called function
		using Function = FunctionDecl;
		std::string targetFName = x.name;
		auto fIt = std::find_if(std::begin(env.functions), std::end(env.functions), 
				[&targetFName](const Function& f){return f.name == targetFName;});

		if(fIt == std::end(env.functions))
			throw std::runtime_error("No function named "+x.name);

		Function f = *fIt;

		//create new environment
		callStack.push(env); 
		env = Environment(env);

		//Declare all arguments
		if(x.args.size() != f.args.size())
			throw std::runtime_error("Expected "+std::to_string(f.args.size())+" arguments, got "+std::to_string(x.args.size()));

		FunctionPassParamsVisitor passParams(*this);
		
		for(size_t i = 0; i < x.args.size(); i++) 
			boost::apply_visitor(passParams, f.args[i], x.args[i]);


		int state = (*this)(f.body); //Until 'return'
		env = callStack.top();
		callStack.pop();
		return state;
	}
}
















