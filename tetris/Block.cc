#include "Block.h"

Block::Block(){
	srand(clock());
	type = rand() % 7;
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			shape[i][j] = BLOCKS[type][orientation][i][j];
		}
	}
	
	position.first = 0;
	position.second = 10 / 2 - 2; //fix so doesn't need grid width;
}

void Block::rotate_right(){
	orientation++;
	if(orientation >= 4) orientation -= 4;
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			shape[i][j] = BLOCKS[type][orientation][i][j];
		}
	}
}

void Block::rotate_left(){
	orientation--;
	if(orientation < 0) orientation += 4;
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			shape[i][j] = BLOCKS[type][orientation][i][j];
		}
	}
}

std::array<std::array<bool, 4>, 4> Block::get_shape(){
	return shape;
}

void Block::move_right(){
	position.second++;
}

void Block::move_left(){
	position.second--;
}

void Block::move_down(){
	position.first++;
}

void Block::move_up(){
	position.first--;
}

int Block::get_y(){
	return position.first;
}

int Block::get_x(){
	return position.second;
}