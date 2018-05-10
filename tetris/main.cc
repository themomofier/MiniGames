#include "Game.h"
#include <iostream>
#include <ctime>
#include <ncurses.h>
#include <sstream>
#include <chrono>
using namespace std;

const double DELAY = 0.5; //seconds
const int GRID_POS_X = 0;
const int GRID_POS_Y = 0;
const int SQUARE_WIDTH = 4;
const int SQUARE_HEIGHT = 2;
const char SQUARE[SQUARE_HEIGHT][SQUARE_WIDTH] = 
	{{'#', '#', '#', ' '},
	 {'#', '#', '#', ' '}};
	 
const char EMPTY_SQUARE[SQUARE_HEIGHT][SQUARE_WIDTH] = 
	{{' ', ' ', ' ', '|'},
	 {' ', ' ', ' ', '|'}};

const char BORDER = 'H';
const int BORDER_COLOR = 9;
const int CONTENT_COLOR = 8;

void print_border(){
	attron(COLOR_PAIR(BORDER_COLOR));
	for(int x = 0; x < GRID_WIDTH * SQUARE_WIDTH + 2; x++){
		mvaddch(0, x, BORDER);
		mvaddch(GRID_HEIGHT * SQUARE_HEIGHT + 1, x, BORDER);
	}
	for(int y = 1; y < GRID_HEIGHT * SQUARE_HEIGHT + 1; y++){
		mvaddch(y, 0, BORDER);
		mvaddch(y, GRID_WIDTH * SQUARE_WIDTH + 1, BORDER);
	}
	attroff(COLOR_PAIR(BORDER_COLOR));
}

void print_game_over(int score, WINDOW* pop_up){

	stringstream ss;
	ss << score;
	
	wattron(pop_up, COLOR_PAIR(BORDER_COLOR));
	wborder(pop_up, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wattroff(pop_up, COLOR_PAIR(BORDER_COLOR));

	


	wattron(pop_up, COLOR_PAIR(CONTENT_COLOR));
	mvwprintw(pop_up, 1,  1, "                 ");
	mvwprintw(pop_up, 2,  1, "    GAME OVER!   ");
	mvwprintw(pop_up, 3,  1, "                 ");
	mvwprintw(pop_up, 5, 1, " your score was: ");
	mvwprintw(pop_up, 6, 1, "                 ");
	mvwprintw(pop_up, 6, 6, ss.str().c_str());
	mvwprintw(pop_up, 8, 1, "  press any key  ");
	mvwprintw(pop_up, 9, 1, "    to restart   ");
	wattroff(pop_up, COLOR_PAIR(CONTENT_COLOR));
	
}

void print_square(int y_coord, int x_coord, Square sqr, WINDOW* game_win, bool del = false){
	if(sqr.present){
		wattron(game_win, COLOR_PAIR(sqr.color + 1));
		for(int y = 0; y < SQUARE_HEIGHT; y++){
			for(int x = 0; x < SQUARE_WIDTH; x++){
				mvwaddch(game_win,
						 y_coord * SQUARE_HEIGHT + y + 1, 
						 x_coord * SQUARE_WIDTH + x + 1, 
						 SQUARE[y][x]);
			}
		}
		wattroff(game_win, COLOR_PAIR(sqr.color + 1));
	}else if(del){
		wattron(game_win, COLOR_PAIR(CONTENT_COLOR));
		for(int y = 0; y < SQUARE_HEIGHT; y++){
			for(int x = 0; x < SQUARE_WIDTH; x++){
				mvwaddch(game_win,
						 y_coord * SQUARE_HEIGHT + y + 1, 
						 x_coord * SQUARE_WIDTH + x + 1, 
						 EMPTY_SQUARE[y][x]);
			}
		}
		wattroff(game_win, COLOR_PAIR(CONTENT_COLOR));
	}	
}

void print_block(Block block, WINDOW* game_win){
	for(int y = 0; y < 4; y++){
		for(int x = 0; x < 4; x++){
			print_square(block.get_y() + y, block.get_x() + x, 
						 {block.get_shape()[y][x], block.get_type()},
						 game_win);
		}
	}
}

void print_game(Game game, WINDOW* game_win){

	for(int y = 0; y < GRID_HEIGHT; y++){
		for(int x = 0; x < GRID_WIDTH; x++){
			print_square(y, x, game.get_grid()[y][x], game_win, true);
		}
	}
	
	print_block(game.get_block(), game_win);

	wrefresh(game_win);
}

void Tetris(){
	
	bool quit = false;
	initscr();
	refresh();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	start_color();
	
	init_pair(1, COLOR_RED, COLOR_RED);       //corresponds with block type 0
	init_pair(2, COLOR_GREEN, COLOR_GREEN);
	init_pair(3, COLOR_YELLOW, COLOR_YELLOW);
	init_pair(4, COLOR_BLUE, COLOR_BLUE);
	init_pair(5, COLOR_MAGENTA, COLOR_MAGENTA);
	init_pair(6, COLOR_CYAN, COLOR_CYAN);
	init_pair(7, COLOR_WHITE, COLOR_WHITE);
	clear();
	init_pair(CONTENT_COLOR, COLOR_WHITE, COLOR_BLACK);
	init_pair(BORDER_COLOR, COLOR_BLUE, COLOR_BLUE);

	refresh();
	WINDOW *game_win = newwin(GRID_HEIGHT * SQUARE_HEIGHT + 2,
							  GRID_WIDTH * SQUARE_WIDTH + 2,
							  GRID_POS_Y, 
							  GRID_POS_X);
	wattron(game_win, COLOR_PAIR(BORDER_COLOR));
	wborder(game_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wattroff(game_win, COLOR_PAIR(BORDER_COLOR));
	wrefresh(game_win);

	chrono::system_clock::time_point start, end;
	//clock_t start, end;
	//time_t start, end
	int diff;
	int c;
	
	while(!quit){
	
		Game game;
		//print_game(game, game_win);
		timeout(0);
		double multiplier = 1;
		start = chrono::system_clock::now();
		//start = clock();
	
		while(!game.is_game_over() && !quit){
			c = getch();
			if(c != ERR){
				switch(c){
					case 'q':
					case 'Q':
						quit = true;
						break;
					case KEY_UP:
						game.rotate_block();
						break;
					case KEY_LEFT:
						game.move_block_left();
						break;
					case KEY_RIGHT:
						game.move_block_right();
						break;
					case KEY_DOWN:
						game.move_block_down();
						break;
					case ' ':
						game.drop_block();
						break;
				}
			}
			
			end = chrono::system_clock::now();
			diff = chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			//end = clock();
			//diff = double(end - start);
			
			if(diff >= DELAY * 1000000 /*/ multiplier*/){
				start = end;
				game.move_block_down();
				//multiplier += 0.006;
			}

			print_game(game, game_win);
		}
		
		quit = false;
		WINDOW* pop_up = newwin( 19, 9,
								(SQUARE_HEIGHT * GRID_HEIGHT + 2 - 9) / 2,
								(SQUARE_WIDTH * GRID_WIDTH + 2 - 19) / 2);
		print_game_over(game.get_score(), pop_up);
		timeout(-1);
		c = getch();
		if(c == 'q' || c == 'Q'){
			quit = true;
		}		
		delwin(pop_up);
	}
	endwin();
}
