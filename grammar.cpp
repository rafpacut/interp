#include <boost/spirit/home/x3.hpp>
#include "ast.h"

namespace x3 = boost::spirit::x3;

namespace grammar 
{
		static auto name 
			= x3::rule<class name_, std::string> {"name"}
		= x3::alpha >> *x3::alnum;

		static auto varDecl
			= x3::rule<class varDecl_, ast::varDecl> {"varDecl"}
		   = name
		   >> '='
		   >> x3::int_
		   ;

		   static auto expr
			   = x3::rule<class expr_, ast::Expr> {"expr"}
		   = x3::int_;


	    static auto statement 
		= x3::rule<class statement_, ast::statement> {"statement"}
		= varDecl
		| expr
		;

	    static auto program 
		= x3::rule<class program_, std::list<ast::statement> > {"program"}
		= +(statement >> ';');
}

