#ifndef astPrinter_H
#define astPrinter_H


#include <iostream>
#include <boost/optional.hpp>
#include "ast.h"



namespace ast
{
	struct printer
	{
		using boost::optional;

		void operator()(unsigned int) const;
		void operator()(int) const;
		void operator()(std::string) const;
		void operator()(Operation const&) const;
		void operator()(Signed_ const&) const;
		void operator()(VarDecl const&) const;
		void operator()(Assignment const&) const;
		void operator()(Print const&) const;
		void operator()(Expr const&) const;
		void operator()(Statement const&) const;
		void operator()(Program const&) const;
		void operator()(WhileLoop const&) const;
		void operator()(Conditional const&) const;
	};
}

#endif
