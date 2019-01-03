#include <iostream>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <string>

#include "ast.h"
#include "astAdapted.hpp"
#include "grammar.cpp"
#include "cmdArgumentProcessor.cpp"
#include "env.hpp"
#include "astPrinter.h"
#include "eval.h"


std::list<ast::Statement>
parse(const std::string& input)
{
	using boost::spirit::x3::space;
	using boost::spirit::x3::eol;
        typedef std::string::const_iterator iterator_type; //why typedef?

	std::list<ast::Statement> ast;
	iterator_type iter = input.begin();
	iterator_type const  end = input.end();

	using boost::spirit::x3::with;
        using boost::spirit::x3::error_handler_tag;
        using error_handler_type = boost::spirit::x3::error_handler<iterator_type>;

        error_handler_type error_handler(iter, end, std::cerr);

	//using grammar::program;
	auto const parser =
		with<error_handler_tag>(std::ref(error_handler))
		[
			grammar::program
		];

       bool r = phrase_parse(iter, end, parser, (space | eol), ast);

       if(r && iter == end)
       {
	       std::cout<<"Parsing succeded\n";
       }
       else
       {
	       std::cout<<"Parsing failed\n";
	       std::cout << "Remaining unparsed: " << std::quoted(std::string(iter, end), '\'') << "\n";
	       ast.clear();
       }
       return ast;
}

int main(int argc, char **argv) 
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
}
