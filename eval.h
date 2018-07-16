#include <map>
#include <iostream>

namespace ast{
	struct Eval
	{
		typedef int result_type;

		int operator()(unsigned int n) const;
		int operator()(int n) const ;
		int operator()(std::string s) const;
		int operator()(operation const& x, int lhs) const;
		int operator()(signed_ const& x) const;
		int operator()(varDecl const& x);
		int operator()(Expr const& x) const;
		int operator()(statement const& x);
		int operator()(program const& x);

		//Ofc that goes into evironment later on.
		std::map<std::string, int> vars;
	};
}
