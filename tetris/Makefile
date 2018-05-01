CXX = g++
CXXFLAGS = -O3 -Wall -g -std=c++17 

a.out: main.o Block.o Game.o
	$(CXX) $(CXXFLAGS) main.o Block.o Game.o -lncurses

main.o: main.cc Block.h Game.h
	$(CXX) $(CXXFLAGS) -c main.cc 

Block.o: Block.cc Block.h
	$(CXX) $(CXXFLAGS) -c Block.cc

Game.o: Game.cc Game.h Block.h
	$(CXX) $(CXXFLAGS) -c Game.cc

clean:
	rm -f a.out core *.o
