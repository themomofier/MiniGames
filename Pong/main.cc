#include <curses.h>
#include <iostream>
#include <cmath>
#include <unistd.h>
using namespace std;

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

int height = 30, width = 50;
int startx = 0, starty = 0, ch;
bool game = true;

struct motion {
	int x = 0, y = 0;
	motion(int new_x, int new_y) {
		x = new_x;
		y = new_y;
	}
	void operator-= (const motion& b) {
		x -= b.x;
		y -= b.y;
	}
	void operator+= (const motion& b) {
		x += b.x;
		y += b.y;
	}
	motion operator+ (const motion& b) const {
		return motion(x + b.x, y + b.y);
	}
	motion operator-(const motion& b) const {
		return motion(x - b.x, y - b.y);
	}
	motion operator*(int var) {
		return motion(x * var, y * var);
	}
};

struct ball {
	char ball_no = '@';
	motion position;
	motion velocity;
	ball(motion new_pos, motion new_vel): position(new_pos), velocity(new_vel) {}
	void update() {
		position += velocity;
		motion overshoot = motion((width-1) - abs(position.x), (height - 3) - position.y);
		if ((overshoot.x == 1)||(overshoot.x == (width - 2))) {
			velocity.x *= -1;
			//position.x = (abs(position.x) - 2 * overshoot.x) * position.x / abs(position.x);
		}
		if ((overshoot.y <= -1) || (overshoot.y >= (height - 4))) {
			velocity.y *= -1;
			//position.y = (abs(position.y) - 2 * overshoot.y) * position.y / abs(position.y);
		}
	}
};

struct block {
	char paddle = '^';
	motion position;
	block(motion new_pos):position(new_pos){}
	void update(char b){
		if(abs(position.x)-width >= 0) { ; }
		else if(b == 'a') { position.x -= 1; }
		else if(b == 's') { position.x += 1; }
	}
};

void Pong() {
	int row, col;
	motion position1(width / 2, height - 2);
	//block paddle(position1);
	WINDOW *my_win;
	initscr();
	curs_set(0);
	getmaxyx(stdscr, row, col);
	cbreak();
	width = (col-1)/2;
	height = (row-1)/2;
	motion position(width / 2, height / 2);
	motion velocity(-1,1);
	ball my_ball(position, velocity);
	clear();
	keypad(stdscr, TRUE);
	noecho();
	refresh();
	my_win = create_newwin(height, width, starty, startx);
	while(true){	
		ch = getch();
		if(ch == 'q') break;
		mvwaddch(my_win, my_ball.position.y, my_ball.position.x, ' ');
		wrefresh(my_win);
		my_ball.update();
		mvwaddch(my_win, my_ball.position.y, my_ball.position.x, my_ball.ball_no);
		mvprintw(height, width,0, my_ball.position.x);
		refresh();
		wrefresh(my_win);
		timeout(100);
	}
	endwin();
}

WINDOW *create_newwin(int height, int width, int starty, int startx) {
	WINDOW *local_win;
	local_win = newwin(height, width, starty, startx);
	wborder(local_win, 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X');
	touchwin(local_win);
	wrefresh(local_win);
	return local_win;
}

void destroy_win(WINDOW *local_win) {
	wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wrefresh(local_win);
	delwin(local_win);
}
