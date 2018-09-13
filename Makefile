CXX = g++-7
ROOT_BOOST = /opt/boost_1_66_0
CXXFLAGS = -Wall -std=c++14 -I $(ROOT_BOOST) -fdiagnostics-color=always



mine: astPrinter eval
	$(CXX) $(CXXFLAGS) eval.o astPrinter.o main.cpp -o interp.a

astPrinter:
	$(CXX) $(CXXFLAGS) -c astPrinter.cpp

eval:
	$(CXX) $(CXXFLAGS) -c eval.cpp

clean:
	rm *.o
	 
debug: astPrinter.o eval.o
	$(CXX) $(CXXFLAGS) -g eval.o astPrinter.o main.cpp -o dinterp.a
