#include <iostream>
#include <algorithm>
#include <ctime>
#include <ncurses.h>
#include <chrono>
#include <set>
#include <sstream>
#include <fstream>
#include <vector>
#include "Snake.h"
//#include "Title.h"
#include "../tree.h"
using namespace std;

const int SIZE_X = 40;
const int SIZE_Y = 20;  
const int TIMEOUT = 130; 
const int HS_MAX = 18;
const char BORDER = ' ';
const char SNAKE = ' ';
const char APPLE = ' ';

void draw_board() {
	attron(COLOR_PAIR(7));
	
	for (int i = 0; i <= SIZE_X; i++) {
		mvaddch(0, i, BORDER);
		mvaddch(SIZE_Y, i, BORDER);
	}
	for (int i = 0; i <= SIZE_Y; i++) {
		mvaddch(i, 0, BORDER);
		mvaddch(i, SIZE_X, BORDER);
	}
	for (int i = 0; i <= SIZE_X; i++) {
		mvaddch(SIZE_Y + 2, i, BORDER);
	}
	//Border for Highscores
	for (int i = 0; i < HS_MAX; i++) {
		mvaddch(0, SIZE_X + i, BORDER);
		mvaddch(SIZE_Y + 2, SIZE_X + i, BORDER);
	}
	for (int i = 0; i <= SIZE_Y + 2; i++) {
		mvaddch(i, SIZE_X + 1, BORDER);
		mvaddch(i, SIZE_X + 17, BORDER);
	}
	for (int i = 0; i < HS_MAX; i++) {
		mvaddch(2, SIZE_X + i, BORDER);
	}
	mvaddch(SIZE_Y + 1, 0, BORDER);
	mvaddch(SIZE_Y + 1, SIZE_X, BORDER);
	
	attroff(COLOR_PAIR(7));
}

void draw_snake(Body snakes, int y, int x) {
	Snake* curr = snakes.get_front();
	while (curr) {
		attron(COLOR_PAIR(3));
		mvaddch(curr->get_y(), curr->get_x(), SNAKE);
		curr = curr->get_next();
		attroff(COLOR_PAIR(3));
	}
}

void draw_apple(Body snakes, int &y, int &x) {
	attron(COLOR_PAIR(5));
	
	//Prevent spawn under snake
	Snake* curr = snakes.get_front();
	while (curr) {
		 if (curr->get_x() == x && curr->get_y() == y && curr != snakes.get_front()) { 
			 x = rand() % (SIZE_X - 1) + 1;
			 y = rand() % (SIZE_Y - 1) + 1;
		 }
		 curr = curr->get_next();
	}

	mvaddch(y, x, APPLE);
	attroff(COLOR_PAIR(5));
}

void print_game_over(int points){
	int min_x = SIZE_X / 2 - 9;
	int min_y = SIZE_Y / 2 - 6;

	stringstream ss;
	ss << points;
	attron(COLOR_PAIR(7));
	for(int x = 0; x < 19; x++){
		mvaddch(min_y, min_x + x, BORDER);
		mvaddch(min_y + 4, min_x + x, BORDER);
		mvaddch(min_y + 7, min_x + x, BORDER);
		mvaddch(min_y + 10, min_x + x, BORDER);
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
	mvprintw(min_y + 8, min_x + 1, 	"  Press any key  ");
	mvprintw(min_y + 9, min_x + 1, 	"    to quit.   ");
	attroff(COLOR_PAIR(1));																			
}

/*void save_score(string name, int points) {
	ofstream highscores ("Highscores", ios::app);
	if (highscores.is_open()) {
		if (points < 10 ) {
			highscores << name << "          00" << points << endl;
		}
		else if (points < 100) {
			highscores << name << "          0" << points << endl;
		}
		else 
			highscores << name << "          " << points << endl;
		highscores.close();
	}
}*/

void print_snake_highscores(BST<int> highscores) {
	int min_x = SIZE_X + 2;
	int min_y = 3;
	stringstream ss;
	ss.str(highscores.to_string());
	mvprintw(1, min_x, 		"   HIGSCORES");
	int next_score;
	while(ss >> next_score) {
		if (next_score < 10) {	
			mvprintw(min_y++, min_x, "      00%u", next_score);
		}
		else if (next_score < 100) {
			mvprintw(min_y++, min_x, "      0%u", next_score);
		}
		else
			mvprintw(min_y++, min_x, "      %u", next_score);
	}
}

void snake() {
	Body snakes;
	int apple_x = rand() % (SIZE_X - 1) + 1;
	int apple_y = rand() % (SIZE_Y - 1) + 1;
	bool game_over = false;
	int direction = 2;
	int points = 0;
	int speed = 130;
	int snake_speed = 1;
	BST<int> highscores("Snake_Highscores");


	srand(time(NULL));
	initscr();
	refresh();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	start_color();

	init_pair(1,COLOR_WHITE,COLOR_BLACK);
	init_pair(3,COLOR_GREEN,COLOR_GREEN);
	init_pair(4,COLOR_YELLOW,COLOR_BLACK);
	init_pair(5,COLOR_RED,COLOR_RED);
	init_pair(7,COLOR_BLUE,COLOR_BLUE);
	
	//Splash Screen
	//splash();
	timeout(TIMEOUT);
	clear();
	
	draw_board();

	//Initial Snake
	for (int i = 0; i < 5; i++)
    	snakes.add_front((SIZE_Y/2), (SIZE_X/2) + i);
	
	while (!game_over) {
		
		//Snake moves faster as points accumulate
		timeout(speed);
		if (points >= 15 && points < 30) {
			speed = 100;
			snake_speed = 2;
		}
		if (points >= 30 && points < 45) {
			speed = 86;
			snake_speed = 3;
		}
		if (points >= 45 && points < 60) {
			speed = 76;
			snake_speed = 4;
		}
		if (points >= 60) {
			speed = 68;
			snake_speed = 5;
		}

		//Display points
		if (points < 10)
			mvprintw(SIZE_Y + 1, SIZE_X - 12, "POINTS: 00%i", points);
		else if (points < 100)
			mvprintw(SIZE_Y + 1, SIZE_X - 12, "POINTS: 0%i", points);
		else
			mvprintw(SIZE_Y + 1, SIZE_X - 12, "POINTS: %i", points);
		
		//Display current speed
		mvprintw(SIZE_Y + 1, 2, "SPEED: %i", snake_speed);	
		
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
		if 		(direction == 1) path_y--;
		else if (direction == 2) path_x++;
		else if (direction == 3) path_y++;
		else if (direction == 4) path_x--;
		
		snakes.add_front(path_y, path_x);

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
		
		Snake* curr = snakes.get_front();
		while (curr) {
			if (curr->get_x() == path_x && curr->get_y() == path_y && curr != snakes.get_front()) game_over = true;
			curr = curr->get_next();
		}

		erase();	
		draw_board();
		draw_apple(snakes, apple_y, apple_x);
		draw_snake(snakes, path_y, path_x);
		print_snake_highscores(highscores);
		refresh();
	}

	print_game_over(points);
	if (points > 0) highscores.insert(points);
	print_snake_highscores(highscores);
	timeout(-1);
	getch();
	refresh();
	highscores.save("Snake_Highscores");
	endwin(); // End curses mode
}

