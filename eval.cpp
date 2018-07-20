#include "eval.h"


namespace ast{

        int Eval::operator()(unsigned int n) const { return n; }
	
	int Eval::operator()(int n) const { return n; }

	int Eval::operator()(std::string s) const
	{
		optional<int> val;
		try
		{
			val = vars.at(s);
			if(!val)
			{
				std::cout<<"Using uninitialized variable " + s<<std::endl;
				throw std::runtime_error("Using uninitialized variable " + s);
			}
		}
		catch(const std::out_of_range& e)
		{
			std::cout<<"Using undeclared variable "<<s<<std::endl;
			throw;
		}
		return *val;
	}

        int Eval::operator()(operation const& x, int lhs) const
        {
		int rhs;
		try
		{
		    rhs = boost::apply_visitor(*this, x.operand_);
		}
		catch(...){throw;}
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
		int rhs;
		try
		{
		    rhs = boost::apply_visitor(*this, x.operand_);
		}
		catch(...){throw;}
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
		try
		{
			value = boost::apply_visitor(*this, x.first);
		}
		catch(...) { throw; }

		for(const operation& o: x.rest)
		{
			try
			{
				value = (*this)(o, value);
			}
			catch(...){throw;}
		}
		return value;
	}

	int Eval::operator()(statement const& x) 
	{
		int value = 0;
		try
		{
			value = boost::apply_visitor(*this, x);
		}
		catch(...){throw;}
		return value;
	}

        int Eval::operator()(program const& x) 
        {
	    int state = 0;
            for (statement const& stmt : x.stmts)
            {
		    try
		    {
			state = (*this)(stmt);
		    }
		    catch(...)
		    {
			    return 1;
		    }
            }
            return state;
        }
    }
