#include "astPrinter.h"
#include "env.hpp"
#include "envPrinter.cpp"

class DebugPrinter
{
	public:
		template<typename T>
		void operator()(T astNode) 
		{
			astPrint_(astNode);

			if(!skipStep)
				pollAction();

		}

		void operator()(ast::Environment& env) 
		{
			printEnv_(env);
		}

	private:

		void pollAction() 
		{
			char action =' ';
			//
			while(!skipStep && !(action == 's' || action == 'c' || action == 'p'))
			{
				std::cout<<"[s]tep, [c]ontinue, [p]rint functions?";
				std::cin>>action;

				if(action == 'c')
					skipStep = true;

				if(action == 'p')
					printFun = true;


			}
		}

	bool skipStep = false;
	bool printFun = false;
	ast::Printer astPrint_;
	ast::envPrinter printEnv_;
};
