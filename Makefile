mine: ast.o grammar.o astPrinter.o
	g++ -Wall -std=c++14 -I /opt/boost_1_66_0 ast.o grammar.o astPrinter.o main.cpp -o parser.a

ast.o:
	g++ -Wall -std=c++14 -I /opt/boost_1_66_0 -c ast.cpp

grammar.o:
	g++ -Wall -std=c++14 -I /opt/boost_1_66_0 -c grammar.cpp

astPrinter.o:
	g++ -Wall -std=c++14 -I /opt/boost_1_66_0 -c astPrinter.cpp

clean:
	rm *.o
	 
