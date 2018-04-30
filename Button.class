public class Button{
	protected boolean view = false;
	private int bombCount = 0;

	public Button(){}

	public void increment()  {bombCount++;}
	public boolean select()  {view = true; return false;}
	public int getBombCount(){return bombCount;}
	public boolean canView() {return view;}

	public void Display(){
		if(view) System.out.print(" " + bombCount);
		else System.out.print(" -");
	}
}
