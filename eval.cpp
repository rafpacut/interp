#include "eval.h"


namespace ast{

        int Eval::operator()(unsigned int n) const { return n; }
	
	int Eval::operator()(int n) const { return n; }

	int Eval::operator()(std::string s) const
	{
		return env.getValue(s);
	}

        int Eval::operator()(Operation const& x, int lhs) const
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

        int Eval::operator()(Signed_ const& x) const
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

	int Eval::operator()(Print const& x) const
	{
		optional<int> val = vars.at(x.name);
		if(val)
			std::cout<<*val<<std::endl;
		else
			std::cout<<x.name<<" was not yet initialized."<<std::endl;

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

	int Eval::operator()(std::list<Statement> body)
	{
		int state = 0;
		for(Statement const& stmt: body)
		{
			state += (*this)(stmt);
		}
		return state;
	}

	int Eval::operator()(Conditional const& x)
	{
		if((*this)(x.condition))
			(*this)(x.tBody);
		else if(x.fBody)
			(*this)(x.fBody);

		return 0;
	}

	int Eval::operator()(Assignment const& x)
	{
		int value = (*this)(x.value);
		env.assignValue(x.name, value);

		return 0;
	}

	int Eval::operator()(Expr const& x) const
	{
		int value;
		value = boost::apply_visitor(*this, x.first);

		for(const operation& o: x.rest)
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
		return value;
	}

        int Eval::operator()(Program const& x) 
        {
	    int state = 0;
            for (statement const& stmt : x.stmts)
            {
		state = (*this)(stmt);
            }
            return state;
        }
    }
