mine: astPrinter.o eval.o
	g++ -Wall -std=c++14 -I /opt/boost_1_66_0 eval.o astPrinter.o main.cpp -o interp.a

astPrinter.o:
	g++ -Wall -std=c++14 -I /opt/boost_1_66_0 -c astPrinter.cpp

eval.o:
	g++ -Wall -std=c++14 -I /opt/boost_1_66_0 -c eval.cpp

clean:
	rm *.o
	 
debug: astPrinter.o eval.o
	g++ -g -Wall -std=c++14 -I /opt/boost_1_66_0 eval.o astPrinter.o main.cpp -o dinterp.a
