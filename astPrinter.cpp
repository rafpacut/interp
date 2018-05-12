#include <iostream>
#include "ast.cpp"

namespace ast
{
    struct printer
    {
        typedef void result_type;

	void operator()(varDecl const& x) const
	{
		std::cout<<"Variable Declaration: "<<x.name<<" = "<<x.value<<'\n';
	}

	void operator()(Expr const& x) const
	{
		std::cout<<"Expression: "<<x.val<<'\n';
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
