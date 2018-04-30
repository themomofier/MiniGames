#include "Game.h"

Game::Game(){
	for(std::array<bool, GRID_WIDTH> &line : grid){
		for(bool &b : line){
			b = false;
		}
	}
	
	for(int &num : holes){
		num = GRID_WIDTH;
	}
	
}

Block Game::get_block(){
	return block;
}

std::array<std::array<bool, GRID_WIDTH>, GRID_HEIGHT> Game::get_grid(){
	return grid;
}

void Game::next_block(){
	Block b;
	block = b;
}

bool Game::collision(){
	for(int y = 0; y < 4; y++){
		for(int x = 0; x < 4; x++){
			if(block.get_shape()[y][x] && 
			   (block.get_y() + y < 0 || block.get_y() + y >= GRID_HEIGHT ||
			    block.get_x() + x < 0 || block.get_x() + x >= GRID_WIDTH ||
			    grid[block.get_y() + y][block.get_x() + x])){
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
		next_block();
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
	for(int y = 0; y < 4; y++){
		for(int x = 0; x < 4; x++){
			if(block.get_shape()[y][x]){
				grid[block.get_y() + y][block.get_x() + x] = true;
				holes[block.get_y() + y]--;
				//std::cout << "Line" << block.get_y() + y << "holes" << holes[block.get_y() + y] << std::endl;
			}
		}
	}
	
	for(int y = 0; y < 4; y++){
		if(block.get_y() + y >= 0 && block.get_y() + y < GRID_HEIGHT && holes[block.get_y() + y] == 0){
			//std::cout << "No holes!! in line " << block.get_y() + y << std::endl;
			for(int Y = block.get_y() + y - 1; Y >= 0; Y--){
				grid[Y + 1] = grid[Y]; 
				holes[Y + 1] = holes[Y];
			}
			for(int X = 0; X < GRID_WIDTH; X++){
				grid[0][X] = false;
			}
			holes[0] = GRID_WIDTH;
		}
	}
	
}
