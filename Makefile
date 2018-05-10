CXX = g++
CXXFLAGS = -O3 -Wall -g -std=c++17 

a.out: main.cc 

#	$(MAKE) -C mineSweeper
	$(MAKE) -C Pong
	$(MAKE) -C snake
	$(MAKE) -C tetris
#	$(CXX) $(CXXFLAGS) main.cc tetris/*.o -lncurses #mineSweeper/*.o snake/*.o Pong/*.o
	
