#include "Block.h"

const int GRID_WIDTH = 10;
const int GRID_HEIGHT = 18;

class Game{
	std::array<std::array<bool ,GRID_WIDTH>, GRID_HEIGHT> grid; // board[y][x] 0 top left
	std::array<int, GRID_HEIGHT> holes; //holes[y]
	Block block;

	public:
	Game();
	Block get_block();
	std::array<std::array<bool, GRID_WIDTH>, GRID_HEIGHT> get_grid();
	void next_block();
	bool collision();
	void move_block_right();
	void move_block_left();
	bool move_block_down();
	void rotate_block();
	void drop_block();
	void place_block();
	
};
