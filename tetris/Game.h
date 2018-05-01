#include "Block.h"

const int GRID_WIDTH = 10;
const int GRID_HEIGHT = 20;

struct Square{
	bool present;
	int color;
};

class Game{
	std::array<std::array<Square, GRID_WIDTH>, GRID_HEIGHT> grid; // board[y][x] 0 top left
	std::array<int, GRID_HEIGHT> holes; //holes[y]
	Block block;
	bool game_over = false;
	int score = 0;

	public:
	Game();
	Block get_block();
	std::array<std::array<Square, GRID_WIDTH>, GRID_HEIGHT> get_grid();
	int get_score();
	bool is_game_over();
	void next_block();
	bool collision();
	void move_block_right();
	void move_block_left();
	bool move_block_down();
	void rotate_block();
	void drop_block();
	void place_block();
	
};
