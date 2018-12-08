#include "eval.h"
namespace ast
{

	class FunctionPassParamsVisitor 
	{
		public:
			FunctionPassParamsVisitor(Eval& e) : eval(e) {};

			void operator()(VarDecl& argDecl, const Expr& expr)
			{
				(eval)(argDecl);
				(eval)(Assignment(argDecl.name, expr));
			}

			void operator()(VarDecl& argDecl, std::string varName)
			{
				(eval)(argDecl);
				(eval)(CopyValue(argDecl.name, varName));
			}

			void operator()(const ArrDecl& argDecl, std::string arrName)
			{
				(eval)(argDecl);
				(eval)(CopyValue(argDecl.name, arrName));
			}

			//Is it necessary?
			void operator()(const ArrDecl&, const Expr&)
			{
				throw std::runtime_error("Expected an array, got expression");
			}

		private:
			Eval& eval;
	};
}
