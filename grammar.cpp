namespace grammar 
{
	namespace x3 = boost::spirit::x3;
	using namespace x3;


        x3::rule<class expression_, ast::Expr> const expression("expression");
        x3::rule<class term_, ast::Expr> const term("term");
        x3::rule<class factor_, ast::Operand> const factor("factor");
	x3::rule<class statement_, ast::Statement> const statement("statement");
	x3::rule<class program_, std::list<ast::Statement> > const program("program");
	x3::rule<class name_, std::string> const name("name");
	x3::rule<class print_, ast::Print> const print("print");
	x3::rule<class type_, std::string> const type("type");
	x3::rule<class arrType, std::string> const arrType("arrType");
	x3::rule<class varDecl_, ast::VarDecl> const varDecl("varDecl");
	x3::rule<class assignment_, ast::Assignment> const assignment("assignment");
	x3::rule<class copyValue, ast::CopyValue> const copyValue("copyValue");
	x3::rule<class comparison_, ast::Comparison> const comparison("comparison");
	x3::rule<class whileLoop_, ast::WhileLoop> const whileLoop("whileLoop");
	x3::rule<class conditional, ast::Conditional > const conditional("conditional");
	x3::rule<class codeBlock_, std::list<ast::Statement> > const codeBlock("codeBlock");
	x3::rule<class arrDecl_, ast::ArrDecl> const arrDecl("arrDecl");
	x3::rule<class assignmentArr_, ast::AssignmentArr> const assignmentArr("assignmentArr");
	x3::rule<class arrValue, ast::ArrValue> const arrValue("arrValue");
	x3::rule<class functionDecl, ast::FunctionDecl> const functionDecl("functionDecl");


	const auto type_def
	= string("int")
	| string("float")
	| string("string")
	;

	const auto arrType_def
	= string("array<int>");

	const auto name_def 
	= (x3::alpha >> *x3::alnum); 

	const auto varDecl_def
	=  type >> name >> -('=' >> expression);

	const auto arrDecl_def
	= arrType >> name; //>> -('=' >> ?init list?);
	
	const auto functionDecl_def
	= 
	    type >> name >> '(' >> +(varDecl | arrDecl) >> ')'
	    >> codeBlock
	    ;

	const auto assignment_def
	= name >> '=' >> expression;

	const auto copyValue_def
	= name >> '=' >> name;

	const auto arrValue_def
	= name >> '[' >> expression >> ']';

	const auto assignmentArr_def
	= arrValue >> '=' >> expression;

	const auto print_def
	=   x3::lit("print(") 
	    >>(
	        x3::uint_
	      | arrValue
	      | name 
	      )
	    >> x3::lit(")")
	;

	const auto conditional_def
	= x3::lit("if")
	  >>'(' >> comparison >> ')'
	  >> codeBlock
	  >> -(x3::lit("else") >> codeBlock)
	 ;

        const auto expression_def 
        =
            term
            >> *(   (char_('+') > term)
                |   (char_('-') > term)
                )
            ;
 
        const auto factor_def 
        =
	        x3::uint_
            |   arrValue
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

	const auto comparison_def
	=
	    expression 
	    >> (char_('<') | char_('>')) 
	    >> expression
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
	    >> '(' >> comparison >> ')'
	    >> codeBlock
	    ;
 
	const auto statement_def 
	= 
	    functionDecl
	    | whileLoop
	    | conditional
	    | (varDecl >> ';')
	    | (arrDecl >> ';')
	    | (copyValue >> ';')
	    | (assignment >> ';')
	    | (assignmentArr >> ';')
	    | (expression >> ';')
	    | (print >> ';')
	    ;


	const auto program_def 
	= +statement
	;


        BOOST_SPIRIT_DEFINE(
            expression
          , term
          , factor
	  , comparison
	  , name
	  , print
	  , varDecl
	  , arrDecl
	  , functionDecl
	  , arrValue
	  , assignment
	  , copyValue
	  , assignmentArr
	  , codeBlock
	  , whileLoop
	  , conditional
	  , statement
	  , type
	  , arrType
	  , program
        );

}

