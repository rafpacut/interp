#include <iostream>
#include "ast.cpp"

namespace ast
{
    struct printer
    {
        typedef void result_type;

        void operator()(unsigned int n) const { std::cout << n; }
	void operator()(int n) const {std::cout<<n;}

        void operator()(operation const& x) const
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

        void operator()(signed_ const& x) const
        {
            boost::apply_visitor(*this, x.operand_);
            switch (x.sign)
            {
                case '-': std::cout << " neg"; break;
                case '+': std::cout << " pos"; break;
            }
        }

	void operator()(varDecl const& x) const
	{
		std::cout<<"Variable Declaration: name="<<x.name<<" value= "<<x.value<<'\n';
	}

	void operator()(Expr const& x) const
	{
		boost::apply_visitor(*this,x.first);
		for(const operation& o : x.rest)
		{
			std::cout << ' ';
			(*this)(o);
		}
		std::cout<<'\n';
	}

	void operator()(statement const& x) const
	{
		boost::apply_visitor(*this, x);
	}

        void operator()(program const& x) const
        {
            for (statement const& stmt : x.stmts)
            {
                std::cout << ' ';
                (*this)(stmt);
            }
        }
    };
}
