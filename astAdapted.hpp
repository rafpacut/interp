#ifndef AST_ADAPTED_H_
#define AST_ADAPTED_H_

BOOST_FUSION_ADAPT_STRUCT(ast::Signed_,
    sign, operand_
)

BOOST_FUSION_ADAPT_STRUCT(ast::Operation,
    operator_, operand_
)

BOOST_FUSION_ADAPT_STRUCT(ast::Return, value)

BOOST_FUSION_ADAPT_STRUCT(ast::Expr, 
		first, rest
		)

BOOST_FUSION_ADAPT_STRUCT(ast::Comparison,
		lhs, op, rhs 
)

BOOST_FUSION_ADAPT_STRUCT(ast::WhileLoop,
		condition, body
		)

BOOST_FUSION_ADAPT_STRUCT(ast::VarDecl, 
		type, name, value
		)

BOOST_FUSION_ADAPT_STRUCT(ast::ArraySize, name)

BOOST_FUSION_ADAPT_STRUCT(ast::PushBack,
		name, value)
	
BOOST_FUSION_ADAPT_STRUCT(ast::ArrDecl, 
		type, name, initValue
		)

BOOST_FUSION_ADAPT_STRUCT(ast::FunctionDecl,
	       	type, name, args, body
		)

BOOST_FUSION_ADAPT_STRUCT(ast::FunctionCall,
	       	name, params 
		)

BOOST_FUSION_ADAPT_STRUCT(ast::ArrValue, 
		name, idx
		)

BOOST_FUSION_ADAPT_STRUCT(ast::Conditional, 
		condition, tBody, fBody
		)

BOOST_FUSION_ADAPT_STRUCT(ast::Assignment, 
		name, value
		)

BOOST_FUSION_ADAPT_STRUCT(ast::AssignmentArr, 
		id, value
		)

BOOST_FUSION_ADAPT_STRUCT(ast::Print, val)


#endif
