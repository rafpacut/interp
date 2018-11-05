#include <iostream>
#include <iterator>
#include <boost/optional/optional_io.hpp>
#include "env.hpp"

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
					std::copy(vec.cbegin(), vec.cend(),
							std::ostream_iterator<int>(std::cout," "));
				}
				else
					std::cout<<"--"<<std::endl;
			}
			

		}

		void pollAction() 
		{
			char action =' ';
			//
			while(!skipStep && !(action == 's' || action == 'c'))
			{
				std::cout<<"[s]tep, [c]ontinue?";
				std::cin>>action;

				if(action == 'c')
					skipStep = true;

			}
		}

		bool skipStep = false;
	};
}











