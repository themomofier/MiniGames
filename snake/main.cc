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
//#include "splash.h"
using namespace std;

const int SIZE_X = 30;
const int SIZE_Y = 30;  
const unsigned int TIMEOUT = 200; 
const int UP = 65; 
const int DOWN = 66;
const int LEFT = 68;
const int RIGHT = 67;

int main()
{
	srand(time(NULL));
	initscr();
	start_color();
	init_pair(1,COLOR_WHITE,COLOR_BLACK);
	init_pair(2,COLOR_CYAN,COLOR_BLACK);
	init_pair(3,COLOR_GREEN,COLOR_BLACK);
	init_pair(4,COLOR_YELLOW,COLOR_BLACK);
	init_pair(5,COLOR_RED,COLOR_BLACK);
	init_pair(6,COLOR_MAGENTA,COLOR_BLACK);
	//clear();
	noecho();
	//cbreak();
	curs_set(0);
	timeout(TIMEOUT);

	//Splash Screen
	//splash();
	//timeout(TIMEOUT);
	//clear();

	bool game_over = false;
	int direction = 2;
	int apple_x = rand() % SIZE_X + 1;
	int apple_y = rand() % SIZE_Y + 1;
		
	Body snakes; //List holding the snake
	for (int i = 0; i < 4; i++) 
		snakes.add_front((SIZE_X/2), (SIZE_Y/2) + i); //Generating initial snake

//	draw_board();
	
	//Will loop until game ends
	while (!game_over) {
		//Printing stuff
		mvprintw(SIZE_X+1,0,"Use arrow keys to move the cursor around");
		
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

        Snake* path = snakes.get_front();
		int path_x = path->get_x();
		int path_y = path->get_y();
		
		//Movement
		if (direction == 1) path_x--;
		else if (direction == 2) path_y++;
		else if (direction == 3) path_x++;
		else if (direction == 4) path_y--;
		
		snakes.add_front(path_x, path_y);
		
		//Check for apple
		if (path_x == apple_y && path_y == apple_x) {
			//Respawn apple
			apple_x = rand() % SIZE_X + 1;
			apple_y = rand() % SIZE_Y + 1;
			//Add points
		}
		else {
			snakes.pop_back();
		}

		erase();
		mvaddch(apple_y, apple_x, 'o');

		//Tracking snake position
		Snake* curr = snakes.get_front();
		while (curr) {
			mvaddch(curr->get_x(), curr->get_y(), 'O');
			curr = curr->get_next();
			//Collision check
		}
		refresh();
	}
//	clear();
//	print_board();
	timeout(-1);
	erase();
	refresh();
	//wait_ticks(300000);
	endwin(); // End curses mode
	system("clear");

	return 0;
}
