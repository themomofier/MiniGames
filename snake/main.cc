#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <ncurses.h>
#include <utility>
#include <limits>
#include <vector>
#include <queue>
#include <set>
#include "Snake.h"
#include "Title.h"
using namespace std;

const int SIZE_X = 40;
const int SIZE_Y = 30;  
const unsigned int TIMEOUT = 200; 
const int UP = 65; 
const int DOWN = 66;
const int LEFT = 68;
const int RIGHT = 67;
int apple_x = rand() % SIZE_X + 1;
int apple_y = rand() % SIZE_Y + 1;
Body snakes;

void draw_board() {
	attron(COLOR_PAIR(7));
	for (int i = 0; i <= SIZE_X; i++) {
		mvaddch(0, i, ' ' | A_REVERSE);
		mvaddch(SIZE_Y, i, ' ' | A_REVERSE);
	}
	for (int i = 0; i <= SIZE_Y; i++) {
		mvaddch(i, 0, ' ' | A_REVERSE);
		mvaddch(i, SIZE_X, ' ' | A_REVERSE);
	}
	attroff(COLOR_PAIR(7));
}

void draw_snake() {
	Snake* curr = snakes.get_front();
	while (curr) {
		attron(COLOR_PAIR(3));
		mvaddch(curr->get_y(), curr->get_x(), ' ' | A_REVERSE);
		curr = curr->get_next();
		
		//Collision check
		
		attroff(COLOR_PAIR(3));
	}
}

void draw_apple(int y, int x) {
	attron(COLOR_PAIR(5));
	mvaddch(y, x, ' '| A_REVERSE);
	attroff(COLOR_PAIR(5));
}

int main()
{
	bool game_over = false;
	int direction = 2;
	int points = 0;

	srand(time(NULL));
	initscr();
	start_color();
	init_pair(1,COLOR_WHITE,COLOR_BLACK);
	init_pair(2,COLOR_CYAN,COLOR_BLACK);
	init_pair(3,COLOR_GREEN,COLOR_BLACK);
	init_pair(4,COLOR_YELLOW,COLOR_BLACK);
	init_pair(5,COLOR_RED,COLOR_BLACK);
	init_pair(6,COLOR_MAGENTA,COLOR_BLACK);
	init_pair(7,COLOR_BLUE,COLOR_BLACK);
	clear();
	noecho();
	cbreak();
	curs_set(0);
	timeout(TIMEOUT);

	//Splash Screen
	splash();
	timeout(TIMEOUT);
	clear();
		
	//Initial Snake
	for (int i = 0; i < 4; i++) 
		snakes.add_front((SIZE_X/2) + i, (SIZE_Y/2)); 
	
	while (!game_over) {
		draw_board();
		mvprintw(SIZE_Y + 1, 0, "Points: %i", points);
		//Movement cases
		int ch = getch(); // Wait for user input, with TIMEOUT delay
		switch(ch) {
			case UP:
				direction = 1;	
				break;
			case RIGHT:
				direction = 2;
				break;
			case DOWN:
				direction = 3;
				break;
			case LEFT:
				direction = 4;
				break;
			case 'q': 
				game_over = true;
				break;
		}

		//Determine path
        Snake* path = snakes.get_front();
		int path_x = path->get_x();
		int path_y = path->get_y();
		
		//Movement
		if (direction == 1) path_y--;
		else if (direction == 2) path_x++;
		else if (direction == 3) path_y++;
		else if (direction == 4) path_x--;

		snakes.add_front(path_x, path_y);

		//Check for apple
		if (path_x == apple_x && path_y == apple_y) {
			
			//Respawn apple
			apple_x = rand() % (SIZE_X - 1) + 1;
			apple_y = rand() % (SIZE_Y - 1) + 1;
			
			//Add points
			points++;
		}
		else {
			snakes.pop_back();
		}

		//Collision
		if (path_y == SIZE_Y || path_x == SIZE_X || path_y == 0 || path_x == 0)
	      game_over = true;

		erase();	
		draw_apple(apple_y, apple_x);
		draw_snake();
		refresh();
	}

	timeout(-1);
	endwin(); // End curses mode
	return 0;
}
