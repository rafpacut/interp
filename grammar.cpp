namespace grammar 
{
	namespace x3 = boost::spirit::x3;
	using namespace x3;

        struct error_handler
        {
            template <typename Iterator, typename Exception, typename Context>
            x3::error_handler_result on_error(
                Iterator& first, Iterator const& last
              , Exception const& x, Context const& context)
            {
                auto& error_handler = x3::get<x3::error_handler_tag>(context).get();
                std::string message = "Error! Expecting: " + x.which() + " here:";
                error_handler(x.where(), message);
                return x3::error_handler_result::fail;
            }
        };
    struct position_cache_tag;

    struct annotate_position
    {
        template <typename T, typename Iterator, typename Context>
        inline void on_success(Iterator const& first, Iterator const& last
        , T& ast, Context const& context)
        {
            auto& position_cache = x3::get<position_cache_tag>(context).get();
            position_cache.annotate(ast, first, last);
        }
    };

	struct expression_	;
	struct term_      	;
	struct factor_    	;
	struct statement_ 	;
	struct name_      	;
	struct print_     	;
	struct type_      	;
	struct arrType    	;
	struct varDecl_   	;
	struct assignment_	;
	struct comparison_	;
	struct whileLoop_ 	;
	struct conditional	;
	struct codeBlock_ 	;
	struct arrDecl_   	;
	struct assignmentArr_	;
	struct arrValue   	;
	struct functionDecl	;
	struct functionCall	; 
	struct returnStmt 	;

        x3::rule<expression_, ast::Expr> const expression("expression");
        x3::rule<term_, ast::Expr> const term("term");
        x3::rule<factor_, ast::Operand> const factor("factor");
	x3::rule<statement_, ast::Statement> const statement("statement");
	x3::rule<name_, std::string> const name("name");
	x3::rule<print_, ast::Print> const print("print");
	x3::rule<type_, std::string> const type("type");
	x3::rule<arrType, std::string> const arrType("arrType");
	x3::rule<varDecl_, ast::VarDecl> const varDecl("varDecl");
	x3::rule<assignment_, ast::Assignment> const assignment("assignment");
	x3::rule<comparison_, ast::Comparison> const comparison("comparison");
	x3::rule<whileLoop_, ast::WhileLoop> const whileLoop("whileLoop");
	x3::rule<conditional, ast::Conditional > const conditional("conditional");
	x3::rule<codeBlock_, std::list<ast::Statement> > const codeBlock("codeBlock");
	x3::rule<arrDecl_, ast::ArrDecl> const arrDecl("arrDecl");
	x3::rule<assignmentArr_, ast::AssignmentArr> const assignmentArr("assignmentArr");
	x3::rule<arrValue, ast::ArrValue> const arrValue("arrValue");
	x3::rule<functionDecl, ast::FunctionDecl> const functionDecl("functionDecl");
	x3::rule<functionCall, ast::FunctionCall> const functionCall("functionCall");
	x3::rule<returnStmt, ast::Return> const returnStmt("returnStmt");


	const auto type_def
	= string("int")
	| string("float")
	| string("string")
	;

	const auto arrType_def
	= string("array<int>");

	const auto name_def 
	= (x3::alpha >> *x3::alnum); 

	const auto returnStmt_def
	= x3::lit("return") >> expression; 

	const auto varDecl_def
	=  type >> name >> -('=' >> expression);

	const auto arrDecl_def
	= arrType >> name >> -('=' >> expression);
	
	const auto functionDecl_def
	= type >> name >> '(' >> -((varDecl | arrDecl) % ',') >> ')'
	  >>codeBlock 
	  ;

	const auto functionCall_def
	= name >> '(' >> -(expression % ',') >> ')';

	const auto assignment_def
	= name >> '=' >> expression;

	const auto arrValue_def
	= name >> '[' >> expression >> ']';

	const auto assignmentArr_def
	= arrValue >> '=' >> expression;

	const auto print_def
	=   x3::lit("print(") 
	    >> expression
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
	    |   functionCall
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
	    >> (
	        string("<=")
	      | string(">=")
	      | string("<")
	      | string(">")
	      | string("==")
	      | string("!=")
	      ) 
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
	    | (returnStmt >> ';')
	    | (varDecl >> ';')
	    | (arrDecl >> ';')
	    | (assignment >> ';')
	    | (print >> ';')
	    | (functionCall >> ';')
	    | (assignmentArr >> ';')
	    | (expression >> ';')
	    ;


	const auto program
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
	  , functionCall
	  , arrValue
	  , assignment
	  , assignmentArr
	  , codeBlock
	  , whileLoop
	  , conditional
	  , statement
	  , type
	  , returnStmt
	  , arrType
        );

	struct statement_ 	: error_handler, annotate_position {};
	struct expression_	: annotate_position {};
	struct term_      	: annotate_position {};
	struct factor_    	: annotate_position {};
	struct name_      	: annotate_position {};
	struct print_     	: annotate_position {};
	struct type_      	: annotate_position {};
	struct arrType    	: annotate_position {};
	struct varDecl_   	: annotate_position {};
	struct assignment_	: annotate_position {};
	struct comparison_	: annotate_position {};
	struct whileLoop_ 	: annotate_position {};
	struct conditional	: annotate_position {};
	struct codeBlock_ 	: annotate_position {};
	struct arrDecl_   	: annotate_position {};
	struct assignmentArr_	: annotate_position {};
	struct arrValue   	: annotate_position {};
	struct functionDecl	: annotate_position {};
	struct functionCall	: annotate_position {}; 
	struct returnStmt 	: annotate_position {};

}

