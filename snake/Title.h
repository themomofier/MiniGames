#include <ncurses.h>
#include <ctime>
#include <cmath>
using namespace std;

void wait_ticks(int ticks) {
	clock_t start = clock();
	while ((clock() - start) < ticks)
		; //Do nothing
}

void words(int frame) {
	int x = 120-frame, y = 0, line = 0;
			mvprintw(y+line++,x,"  													");
			mvprintw(y+line++,x," .d88888b                    dP                 	");
			mvprintw(y+line++,x," 88.    ''                   88                	");
			mvprintw(y+line++,x," `Y88888b. 88d888b. .d8888b. 88  .dP  .d8888b. 	");
			mvprintw(y+line++,x,"       `8b 88'  `88 88'  `88 88888'   88ooood8 	");
			mvprintw(y+line++,x," d8'   .8P 88    88 88.  .88 88  `8b. 88.  ... 	");
			mvprintw(y+line++,x,"  Y88888P  dP    dP `88888P8 dP   `YP `88888P' 	");
			mvprintw(y+line++,x," oooooooooooooooooooooooooooooooooooooooooooooo	");
			mvprintw(y+line++,x," 										 			");
			mvprintw(y+line++,x,"                          .-=-.           .--.		");
			mvprintw(y+line++,x,"            __         .'      '.        /  '  )	");
			mvprintw(y+line++,x,"   _      .'   '.     /   .-.    .      /   .-'  	");
			mvprintw(y+line++,x,"  (  '   /  .-.  '   /   /    .   .    /   /      	");
			mvprintw(y+line++,x,"   '  `-`  /   '  `-'   /      .    `-`   /		");
			mvprintw(y+line++,x,"     `-.-`      '.____.'        `..____ .'			");
			mvprintw(y+line++,x,"   ");
			mvprintw(y+line++,x," 	");
			mvprintw(y+line++,x," 	");
			mvprintw(y+line++,x,"   ");
			mvprintw(y+line++,x,"   ");
			mvprintw(y+line++,x,"   ");
			mvprintw(y+line++,x,"   ");
			mvprintw(y+line++,x,"");
			mvprintw(y+line++,x,"");
}

void splash() {
	int x = 0, y = 0; //Holds offsets to draw the ASCII art
	for (int i = 0; i < 120; i++) {
		timeout(10);
		int ch = getch(); //Allow them to hit a key to skip the splash screen
		if (ch != ERR) return;
		
		//Redraw splash screen
		clear();
		words(i);
		refresh();

		wait_ticks(0.05 * CLOCKS_PER_SEC);
	}
	wait_ticks(0.75 * CLOCKS_PER_SEC);
	clear();
}
