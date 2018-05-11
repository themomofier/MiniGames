#include <curses.h>
#include <iostream>
#include <cmath>
#include <ctime>
#include <sstream>
#include <unistd.h>
#include "internet_server.h"
#include "internet_client.h"
using namespace std;

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

int height, width;
int startx = 0, starty = 0, ch;
int p1_score = 0, p2_score = 0;

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

struct paddle {
	char a = 'O';
	motion position;
	paddle(motion new_pos):position(new_pos){}
	void update(char b){
		if(abs(position.x)-width >= 0) { ; }
		else if(b == 'a') { position.y -= 1; }
		else if(b == 's') { position.y += 1; }
	}
	void del_pad(WINDOW *my_win){	
		for(int i = 0; i < 3; i++){	
			mvwaddch(my_win, position.y + i, position.x, ' ');
		}

	}
	void print(WINDOW *my_win){
		for(int i = 0; i < 3; i++){
			mvwaddch(my_win, position.y + i, position.x, a);
		}
	}
};


struct ball {
	char ball_no = '@';
	motion position;
	motion velocity;
	ball(motion new_pos, motion new_vel): position(new_pos), velocity(new_vel) {}
	void update(paddle pad, paddle opp) {
		if((position.x == pad.position.x +1)&&((position.y == pad.position.y)||\
		(position.y == pad.position.y +1)||(position.y == pad.position.y +2))) {
			velocity.x *= -1;
		}
		else if((position.x == opp.position.x -1)&&((position.y == opp.position.y)||\
		(position.y == opp.position.y +1)||(position.y == opp.position.y +2))) {
			velocity.x *= -1;
		}
		if((position.x == 1)||(position.x == (width - 2))) { 
			if (position.x == 1) p2_score++;
			else p1_score++;
			position.x = width/2;
			position.y = height/2;
		}
		position += velocity;
		motion overshoot = motion((width-1) - abs(position.x), (height - 3) - position.y);
		if ((overshoot.x == 1)||(overshoot.x == (width - 2))) {
			velocity.x *= -1;
		}
		else if ((overshoot.y <= -1) || (overshoot.y >= (height - 4))) {
			velocity.y *= -1;
		}

	}
};
void Pong() {
	//add user input for this
	char mode;
	cin >> mode;
	string host = "localhost";
	string port = "1026";
	Internet_Server server;
	Internet_Client client;
	if(mode == 'c'){
		server.start(1026);
	}else if (mode == 'j'){
		client.start(host.c_str(), port.c_str());
	}
	stringstream outgoing;
	stringstream incoming;
	bool ignore;

	srand(time(0));
	int row, col;
	WINDOW *my_win;
	initscr();
	curs_set(0);
	//edit
	if(mode == 's') getmaxyx(stdscr, row, col);
	else { row = 50; col = 200;}

	cbreak();
	width = (col-1) /2;
	height = (row-1) /2;
	motion pos(2 , height / 2);
	motion pos1(width - 4, height /2);
	paddle my_pad(pos);
	paddle my_opp(pos1);
	motion position(4, height / 2);
	motion velocity(1,1);
	ball my_ball(position, velocity);
	bool quit = false;
	clear();
	keypad(stdscr, TRUE);
	noecho();
	refresh();
	my_win = create_newwin(height, width, starty, startx);
	mvwaddch(my_win, my_ball.position.y, my_ball.position.x, my_ball.ball_no);
	my_pad.print(my_win);	
	my_opp.print(my_win);	
	wrefresh(my_win);
	refresh();
	ch = getch();
	timeout(100);
	while(!quit){	
		mvprintw(height + 2, width /2, "Player 1: %u", p1_score);
		mvprintw(height + 3, width /2, "Player 2: %u", p2_score);
		mvwaddch(my_win, my_ball.position.y, my_ball.position.x, ' ');

		ch = getch();
		if(ch == 'q') quit = true;
		my_pad.del_pad(my_win);
		my_opp.del_pad(my_win);
		//update my pad
		if(ch == KEY_UP)   { 
			if(my_pad.position.y == 1) { ; }
			else { --my_pad.position.y; }
		}
		if(ch == KEY_DOWN) {
			if(my_pad.position.y == (height - 4)) { ; }
			else { ++my_pad.position.y; }
		}

		outgoing.str("");
		//write my pad and quit.
		if(mode != 's'){
			outgoing << quit << " " << my_pad.position.y << " ";
		}
		//write ball
		if(mode == 'c'){
			outgoing << width - 2 - my_ball.position.x << " " << my_ball.position.y << " ";
		}
		//write
		if(mode == 'c'){
			server.write(outgoing.str());
		}else if(mode == 'j'){
			client.write(outgoing.str());
		}

		//read
		if(mode == 'c'){
			incoming.str(server.read());
		}else if(mode == 'j'){
			incoming.str(client.read());
		}

		//update opponent pad and quit
		if(mode == 's'){	
			if(my_ball.position.y >= (height - 4)) { ; }
			else if(my_ball.position.y == 1) { ; }
			else {
				int direction = 0;
				if (my_ball.position.y < my_opp.position.y)
					direction = -1;
				else if (my_ball.position.y > my_opp.position.y)
					direction = 1;
				if (rand() % 4 == 0) direction *= -1;
				my_opp.position.y += direction;
			}
		}else{
			if(!quit) incoming >> quit;
			else incoming >> ignore;
			incoming >> my_opp.position.y;
		}	
		
		//update ball
		if(mode == 'c' || mode == 's'){
			my_ball.update(my_pad, my_opp);
		}else{
			incoming >> my_ball.position.x;
			incoming >> my_ball.position.y;
		}

		//print
		my_pad.print(my_win);	
		my_opp.print(my_win);	
		mvwaddch(my_win, my_ball.position.y, my_ball.position.x, my_ball.ball_no);
		refresh();
		wrefresh(my_win);
	}
	endwin();
}

WINDOW *create_newwin(int height, int width, int starty, int startx) {
	WINDOW *local_win;
	local_win = newwin(height, width, starty, startx);
	wborder(local_win, ' ', ' ', 'X', 'X', 'X', 'X', 'X', 'X');
	touchwin(local_win);
	wrefresh(local_win);
	return local_win;
}

void destroy_win(WINDOW *local_win) {
	wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wrefresh(local_win);
	delwin(local_win);
}

/*
void score(ball my_ball){
	if(my_ball.position.x == 1) { ++p1_score; }
	else if(my_ball.position.x == (width - 2)) { ++p2_score; }
	start();
}
*/
