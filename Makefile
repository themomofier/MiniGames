CXX = g++
CXXFLAGS = -O3 -Wall -g -std=c++17 

a.out: main.cc 
	$(MAKE) -C tetris
	$(MAKE) -C snake
#	$(MAKE) -C mineSweeper
	$(MAKE) -C Pong
	$(CXX) $(CXXFLAGS) main.cc tetris/*.o -lncurses #mineSweeper/*.o snake/*.o Pong/*.o
	
