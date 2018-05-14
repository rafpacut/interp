#include <list>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/adapted.hpp>
#include <boost/spirit/home/x3.hpp>


namespace x3 = boost::spirit::x3;

namespace ast {
	struct signed_;
	struct operation;
	struct varDecl;
	struct Expr;
	struct statement;
	struct program;
}
