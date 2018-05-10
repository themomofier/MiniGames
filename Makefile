CXX = g++
CXXFLAGS = -O3 -Wall -g -std=c++17 

a.out: main.cc tetris snake Pong mineSweeper 

	$(MAKE) -C mineSweeper
	$(MAKE) -C Pong
	$(MAKE) -C snake
	$(MAKE) -C tetris
	$(CXX) $(CXXFLAGS) main.cc tetris/*.o snake/*.o Pong/*.o -lncurses 
