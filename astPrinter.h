#ifndef astPrinter_H
#define astPrinter_H


#include <iostream>
#include <boost/optional.hpp>
#include "ast.h"
#include <map>


using boost::optional;

//should be in env...
extern std::map<std::string, optional<int>> vars;
namespace ast
{
	struct printer
	{
		void operator()(unsigned int) const;
		void operator()(int) const;
		void operator()(std::string) const;
		void operator()(operation const&) const;
		void operator()(signed_ const&) const;
		void operator()(varDecl const&) const;
		void operator()(assignment const&) const;
		void operator()(print const&) const;
		void operator()(Expr const&) const;
		void operator()(statement const&) const;
		void operator()(program const&) const;
		void operator()(whileLoop const&) const;
		void operator()(conditional const&) const;
	};
}

#endif
