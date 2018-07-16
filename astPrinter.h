#ifndef astPrinter_H
#define astPrinter_H


#include <iostream>
#include "ast.h"
#include <map>

//should be in eval...
extern std::map<std::string, int> vars;
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
		void operator()(Expr const&) const;
		void operator()(statement const&) const;
		void operator()(program const&) const;
	};
}

#endif
