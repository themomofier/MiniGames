#include <iostream>
#include <ctime>
#include <ncurses.h>
#include <chrono>
#include <set>
#include <sstream>
#include "Snake.h"
#include "Title.h"
using namespace std;

const int SIZE_X = 40;
const int SIZE_Y = 30;  
const unsigned int TIMEOUT = 120; 
const char BORDER = ' ';
const char SNAKE = ' ';
const char APPLE = ' ';
int apple_x = rand() % SIZE_X + 1;
int apple_y = rand() % SIZE_Y + 1;
bool game_over = false;
Body snakes;

void draw_board() {
	attron(COLOR_PAIR(7));
	for (int i = 0; i <= SIZE_X; i++) {
		mvaddch(0, i, BORDER | A_REVERSE);
		mvaddch(SIZE_Y, i, BORDER | A_REVERSE);
	}
	for (int i = 0; i <= SIZE_Y; i++) {
		mvaddch(i, 0, BORDER | A_REVERSE);
		mvaddch(i, SIZE_X, BORDER | A_REVERSE);
	}
	attroff(COLOR_PAIR(7));
}

void draw_snake(int x, int y) {
	Snake* curr = snakes.get_front();
	while (curr) {
		attron(COLOR_PAIR(3));
		
		mvaddch(curr->get_y(), curr->get_x(), SNAKE | A_REVERSE);
		if (curr->get_x() == x && curr->get_y() == y && curr != snakes.get_front()) game_over = true;
		curr = curr->get_next();
		attroff(COLOR_PAIR(3));
	}
}

void draw_apple(int y, int x) {
	attron(COLOR_PAIR(5));
	mvaddch(y, x, APPLE | A_REVERSE);
	attroff(COLOR_PAIR(5));
}

void print_game_over(int points){
	int min_x = SIZE_X / 2 - 9;
	int min_y = SIZE_Y / 2 - 6;

	stringstream ss;
	ss << points;
	attron(COLOR_PAIR(7));
	for(int x = 0; x < 19; x++){
		mvaddch(min_y, min_x + x, BORDER | A_REVERSE);
		mvaddch(min_y + 4, min_x + x, BORDER | A_REVERSE);
		mvaddch(min_y + 7, min_x + x, BORDER | A_REVERSE);
		mvaddch(min_y + 10, min_x + x, BORDER | A_REVERSE);
	}
	
	for(int y = 0; y < 11; y++){
		mvaddch(min_y + y, min_x, BORDER | A_REVERSE);
		mvaddch(min_y + y, min_x + 18, BORDER | A_REVERSE);
	}
	attroff(COLOR_PAIR(7));
									
	attron(COLOR_PAIR(1));
	mvprintw(min_y + 1,  min_x + 1, "                 ");
	mvprintw(min_y + 2,  min_x + 1, "    GAME OVER!   ");
	mvprintw(min_y + 3,  min_x + 1, "                 ");
	mvprintw(min_y + 5, min_x + 1, 	" Your score was: ");
	mvprintw(min_y + 6, min_x + 1, 	"                 ");
	mvprintw(min_y + 6, min_x + 6, ss.str().c_str());
	mvprintw(min_y + 8, min_x + 1, 	"  press any key  ");
	mvprintw(min_y + 9, min_x + 1, 	"    to restart   ");
	attroff(COLOR_PAIR(1));																			
}

int main() {
	//bool quit = false;
	int direction = 2;
	int points = 0;

	srand(time(NULL));
	initscr();
	refresh();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	start_color();

	init_pair(1,COLOR_WHITE,COLOR_BLACK);
	init_pair(2,COLOR_CYAN,COLOR_BLACK);
	init_pair(3,COLOR_GREEN,COLOR_BLACK);
	init_pair(4,COLOR_YELLOW,COLOR_BLACK);
	init_pair(5,COLOR_RED,COLOR_BLACK);
	init_pair(6,COLOR_MAGENTA,COLOR_BLACK);
	init_pair(7,COLOR_BLUE,COLOR_BLACK);
	
	//Splash Screen
	splash();
	timeout(TIMEOUT);
	clear();

	//Initial Snake
	draw_board();
	for (int i = 0; i < 4; i++)
    	snakes.add_front((SIZE_X/2) + i, (SIZE_Y/2));
	
	while (!game_over) {
		//draw_board();
		mvprintw(SIZE_Y + 1, 0, "Points: %i", points);
		
		//Movement cases
		int ch = getch(); // Wait for user input, with TIMEOUT delay
		if (ch == KEY_UP && direction != 3) 
			direction = 1;
		else if (ch == KEY_RIGHT && direction != 4) 
			direction = 2;
		else if (ch == KEY_DOWN && direction != 1) 
			direction = 3;
		else if (ch == KEY_LEFT && direction != 2) 
			direction = 4;
		else if (ch == 'q' || ch == 'Q')
			game_over = true;

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
		else snakes.pop_back();

		//Collision
		if (path_y == SIZE_Y || path_x == SIZE_X || path_y == 0 || path_x == 0)
	      game_over = true;
		
		erase();	
		draw_board();
		draw_apple(apple_y, apple_x);
		draw_snake(path_x, path_y);
		refresh();
	}

	timeout(-1);
	print_game_over(points);
	getch();
	refresh();
	endwin(); // End curses mode
	return 0;
}
