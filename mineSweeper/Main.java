import java.util.Random;
import java.util.Scanner;

public class Main{
	public static void main(String[]args){
		Random bombGen =  new Random();
		Scanner keys = new Scanner(System.in);
		
		Button[][] board = new Button[10][10];
		boolean[][] isBomb = new boolean[10][10];
		boolean gameOver = false;
		boolean left, right, top, bot;
		left = right = top = bot = false;

		int i, j;
		
		for(i = 0; i < 10; i++){
			for(j = 0; j < 10; j++){
				isBomb[i][j] = false;
			}
		}
		

		int x, y;
		
		for(i = 0; i < 20; i++){
			int temp = bombGen.nextInt(100);
			x = temp / 10;
			y = temp % 10;
			if(!isBomb[x][y]){
				board[x][y] = new Bomb();
				isBomb[x][y] = true;
			}
			else i--;
		}
		
		for(i = 0; i < 10; i++){
			for(j = 0; j < 10; j++){
				if(!isBomb[i][j]) board[i][j] = new Button();
			}
		
		}
		for(i = 0; i < 10; i++){
			top = bot = false;
			if(i > 0) top = true;
			if(i < 9) bot = true;

			for(j = 0; j < 10; j++){
				right = left = false;
				if (j > 0) left = true;
				if (j < 9) right = true;
				
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
			printBoard(board);
			System.out.print("Next selection: ");
			x = keys.nextInt();
			y = keys.nextInt();
			keys.nextLine();
			gameOver = board[y][x].select();
		}while(!gameOver);
		
		System.out.println("GAME OVER\n");
		
		for(i = 0; i < 10; i++){
			for(j = 0; j < 10; j++){
				if(isBomb[i][j]) System.out.print(" X");
				else System.out.print(" " + board[i][j].getBombCount());
			}
			System.out.println();
		}

	}
	
	public static void printBoard(Button[][] board){
		System.out.println(" 0 1 2 3 4 5 6 7 8 9 ");
		for(int i = 0; i < 10; i++){
			System.out.print(i);
			for(int j = 0; j < 10; j ++)  board[i][j].Display();
			System.out.println();
		}
	}
}
