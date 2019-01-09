#include <boost/config/warning_disable.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>

#include <list>
#include <algorithm>
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <iterator>

#include "ast.h"
#include "astAdapted.hpp"
#include "grammar.cpp"
#include "cmdArgumentProcessor.cpp"
#include "env.hpp"
#include "astPrinter.h"
#include "eval.h"

std::list<ast::Statement>
parse(std::string const& input)
{
    using boost::spirit::x3::ascii::space;
    using boost::spirit::x3::eol;
    typedef std::string::const_iterator iterator_type;

    std::list<ast::Statement> ast;
    iterator_type iter = input.begin();
    iterator_type const end = input.end();

    //position cache
    using iterator_type = std::string::const_iterator;
    using position_cache = boost::spirit::x3::position_cache<std::vector<iterator_type>>;
    position_cache positions{input.begin(), input.end()};

    // Our error handler
    using boost::spirit::x3::with;
    using boost::spirit::x3::error_handler_tag;
    using error_handler_type = boost::spirit::x3::error_handler<iterator_type>;

    error_handler_type error_handler(iter, end, std::cerr);

    auto const parser =
	with<grammar::position_cache_tag>(std::ref(positions))
        [
		with<error_handler_tag>(std::ref(error_handler))
			[
			   grammar::program
			]
        ];

    bool r = phrase_parse(iter, end, parser, (eol|space), ast);

    ast::Printer print;
    if (r && iter == end)
    {
	    auto frst = ast.front();
	    std::cout<<"First node in ast:\n";
	    print(frst);
	    std::cout<<"\n";
	    auto pos = positions.position_of(frst);
	        std::cout << "-------------------------\n";
	    std::cout << std::string(pos.begin(), pos.end()) << std::endl;
	        std::cout << "-------------------------\n";
            std::cout << "-------------------------\n";
            std::cout << "Parsing succeeded\n";
    }
    else
    {
        std::cout << "-------------------------\n";
        std::cout << "Parsing failed\n";
        std::cout << "-------------------------\n";
        ast.clear();
    }
    return ast;
}

int
main(int argc, char** argv)
{
	auto [filePath, runInteractive, debug] = cmdArgProcessor::process(argc, argv);
	
	//if provided an incorrect filePath run interactive
	//Why not in cmdArgProcessor?
	if(!filePath)
		runInteractive = true;

       ast::Printer print;
       ast::Eval eval(debug);

	std::string str;
	if(!runInteractive) 
	{
	       //read the file
	       std::ifstream in(filePath, std::ios::in | std::ios::binary);
	       str = std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	
	       //process
	       auto ast = parse(str);
	       if(!ast.empty())
	       {
		       if(debug)
			       print(ast);

		    	try
		    	{
		    		eval(ast);
		    	}
		    	catch(std::runtime_error& e)
		    	{
		    		std::cout<<e.what()<<std::endl;
		    		return 1;
		    	}
		    	catch(std::out_of_range& e) //doesn't say which var is undeclared.
		    	{
		    		std::cout<<"Using undeclared variable"<<std::endl;
		    	}
		    	catch(...)
		    	{
				std::cout<<"Unknown error happened\n";
		    		return 1;
		    	}
	       }
	}
	else
	{
	        std::cout << "Type an expression...or [q or Q] to quit\n\n";
	
	
	        while (std::getline(std::cin, str)) 
	        {
	    		if (str[0] == 'q' || str[0] == 'Q')
	    		    break;
	    		if(str.empty())
	    		       	continue;

			auto ast = parse(str);
			if(!ast.empty())
			{
			       if(debug)
				       print(ast);
	    		    try
	    		    {
	    			   eval(ast);
	    		    }
	    		    catch(std::runtime_error& e)
	    		    {
	    			    std::cout<<e.what()<<std::endl;
	    		    }
	        	        catch(std::out_of_range& e)
	    		    {
	    		            std::cout<<"Using undeclared variable"<<std::endl;
	    		    }
	    		    catch(...)
	    		    {
					std::cout<<"Unknown error happened\n";
	    		    }//do nothing. Rn all throwable exceptions allow processing of the next query.
	    		}
	        }
	}

    return 0;
}
