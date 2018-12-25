CXX = g++
ROOT_BOOST = /opt/boost_1_69_0
CXXFLAGS = -Wall -Wextra -std=c++17 -I $(ROOT_BOOST) 
DEBUGFLAGS = -g3 -O0
.PHONY: clean

rebuild: 
	$(CXX) $(CXXFLAGS) env.o eval.o astPrinter.o envPrinter.o main.cpp -o interp.a -lstdc++fs

build: astPrinter eval env envPrinter
	$(CXX) $(CXXFLAGS) env.o eval.o astPrinter.o envPrinter.o main.cpp -o interp.a -lstdc++fs

astPrinter:
	$(CXX) $(CXXFLAGS) -c astPrinter.cpp

envPrinter: 
	$(CXX) $(CXXFLAGS) -c envPrinter.cpp

eval: envPrinter
	$(CXX) $(CXXFLAGS) -c eval.cpp

env: envPrinter
	$(CXX) $(CXXFLAGS) -c env.cpp

clean:
	rm *.o
	 
rebuildDebug: 
	$(CXX) $(DEBUGFLAGS) $(CXXFLAGS) env.cpp eval.cpp astPrinter.cpp envPrinter.cpp main.cpp -o dinterp.a -lstdc++fs

buildDebug: 
	$(CXX) $(DEBUGFLAGS) $(CXXFLAGS) env.o eval.o astPrinter.o envPrinter.o main.cpp -o dinterp.a -lstdc++fs

