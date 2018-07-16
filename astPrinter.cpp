#include "astPrinter.h"


namespace ast
{
        void printer::operator()(unsigned int n) const { std::cout << n; }
	void printer::operator()(int n) const {std::cout<<n;}

	void printer::operator()(std::string s) const
	{
		std::cout<<"variable "<<s;
	}

        void printer::operator()(operation const& x) const
        {
            boost::apply_visitor(*this, x.operand_);
            switch (x.operator_)
            {
                case '+': std::cout << " add"; break;
                case '-': std::cout << " subt"; break;
                case '*': std::cout << " mult"; break;
                case '/': std::cout << " div"; break;
            }
        }

        void printer::operator()(signed_ const& x) const
        {
            boost::apply_visitor(*this, x.operand_);
            switch (x.sign)
            {
                case '-': std::cout << " neg"; break;
                case '+': std::cout << " pos"; break;
            }
        }

	void printer::operator()(varDecl const& x) const
	{
		std::cout<<"(Variable Declaration: name= "<<x.name<<" value= (";
		(*this)(x.value);
		std::cout<<')';
	}

	void printer::operator()(Expr const& x) const
	{
		boost::apply_visitor(*this, x.first);
		for(const operation& o : x.rest)
		{
			std::cout<<' ';
			(*this)(o);
		}
	}

	void printer::operator()(statement const& x) const
	{
		boost::apply_visitor(*this, x);
	}

        void printer::operator()(program const& x) const
        {
            for (statement const& stmt : x.stmts)
            {
                std::cout << "Statement: ";
                (*this)(stmt);
		std::cout<<'\n';
            }
        }
}
