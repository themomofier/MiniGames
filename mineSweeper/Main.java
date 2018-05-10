import java.util.Random;
import java.util.Scanner;

public class Main{

	public static final int BOARD_SIZE = 10;


	public static void main(String[]args){
		Random bombGen =  new Random();
		Scanner keys = new Scanner(System.in);
		
		Button[][] board = new Button[BOARD_SIZE][BOARD_SIZE];
		boolean[][] isBomb = new boolean[BOARD_SIZE][BOARD_SIZE];
		boolean gameOver = false;
		boolean left, right, top, bot;
		left = right = top = bot = false;

		int i, j;
		
		for(i = 0; i < BOARD_SIZE; i++){
			for(j = 0; j < BOARD_SIZE; j++){
				isBomb[i][j] = false;
			}
		}
		

		int x, y;
		
		for(i = 0; i < 20; i++){
			int temp = bombGen.nextInt(BOARD_SIZE * BOARD_SIZE);
			x = temp / BOARD_SIZE;
			y = temp % BOARD_SIZE;
			if(!isBomb[x][y]){
				board[x][y] = new Bomb();
				isBomb[x][y] = true;
			}
			else i--;
		}
		
		for(i = 0; i < BOARD_SIZE; i++){
			for(j = 0; j < BOARD_SIZE; j++){
				if(!isBomb[i][j]) board[i][j] = new Button();
			}
		
		}
		for(i = 0; i < BOARD_SIZE; i++){
			top = bot = false;
			if(i > 0) 				top = true;
			if(i < BOARD_SIZE - 1) 	bot = true;

			for(j = 0; j < BOARD_SIZE; j++){
				right = left = false;
				if (j > 0) 				left = true;
				if (j < BOARD_SIZE - 1) right = true;
				
				if(!isBomb[i][j]){
					if(top){
						if(isBomb[i-1][j])            board[i][j].increment();
						if(right && isBomb[i-1][j+1]) board[i][j].increment();
						if(left && isBomb[i-1][j-1])  board[i][j].increment();
					}
					if(right && isBomb[i][j+1]) board[i][j].increment();
					if(left && isBomb[i][j-1])  board[i][j].increment();
					if(bot){
						if(isBomb[i+1][j])            board[i][j].increment();
						if(right && isBomb[i+1][j+1]) board[i][j].increment();
						if(left && isBomb[i+1][j-1])  board[i][j].increment();
					}
				}
			}
		}

		System.out.println("MINESWEEPER\nSelect points by entering an ordered pari with no commas\nEX: 5 8\n");
		
		do{
			try{
				printBoard(board);
				System.out.print("Next selection: ");
				x = keys.nextInt();
				y = keys.nextInt();
				keys.nextLine();
				if(x >= BOARD_SIZE || y >= BOARD_SIZE) System.out.println("Invalid input\n");
				else
					gameOver = board[y][x].select();
			}
			catch(Exception e){
				gameOver = true;
			}
		}while(!gameOver);
		
		System.out.print("\033[H\033[2J");
		System.out.flush();
		System.out.println("      GAME OVER\n");
		
		for(i = 0; i < BOARD_SIZE; i++){
			for(j = 0; j < BOARD_SIZE; j++){
				if(isBomb[i][j]) System.out.print(" X");
				else System.out.print(" " + board[i][j].getBombCount());
			}
			System.out.println();
		}
		System.out.println("\n\n");
	}
	
	public static void printBoard(Button[][] board){
		//system("CLEAR");
		System.out.print("\033[H\033[2J");
		System.out.flush();
		System.out.print(" ");
		for(int i = 0; i < BOARD_SIZE; i++) System.out.print(" " + i);
		System.out.println();
		for(int i = 0; i < BOARD_SIZE; i++){
			System.out.print(i);
			for(int j = 0; j < BOARD_SIZE; j ++)  board[i][j].Display();
			System.out.println();
		}
	}
}
