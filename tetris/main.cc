#include "Game.h"
#include <iostream>
#include <ctime>
#include <array>
#include <ncurses.h>
#include <sstream>
using namespace std;

const double DELAY = 1.0; //seconds

const int SQUARE_WIDTH = 4;
const int SQUARE_HEIGHT = 2;
const char SQUARE[SQUARE_HEIGHT][SQUARE_WIDTH] = 
	{{'#', '#', '#', ' '},
	 {'#', '#', '#', ' '}};

const char BORDER = 'H';

void print_border(){
	for(int x = 0; x < GRID_WIDTH * SQUARE_WIDTH + 2; x++){
		mvaddch(0, x, BORDER);
		mvaddch(GRID_HEIGHT * SQUARE_HEIGHT + 1, x, BORDER);
	}
	for(int y = 1; y < GRID_HEIGHT * SQUARE_HEIGHT + 1; y++){
		mvaddch(y, 0, BORDER);
		mvaddch(y, GRID_WIDTH * SQUARE_WIDTH + 1, BORDER);
	}
}

void print_square(int y_coord, int x_coord, bool present = true, bool del = false){
	if(present){
		for(int y = 0; y < SQUARE_HEIGHT; y++){
			for(int x = 0; x < SQUARE_WIDTH; x++){
				mvaddch(y_coord * SQUARE_HEIGHT + y + 1, 
						x_coord * SQUARE_WIDTH + x + 1, 
						SQUARE[y][x]);
			}
		}
	}else if(del){
		for(int y = 0; y < SQUARE_HEIGHT; y++){
			for(int x = 0; x < SQUARE_WIDTH; x++){
				mvaddch(y_coord * SQUARE_HEIGHT + y + 1, 
						x_coord * SQUARE_WIDTH + x + 1, 
						' ');
			}
		}
	}	
}

void delete_square(int y_coord, int x_coord, bool present = true){
	if(present){
		for(int y = 0; y < SQUARE_HEIGHT; y++){
			for(int x = 0; x < SQUARE_WIDTH; x++){
				mvaddch(y_coord * SQUARE_HEIGHT + y + 1, 
						x_coord * SQUARE_WIDTH + x + 1, 
						' ');
			}
		}
	}	
}

void delete_block(Block block){
	for(int y = 0; y < 4; y++){
		for(int x = 0; x < 4; x++){
			delete_square(block.get_y() + y, block.get_x() + x, block.get_shape()[y][x]);
		}
	}
}

void print_block(Block block){
	
	for(int y = 0; y < 4; y++){
		for(int x = 0; x < 4; x++){
			print_square(block.get_y() + y, block.get_x() + x, block.get_shape()[y][x]);
		}
	}
}

void print_game(Game game){

	for(int y = 0; y < GRID_HEIGHT; y++){
		for(int x = 0; x < GRID_WIDTH; x++){
			print_square(y, x, game.get_grid()[y][x], true);
		}
	}
	
	print_block(game.get_block());

	refresh();
}

int main(){


	array<array<bool, GRID_WIDTH>, GRID_HEIGHT> board;
	for(int i = 0; i < GRID_HEIGHT; i++){
		for(int j = 0; j < GRID_WIDTH; j++){
			board[i][j] = 0;
		}
	}
	board[17][9] = 1;
	board[16][8] = 1;
	board[17][8] = 1;
	board[16][9] = 1;
	
	
	bool game_over = false;
	initscr();
	printw("Hello World !!!");
	refresh();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	timeout(0);

	//Block block;
	Game game;

	clock_t start = clock();

	print_border();
	
	while(!game_over){
		int c = getch();
		if(c != ERR){
			delete_block(game.get_block());
			switch(c){
				case 'q':
				case 'Q':
					game_over = true;
					break;
				case KEY_UP:
					game.rotate_block();
					//block.rotate();
					break;
				case KEY_LEFT:
					game.move_block_left();
					//block.move_left();
					break;
				case KEY_RIGHT:
					game.move_block_right();
					//block.move_right();
					break;
				case KEY_DOWN:
					game.move_block_down();
					//block.move_down();
					break;
				case ' ':
					game.drop_block();
					break;
			}
			
			//print_block(game.get_block());
		}
		
		
		if(clock() - start >= DELAY * CLOCKS_PER_SEC){
			start = clock();
			delete_block(game.get_block());
			game.move_block_down();
			//print_block(game.get_block());

			
			//check if lines should be cleared
			//clear lines if so
			//check if game over
		}

		print_game(game);
		
		
		refresh();
	}
	endwin();
	return 0;
}
