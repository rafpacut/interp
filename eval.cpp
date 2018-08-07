#include "eval.h"


namespace ast{

        int Eval::operator()(unsigned int n) const { return n; }
	
	int Eval::operator()(int n) const { return n; }

	int Eval::operator()(std::string s) const
	{
		optional<int> val;
		val = vars.at(s);
		if(!val)
		{
			throw std::runtime_error("Using uninitialized variable " + s);
		}
		return *val;
	}

        int Eval::operator()(operation const& x, int lhs) const
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

        int Eval::operator()(signed_ const& x) const
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

	int Eval::operator()(varDecl const& x)
	{
		optional<int> value = boost::none;
		if(x.value)
			value = (*this)(*(x.value));
		vars.emplace(x.name, value);

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

	int Eval::operator()(whileLoop const& x)
	{
		int state=0;
		//while((*this)(x.condition))
		//{
		////	for(statement const& stmt : x.body)
		////	{
		////		state = (*this)(stmt);
		////	}
		//}
		return state;
	}

	int Eval::operator()(statement const& x) 
	{
		int value = 0;
		value = boost::apply_visitor(*this, x);
		return value;
	}

        int Eval::operator()(program const& x) 
        {
	    int state = 0;
            for (statement const& stmt : x.stmts)
            {
		state = (*this)(stmt);
            }
            return state;
        }
    }
