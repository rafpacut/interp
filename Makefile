CXX = g++-7
ROOT_BOOST = /opt/boost_1_66_0
CXXFLAGS = -Wall -Wextra -std=c++14 -I $(ROOT_BOOST) -fdiagnostics-color=always



mine: astPrinter eval env
	$(CXX) $(CXXFLAGS) env.o eval.o astPrinter.o main.cpp -o interp.a

astPrinter:
	$(CXX) $(CXXFLAGS) -c astPrinter.cpp

eval:
	$(CXX) $(CXXFLAGS) -c eval.cpp

env:
	$(CXX) $(CXXFLAGS) -c env.cpp

clean:
	rm *.o
	 
debug: astPrinter eval env
	$(CXX) $(CXXFLAGS) -g env.o eval.o astPrinter.o main.cpp -o dinterp.a
