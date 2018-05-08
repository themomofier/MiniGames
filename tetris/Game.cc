#include "Game.h"
#include <iostream>

Game::Game() : block(GRID_WIDTH / 2 - 2) {
	for(std::array<Square, GRID_WIDTH> &line : grid){
		for(Square &sqr : line){
			sqr.present = false;
		}
	}
	
	for(int &num : holes){
		num = GRID_WIDTH;
	}
	
}

Block Game::get_block(){return block;}

std::array<std::array<Square, GRID_WIDTH>, GRID_HEIGHT> Game::get_grid(){return grid;}

int Game::get_score(){return score;}

bool Game::is_game_over(){return game_over;}

void Game::next_block(){
	Block b(GRID_WIDTH / 2 - 2);
	block = b;
	if(collision()){
		game_over = true;
	}
	
}

bool Game::collision(){
	for(int y = 0; y < 4; y++){
		for(int x = 0; x < 4; x++){
			if(block.get_shape()[y][x] && 
			   (block.get_y() + y < 0 || block.get_y() + y >= GRID_HEIGHT ||
			    block.get_x() + x < 0 || block.get_x() + x >= GRID_WIDTH ||
			    grid[block.get_y() + y][block.get_x() + x].present)){
					return true;
			}
		}
	}
	return false;
}

void Game::move_block_right(){
	block.move_right();
	if(collision()){
		block.move_left();
	}
}

void Game::move_block_left(){
	block.move_left();
	if(collision()){
		block.move_right();
	}
}

bool Game::move_block_down(){
	block.move_down();
	if(collision()){
		block.move_up();
		place_block();
		return false;
	}
	return true;
}

void Game::rotate_block(){
	block.rotate_right();
	if(collision()){
		block.rotate_left();
	}
}

void Game::drop_block(){
	while(move_block_down());
}

void Game::place_block(){
	int y_coord, x_coord, numclears = 0;
	for(int y = 0; y < 4; y++){
		for(int x = 0; x < 4; x++){
			y_coord = block.get_y() + y;
			x_coord = block.get_x() + x;
			if(block.get_shape()[y][x]){
				grid[y_coord][x_coord] = {true, block.get_type()};
				holes[y_coord]--;
			}
		}
	}
	score += 4;

	for(int y = 0; y < 4; y++){
		y_coord = block.get_y() + y;
		if(y_coord >= 0 && y_coord < GRID_HEIGHT && holes[y_coord] == 0){
			numclears++;
			for(int Y = y_coord - 1; Y >= 0; Y--){
				grid[Y + 1] = grid[Y]; 
				holes[Y + 1] = holes[Y];
			}
			for(int X = 0; X < GRID_WIDTH; X++){
				grid[0][X].present = false;
			}
			holes[0] = GRID_WIDTH;
			std::cout << "" << std::endl;
		}
	}
	score += numclears * numclears * GRID_WIDTH;
	next_block();
}
