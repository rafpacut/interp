#include <iostream>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <string>

#include "astPrinter.h"
#include "eval.h"
#include "grammar.cpp"


int main(int argc, char **argv) {

    auto &parser = grammar::program;
    ast::program program; 
    ast::printer print;
    ast::Eval eval;

    using It = std::string::const_iterator;

    std::string str;

    if(argc > 1 && argc < 3)
    {
	//read the file
	   std::ifstream in(argv[1], std::ios::in | std::ios::binary);
	   str = std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

	   //process
	   It iter = str.begin(), end = str.end();
	   if(phrase_parse(iter, end, parser, x3::space, program))
	   {
		std::cout<< "Parsing succeded\n";
		print(program);
		try
		{
			std::cout<<'\n'<<eval(program)<<'\n';
		}
		catch(std::runtime_error& e)
		{
			std::cout<<e.what()<<std::endl;
			return 1;
		}
		catch(...)
		{
			return 1;
		}

		if (iter != end)
			std::cout << "Remaining unparsed: " << std::quoted(std::string(iter, end), '\'') << "\n";
	   }
	   else
	    std::cout << "Parsing failed\n";

    }
    else
    {
	    std::cout << "Type an expression...or [q or Q] to quit\n\n";


	    while (std::getline(std::cin, str)) {
		if (str.empty() || str[0] == 'q' || str[0] == 'Q')
		    break;

		It iter = str.begin(), end = str.end();
		if (phrase_parse(iter, end, parser, x3::space, program)) {
		    std::cout << "Parsing succeeded\n";
		    print(program);

		    try
		    {
			    std::cout<<'\n'<<eval(program)<<'\n';
		    }
		    catch(std::runtime_error& e)
		    {
			    std::cout<<e.what()<<std::endl;
		    }
		    catch(...){}//do nothing. Rn all throwable exceptions allow processing of the next query.
		}
		else
		    std::cout << "Parsing failed\n";

		if (iter != end)
		    std::cout << "Remaining unparsed: " << std::quoted(std::string(iter, end), '\'') << "\n";
	    }
    }
}
