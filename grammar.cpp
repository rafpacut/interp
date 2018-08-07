namespace grammar 
{
	using namespace x3;

        x3::rule<class expression_, ast::Expr> const expression("expression");
        x3::rule<class term_, ast::Expr> const term("term");
        x3::rule<class factor_, ast::operand> const factor("factor");
	x3::rule<class statement_, ast::statement> const statement("statement");
	x3::rule<class program_, std::list<ast::statement> > const program("program");
	x3::rule<class name_, std::string> const name("name");
	x3::rule<class print_, ast::print> const print("print");
	x3::rule<class type_, std::string> const type("type");
	x3::rule<class varDecl_, ast::varDecl> const varDecl("varDecl");
	x3::rule<class assignment_, ast::assignment> const assignment("assignment");
	x3::rule<class whileLoop_, ast::whileLoop> const whileLoop("whileLoop");
	x3::rule<class codeBlock_, std::list<ast::statement> > const codeBlock("codeBlock");


	const auto type_def
	= string("int")
	| string("float")
	| string("string")
	;


	const auto name_def 
	= (x3::alpha >> *x3::alnum) - x3::lit("while");

	const auto varDecl_def
	= type >> name >> -('=' >> expression);

	const auto assignment_def
	= name >> '=' >> expression;

	const auto print_def
	=     x3::lit("print(") 
	    >> name >> x3::lit(")");


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

	const auto codeBlock_def
	= 
	    '{'
	    >> +statement
	    >> '}'
	    ;

	const auto whileLoop_def
	= 
	    x3::lit("while") 
	    >> '(' >> expression >> ')'
	    >> codeBlock
	    ;
 
	const auto statement_def 
	= 
	    (varDecl >> ';')
	    | (assignment >> ';')
	    | (expression >> ';')
	    | whileLoop
	    | (print >> ';')
	    ;

	const auto program_def 
	= +statement
	;


        BOOST_SPIRIT_DEFINE(
            expression
          , term
          , factor
	  , name
	  , print
	  , varDecl
	  , assignment
	  , codeBlock
	  , whileLoop
	  , statement
	  , type
	  , program
        );

}

