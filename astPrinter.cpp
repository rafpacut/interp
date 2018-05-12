#include <iostream>
#include "ast.cpp"

namespace ast
{
    ///////////////////////////////////////////////////////////////////////////
    //  The AST Printer
    ///////////////////////////////////////////////////////////////////////////
    struct printer
    {
        typedef void result_type;


        //void operator()(operation const& x) const
        //{
        //    boost::apply_visitor(*this, x.operand_);
        //    switch (x.operator_)
        //    {
        //        case '+': std::cout << " add"; break;
        //        case '-': std::cout << " subt"; break;
        //        case '*': std::cout << " mult"; break;
        //        case '/': std::cout << " div"; break;
        //    }
        //}

        //void operator()(signed_ const& x) const
        //{
        //    boost::apply_visitor(*this, x.operand_);
        //    switch (x.sign)
        //    {
        //        case '-': std::cout << " neg"; break;
        //        case '+': std::cout << " pos"; break;
        //    }
        //}

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

    ///////////////////////////////////////////////////////////////////////////
    //  The AST evaluator
    ///////////////////////////////////////////////////////////////////////////
    //struct eval
    //{
    //    typedef int result_type;


    //    int operator()(nil) const { BOOST_ASSERT(0); return 0; }
    //    int operator()(unsigned int n) const { return n; }

    //    int operator()(operation const& x, int lhs) const
    //    {
    //        int rhs = boost::apply_visitor(*this, x.operand_);
    //        switch (x.operator_)
    //        {
    //            case '+': return lhs + rhs;
    //            case '-': return lhs - rhs;
    //            case '*': return lhs * rhs;
    //            case '/': return lhs / rhs;
    //        }
    //        BOOST_ASSERT(0);
    //        return 0;
    //    }

    //    int operator()(signed_ const& x) const
    //    {
    //        int rhs = boost::apply_visitor(*this, x.operand_);
    //        switch (x.sign)
    //        {
    //            case '-': return -rhs;
    //            case '+': return +rhs;
    //        }
    //        BOOST_ASSERT(0);
    //        return 0;
    //    }

    //    int operator()(statement const& x) const
    //    {
    //    	(*this)(x);
    //    	return 0;
    //    }

    //    int operator()(program const& x) const
    //    {
    //        int state = 1;
    //        for (statement const& stmt : x.stmts)
    //        {
    //            state = (*this)(stmt);
    //        }
    //        return state;
    //    }
    //};
}
