#include "astPrinter.h"


namespace ast
{
        void Printer::operator()(unsigned int n) const { std::cout << n; }
	void Printer::operator()(int n) const {std::cout<<n;}

	void Printer::operator()(std::string s) const
	{
		std::cout<<"variable "<<s;
	}

        void Printer::operator()(Operation const& x) const
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

        void Printer::operator()(Signed_ const& x) const
        {
            boost::apply_visitor(*this, x.operand_);
            switch (x.sign)
            {
                case '-': std::cout << " neg"; break;
                case '+': std::cout << " pos"; break;
            }
        }

	void Printer::operator()(Comparison const& x) const
	{
		(*this)(x.lhs);
		std::cout<<x.op;
		(*this)(x.rhs);
	}

	void Printer::operator()(Print const& x) const
	{
		std::cout<<"Print ";
		(*this)(x.val);
		std::cout<<std::endl;
	}

	void Printer::operator()(VarDecl const& x) const
	{
		std::cout<<"(Variable Declaration: name= "<<x.name<<" value= (";
		if(x.value)
			(*this)(*(x.value));
		std::cout<<')';
	}

	void Printer::operator()(ArrDecl const& x) const
	{
		std::cout<<"(Array Declaration: name= "<<x.name<<')'<<std::endl;
	}

	void Printer::operator()(ArrValue const& x) const
	{
		std::cout<<"(Array Value: name= "<<x.name;
		(*this)(x.idx);
		std::cout<<')'<<std::endl;
	}

	void Printer::operator()(Conditional const& x) const
	{
		std::cout<<"IF ";
		(*this)(x.condition);
		std::cout<<std::endl;
		(*this)(x.tBody);

		if(x.fBody)
		{
			std::cout<<"\n ELSE \n";
			(*this)(*(x.fBody));
		}
	}

	void Printer::operator()(Assignment const& x) const
	{
		std::cout<<"Assigning value(";
		(*this)(x.value);
		std::cout<<") to "<<x.name<<std::endl;
	}

	void Printer::operator()(CopyValue const& x) const
	{
		std::cout<<"Copying "<<x.from<<" to "<<x.to<<std::endl;
	}

	void Printer::operator()(AssignmentArr const& x) const
	{
		std::cout<<"Assigning value(";
		(*this)(x.value);
		std::cout<<") to ";
		(*this)(x.id);
		std::cout<<std::endl;
	}

	void Printer::operator()(Expr const& x) const
	{
		boost::apply_visitor(*this, x.first);
		for(const Operation& o : x.rest)
		{
			std::cout<<' ';
			(*this)(o);
		}
	}

	void Printer::operator()(WhileLoop const& x) const
	{
		std::cout<<"While loop (";
		(*this)(x.condition);
		std::cout<<")\n";
		(*this)(x.body);
	}

	void Printer::operator()(Statement const& x) const
	{
		boost::apply_visitor(*this, x);
	}

        void Printer::operator()(Program const& x) const
        {
            for (Statement const& stmt : x.stmts)
            {
                std::cout << "Statement: ";
                (*this)(stmt);
		std::cout<<'\n';
            }
        }

	void Printer::operator()(Return const& x) const
	{
		std::cout<<"return ";
		apply_visitor(*this,x.value);
	}

	void Printer::operator()(FunctionDecl const& x) const
	{
		std::cout<<"Function declaration\n";
		std::cout<<"name:"<<x.name;
		std::cout<<"\nargs:\n";
		for(const auto& a: x.args)
			boost::apply_visitor(*this, a);

		(*this)(x.body);
	}

	void Printer::operator()(FunctionCall const& x) const
	{
		std::cout<<"Function call\n";
		std::cout<<x.name<<'(';
		for(size_t i = 0; i < x.params.size(); ++i)
		{
			boost::apply_visitor(*this,x.params[i]);
			std::cout<<", ";
		}
		std::cout<<")\n";

	}

	void Printer::operator()(std::list<Statement> const& x) const
	{
		std::cout<<"\n{\n";
		for(Statement const& stmt : x)
		{
			(*this)(stmt);
		}
		std::cout<<"\n}\n";
	}

}









