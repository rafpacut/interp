namespace grammar 
{
	using namespace x3;

        x3::rule<class expression_, ast::Expr> const expression("expression");
        x3::rule<class term_, ast::Expr> const term("term");
        x3::rule<class factor_, ast::operand> const factor("factor");
	x3::rule<class statement_, ast::statement> const statement("statement");
	x3::rule<class program_, std::list<ast::statement> > const program("program");
	x3::rule<class name_, std::string> const name("name");
	x3::rule<class type_, std::string> const type("type");
	x3::rule<class varDecl_, ast::varDecl> const varDecl("varDecl");


	const auto type_def
	= string("int")
	| string("float")
	| string("string")
	;


	const auto name_def 
	= x3::alpha >> *x3::alnum;

	const auto varDecl_def
	= type >> name >> -('=' >> expression);

        const auto expression_def 
        =
            term
            >> *(   (char_('+') > term)
                |   (char_('-') > term)
		|   (char_('+') > name)
		|   (char_('-') > name)
                )
            ;
 
        const auto factor_def 
        =
                x3::uint_
            |   name		
            |   ('(' > expression > ')')
            |   (char_('-') > factor)
            |   (char_('+') > factor)
            ;
 
        const auto term_def 
        =
            factor
            >> *(   (char_('*') > factor)
                |   (char_('/') > factor)
                )
            ;
 
	const auto statement_def 
	= 
	    varDecl
	    | expression
	    ;

	const auto program_def 
	= +(statement >> ';');


        BOOST_SPIRIT_DEFINE(
            expression
          , term
          , factor
	  , name
	  , varDecl
	  , statement
	  , type
	  , program
        );

}

