#include "astPrinter.h"


namespace ast
{
        void printer::operator()(unsigned int n) const { std::cout << n; }
	void printer::operator()(int n) const {std::cout<<n;}

	void printer::operator()(std::string s) const
	{
		std::cout<<"variable "<<s;
	}

        void printer::operator()(Operation const& x) const
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

        void printer::operator()(Signed_ const& x) const
        {
            boost::apply_visitor(*this, x.operand_);
            switch (x.sign)
            {
                case '-': std::cout << " neg"; break;
                case '+': std::cout << " pos"; break;
            }
        }

	void printer::operator()(Print const& x) const
	{
		std::cout<<"Print "<<x.name<<std::endl;
	}

	void printer::operator()(VarDecl const& x) const
	{
		std::cout<<"(Variable Declaration: name= "<<x.name<<" value= (";
		if(x.value)
			(*this)(*(x.value));
		std::cout<<')';
	}

	void printer::operator()(Conditional const& x) const
	{
		std::cout<<"IF ";
		(*this)(x.condition);
		std::cout<<std::endl;
		for(statement const& stmt : x.tBody)
		{
			(*this)(stmt);
		}

		if(x.fBody)
		{
			std::cout<<"\n ELSE \n";
			for(statement const& stmt : (*x.fBody))
			{
				(*this)(stmt);
			}
		}
	}

	void printer::operator()(Assignment const& x) const
	{
		std::cout<<"Assigning value(";
		(*this)(x.value);
		std::cout<<") to "<<x.name<<std::endl;
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

	void printer::operator()(WhileLoop const& x) const
	{
		std::cout<<"While loop (";
		(*this)(x.condition);
		std::cout<<")\n{\n";
		for(statement const& stmt : x.body)
		{
			(*this)(stmt);
		}
		std::cout<<"\n}\n";
	}

	void printer::operator()(Statement const& x) const
	{
		boost::apply_visitor(*this, x);
	}

        void printer::operator()(Program const& x) const
        {
            for (statement const& stmt : x.stmts)
            {
                std::cout << "Statement: ";
                (*this)(stmt);
		std::cout<<'\n';
            }
        }
}
