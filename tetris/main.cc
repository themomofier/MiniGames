#include "Game.h"
#include <iostream>
#include <ctime>
#include <ncurses.h>
#include <sstream>
#include <chrono>
using namespace std;

const double DELAY = 0.5; //seconds
const int GRID_POS_X = 1;
const int GRID_POS_Y = 1;
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
	mvwprintw(pop_up, 4,  1, "                 ");
	mvwprintw(pop_up, 7,  1, "                 ");
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
	wrefresh(pop_up);
	
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

void print_side_win(Game game, WINDOW* side_win){
	Block next_block = game.get_next_block();
	for(int y_coord = 0; y_coord < 2; y_coord++){ //block only 2 tall max at orientation 0
		for(int x_coord = 0; x_coord < 4; x_coord++){
			if(next_block.get_shape()[y_coord][x_coord]){
				wattron(side_win, COLOR_PAIR(next_block.get_type() + 1));
				for(int y = 0; y < SQUARE_HEIGHT; y++){
					for(int x = 0; x < SQUARE_WIDTH; x++){
						mvwaddch(side_win,
								 y_coord * SQUARE_HEIGHT + y + 2, 
								 x_coord * SQUARE_WIDTH + x + 2, 
								 SQUARE[y][x]);
					}
				}
				wattroff(side_win, COLOR_PAIR(next_block.get_type() + 1));
			}else{
				wattron(side_win, COLOR_PAIR(CONTENT_COLOR));
				for(int y = 0; y < SQUARE_HEIGHT; y++){
					for(int x = 0; x < SQUARE_WIDTH; x++){
						mvwaddch(side_win,
								 y_coord * SQUARE_HEIGHT + y + 2, 
								 x_coord * SQUARE_WIDTH + x + 2, 
								 ' ');
					}
				}
				wattroff(side_win, COLOR_PAIR(CONTENT_COLOR));
			}
		}
	}

	for(int x = 1; x < 4 * SQUARE_WIDTH + 3; x++){
		wattron(side_win, COLOR_PAIR(BORDER_COLOR));
		mvwaddch(side_win, 2 * SQUARE_HEIGHT + 3, x, BORDER);
		wattroff(side_win, COLOR_PAIR(BORDER_COLOR));
	}
	wattron(side_win, COLOR_PAIR(CONTENT_COLOR));
	mvwprintw(side_win, 2 * SQUARE_HEIGHT + 4, 2, "Score: %d", game.get_score());
	wattroff(side_win, COLOR_PAIR(CONTENT_COLOR));
	
	wrefresh(side_win);
}

void print_directions(){
	int start_y = SQUARE_HEIGHT * GRID_HEIGHT + 2 + GRID_POS_Y;
	mvprintw(start_y, GRID_POS_X, "TETRIS"); 
	mvprintw(start_y + 1, GRID_POS_X, "Rotate block: up arrow"); 
	mvprintw(start_y + 2, GRID_POS_X, "Move block left: left arrow"); 
	mvprintw(start_y + 3, GRID_POS_X, "Move block right: right arrow"); 
	mvprintw(start_y + 4, GRID_POS_X, "Move block down: down arrow"); 
	mvprintw(start_y + 5, GRID_POS_X, "Drop block: space bar"); 
	mvprintw(start_y + 6, GRID_POS_X, "Quit: q"); 
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
	WINDOW *side_win = newwin(2 * SQUARE_HEIGHT + 6,
							  4 * SQUARE_WIDTH + 4,
							  GRID_POS_Y, 
							  GRID_POS_X + GRID_WIDTH * SQUARE_WIDTH + 2 + 1);
	wattron(side_win, COLOR_PAIR(BORDER_COLOR));
	wborder(side_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wattroff(side_win, COLOR_PAIR(BORDER_COLOR));
	wrefresh(side_win);

	print_directions();

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
			print_side_win(game, side_win);
			print_game(game, game_win);
		}
		
		quit = false;
		WINDOW* pop_up = newwin( 11, 19,
								(SQUARE_HEIGHT * GRID_HEIGHT + 2 - 9) / 2 + GRID_POS_Y,
								(SQUARE_WIDTH * GRID_WIDTH + 2 - 19) / 2 + GRID_POS_X);
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
