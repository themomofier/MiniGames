#include <iostream>
using namespace std;

void Pong();
void Snake()
void Tetris();

int main(){
	string user_input = "";
	char indicator = 0;
	bool loop = true;

	cout << "What would you like to play?\nMinesweeper\nSnake\nTetris\nBlock breakers\n>";
	getline(cin, user_input);

	indicator = toupper(user_input.at(0));
	
	while(loop){	
		switch(indicator){
			case 'M':
				//start Minesweeper
				break;
			case 'P':
				Pong();
				break;
			case 'S':
				//start Snake
				Snake();
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
