#include <iostream>
using namespace std;

void Pong();
void snake();
void Tetris();

int main(){
	string user_input = "";
	char indicator = 0;
	bool loop = true;

	while(loop){	
		cout << "What would you like to play?\nMinesweeper\nPong\nSnake\nTetris\n>";
		getline(cin, user_input);
		indicator = toupper(user_input.at(0));
		switch(indicator){
			case 'M':
				//start Minesweeper
				system("java -classpath mineSweeper Main");
				break;
			case 'P':
				Pong();
				break;
			case 'S':
				//start Snake
				snake();
				break;
			case 'T':
				//start Tetris
				Tetris();
				break;
			case 'Q':
				loop = false;
				break;
			default:
				cout << "Invalid input, try again.\nQuit will exit the game.\n";
		}
	}
	return 0;
}
