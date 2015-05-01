/**
 * CS 180 - Project 03 - RiverCrossingPuzzle
 * 
 * The game river crossing.
 * 
 * @author He Huang <huang654@purdue.edu>
 * 
 * @lab 814
 *
 * @date 10-07-2014
 */
import java.util.Scanner;

public class RiverCrossingPuzzle {
	private static int countMove = 0;
	private int cannibals = 0;
	private int missionaries = 0;
	private int boatSize = 0;
	private int numEach = 0;
	
	public RiverCrossingPuzzle(int numEach, int boatSize) {
		this.numEach = numEach;
		this.missionaries = numEach;
		this.cannibals = numEach;
		this.boatSize = boatSize;
	}
	
	public static void main(String[] args) {
		int n = Integer.parseInt(args[1]);
		int b = Integer.parseInt(args[3]);
		RiverCrossingPuzzle puzzle = new RiverCrossingPuzzle(n, b);
		puzzle.reset();
		puzzle.play();
	}
	
	// returns the number of missionaries on the left bank.
	public int numMissionariesOnLeftBank() {
		return this.missionaries;
	}
	
	// returns the number of missionaries on the right bank.
	public int numMissionariesOnRightBank() {
		return this.numEach - this.missionaries;
	}
	
	// returns the number of cannibals on the left bank.
	public int numCannibalsOnLeftBank() {
		return this.cannibals;
	}
	
	// returns the number of cannibals on the right bank.
	public int numCannibalsOnRightBank() {
		return this.numEach - this.cannibals;
	}
	
	// returns true if boat is on left (starting) bank
	public boolean boatOnLeftBank() {
		return (countMove % 2) == 0;
	}
	
	// returns true if boat is on right (ending) bank
	public boolean boatOnRightBank() {
		return (countMove % 2) != 0;
	}
	
	// returns a String containing all available actions for current puzzle
	// state.
	// Each action should be on its own line, preceded by an integer in
	// parentheses.
	// Example: (1) 2 Cannibals and 1 Missionary cross
	public String availableActions() {
		int c = 0;
		int tc;
		int tm;
		String availableActions = "";
		if (this.boatOnLeftBank()) {
			tc = this.numCannibalsOnLeftBank();
			tm = this.numMissionariesOnLeftBank();
		} else {
			tc = this.numCannibalsOnRightBank();
			tm = this.numMissionariesOnRightBank();
		}
		for (int i = 0; i <= tc; i++)
			for (int j = 0; j <= tm; j++)
				if (i + j <= this.boatSize && !(i == 0 && j == 0)) {
					c++;
					if ((i == 1 || i == 0) && (j == 1 || j == 0))
						availableActions = availableActions + "(" + c + ") " + i + " Cannibal and " + j
						        + " Missionary cross\n";
					else if (i == 1 || i == 0)
						availableActions = availableActions + "(" + c + ") " + i + " Cannibal and " + j
						        + " Missionaries cross\n";
					else if (j == 1 || j == 0)
						availableActions = availableActions + "(" + c + ") " + i + " Cannibals and " + j
						        + " Missionary cross\n";
					else
						availableActions = availableActions + "(" + c + ") " + i + " Cannibals and " + j
						        + " Missionaries cross\n";
				}
		return availableActions;
	}
	
	// move some people to the other side.
	// numCannibalsToMove: number of cannibals to move to the other side
	// numMissionariesToMove: number of missionaries to move to the other side
	public void move(int numCannibalsToMove, int numMissionariesToMove) {
		if ((numCannibalsToMove + numMissionariesToMove) > this.boatSize)
			return;
		if ((numCannibalsToMove < 0) || (numMissionariesToMove < 0))
			return;
		if ((numCannibalsToMove == 0) && (numMissionariesToMove == 0))
			return;
		if (boatOnLeftBank()) {
			this.cannibals = this.cannibals - numCannibalsToMove;
			this.missionaries = this.missionaries - numMissionariesToMove;
		} else {
			this.cannibals = this.cannibals + numCannibalsToMove;
			this.missionaries = this.missionaries + numMissionariesToMove;
		}
		countMove++;
	}
	
	// returns the total number of crossings so far.
	public int totalMoves() {
		return countMove;
	}
	
	// resets the game to the original state.
	public void reset() {
		countMove = 0;
		this.cannibals = this.numEach;
		this.missionaries = this.numEach;
	}
	
	// checks the status of the game.
	// returns -1 if puzzle faciled, 0 if puzzle not yet solved, 1 if puzzle
	// solved
	public int status() {
		if (this.missionaries == 0 && this.cannibals == 0)
			return 1;
		if (this.numMissionariesOnLeftBank() != 0 && this.numCannibalsOnLeftBank() > this.numMissionariesOnLeftBank())
			return -1;
		if (this.numMissionariesOnRightBank() != 0
		        && this.numCannibalsOnRightBank() > this.numMissionariesOnRightBank())
			return -1;
		return 0;
	}
	
	// facilitate a user interacting with the puzzle.
	public void play() {
		Scanner s = new Scanner(System.in);
		while (this.status() == 0) {
			System.out.println(this.puzzleState());
			System.out.println(this.prompt());
			int c = s.nextInt();
			int m = s.nextInt();
			move(c, m);
		}
		if (this.status() == 1) {
			System.out.println("You Win!!");
		}
		if (this.status() == -1) {
			System.out.println("You Lose!!");
		}
		s.close();
	}
	
	/**
	 * ***DO NOT CHANGE THIS FUNCTION.***
	 * 
	 * @return String containing the prompt for user input
	 */
	public String prompt() {
		String str = "";
		str += "Available Actions\n";
		str += availableActions();
		str += "Action: ";
		return str;
	}
	
	/**
	 * ***DO NOT CHANGE THIS FUNCTION.***
	 * 
	 * @return state of left (starting) bank as a String
	 */
	private String leftBank() {
		String str = "";
		for (int i = 0; i < numCannibalsOnLeftBank(); i++)
			str += "C";
		str += " ";
		for (int i = 0; i < numMissionariesOnLeftBank(); i++)
			str += "M";
		str += " ";
		if (boatOnLeftBank())
			str += "B";
		return str;
	}
	
	/**
	 * ***DO NOT CHANGE THIS FUNCTION.***
	 * 
	 * @return state of right (ending) bank as a String
	 */
	private String rightBank() {
		String str = "";
		if (boatOnRightBank())
			str += "B ";
		for (int i = 0; i < numCannibalsOnRightBank(); i++)
			str += "C";
		str += " ";
		for (int i = 0; i < numMissionariesOnRightBank(); i++)
			str += "M";
		return str;
	}
	
	public String toString() {
		return leftBank() + " | " + rightBank();
	}
	
	/**
	 * ***DO NOT CHANGE THIS FUNCTION.***
	 * 
	 * @return String representation of current state of puzzle
	 */
	private String puzzleState() {
		String lb = leftBank();
		String rb = rightBank();
		String str = "\n";
		str += "Left Bank";
		for (int i = 9; i < lb.length(); i++)
			str += " ";
		str += " | ";
		for (int i = lb.length() + 3; i < lb.length() + rb.length() + 3 - 10; i++)
			str += " ";
		str += "Right Bank";
		str += "\n";
		str += lb;
		for (int i = lb.length(); i < 9; i++)
			str += " ";
		str += " | ";
		for (int i = rb.length() - 10; i < 0; i++)
			str += " ";
		str += rb;
		str += "\n";
		str += "\n";
		str += "   Cannibals on left,right banks: ";
		str += String.format("%3d,%3d", numCannibalsOnLeftBank(), numCannibalsOnRightBank());
		str += "\n";
		str += "Missionaries on left,right banks: ";
		str += String.format("%3d,%3d", numMissionariesOnLeftBank(), numMissionariesOnRightBank());
		str += "\n";
		str += "\n";
		str += "Number of moves: " + totalMoves();
		str += "\n";
		return str;
	}
}