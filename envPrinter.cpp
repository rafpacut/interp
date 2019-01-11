#include <iostream>
#include <iterator>
#include <boost/optional/optional_io.hpp>
#include "env.hpp"
#include "astPrinter.h"

namespace ast
{
	struct envPrinter
	{
		void operator()(const Environment& e) 
		{
			pollAction();
			if(!skipStep)
			{
				std::cout<<"Environment:"<<std::endl;

				std::for_each(e.scopes.crbegin(), e.scopes.crend(),
						[this](Scope const& s)
						{
							std::cout<<'{'<<std::endl;
							printScope(s);
							std::cout<<'}'<<std::endl;
						});

				if(printFun)
				{

					std::cout<<"functions:\n";
					for(const auto& f: e.functions)
						astPrint(f);
				}
			}
		}

		void printScope(const Scope& s) 
		{
			std::cout<<"	vars:"<<std::endl;
			for(const auto& v: s.ints)
				std::cout<<"	"<<v.first<<" = "<< v.second<<std::endl;

			std::cout<<"	arrays:"<<std::endl;
			for(const auto& a: s.intVecs)
			{
				std::cout<<a.first<<" : ";
				if(a.second)
				{
					std::vector<int> vec = *(a.second);
					std::cout<<'{';
					std::copy(vec.cbegin(), vec.cend(),
							std::ostream_iterator<int>(std::cout," "));
					std::cout<<'}';
				}
				else
					std::cout<<"--"<<std::endl;
				std::cout<<'\n';
			}
		}

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

		bool printFun = false;
		bool skipStep = false;
		ast::Printer astPrint;
	};
}











