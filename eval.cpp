#include "eval.h"
#include "ast.h"


namespace ast{

        int Eval::operator()(unsigned int n) const { return n; }
	
	int Eval::operator()(int n) const { return n; }

	int Eval::operator()(std::string s) const
	{
		int val;
		try{
			val = vars.at(s);
		}
		catch(const std::out_of_range& e)
		{
			std::cout<<"Using undeclared variable "<<s;
		}
		return val;
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
		int value = (*this)(x.value);
		vars.emplace(x.name, value);

		return 0;
	}

	int Eval::operator()(Expr const& x) const
	{
		int value = boost::apply_visitor(*this, x.first);
		for(const operation& o: x.rest)
		{
			value = (*this)(o, value);
		}
		return value;
	}

	int Eval::operator()(statement const& x) 
	{
		return boost::apply_visitor(*this, x);
	}

        int Eval::operator()(program const& x) 
        {
	    int state = 1;
            for (statement const& stmt : x.stmts)
            {
                state = (*this)(stmt);
            }
            return state;
        }
    }
