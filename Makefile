CXX = g++-7
ROOT_BOOST = /opt/boost_1_66_0
CXXFLAGS = -Wall -Wextra -std=c++17 -I $(ROOT_BOOST) 
DEBUGFLAGS = -g3 -O0
.PHONY: clean

build: 
	$(CXX) $(CXXFLAGS) env.o eval.o astPrinter.o envPrinter.o main.cpp -o interp.a

rebuild: astPrinter eval env envPrinter
	$(CXX) $(CXXFLAGS) env.o eval.o astPrinter.o envPrinter.o main.cpp -o interp.a

astPrinter:
	$(CXX) $(CXXFLAGS) -c astPrinter.cpp

envPrinter: 
	$(CXX) $(CXXFLAGS) -c envPrinter.cpp

eval: envPrinter
	$(CXX) $(CXXFLAGS) -c eval.cpp

env: eval envPrinter
	$(CXX) $(CXXFLAGS) -c env.cpp

clean:
	rm *.o
	 
rebuildDebug: 
	$(CXX) $(DEBUGFLAGS) $(CXXFLAGS) env.cpp eval.cpp astPrinter.cpp envPrinter.cpp main.cpp -o dinterp.a

buildDebug: 
	$(CXX) $(DEBUGFLAGS) $(CXXFLAGS) env.o eval.o astPrinter.o envPrinter.o main.cpp -o dinterp.a

