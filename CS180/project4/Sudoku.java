import java.util.Arrays;

public class Sudoku {
	private int[][] board = new int[9][9];
	
	public Sudoku() {
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				this.board[i][j] = 0;
	}
	
	public Sudoku(int[][] board) {
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				this.board[i][j] = board[i][j];
	}
	
	public int[][] board() {
		int[][] ans = new int[9][9];
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				ans[i][j] = this.board[i][j];
		return ans;
		
	}
	
	public boolean[] candidates(int row, int column) {
		boolean[] result = new boolean[10];
		result[0] = false;
		for (int i = 0; i < 9; i++) {
			result[i + 1] = this.candidatesNew(row, column)[i];
		}
		return result;
	}
	
	public boolean[] candidatesNew(int row, int column) {
		boolean[] result = new boolean[9];
		for (int i = 0; i < 9; i++)
			result[i] = true;
		
		for (int i = 0; i < 9; i++) {
			if (board[row][i] != 0)
				result[board[row][i] - 1] = false;
		}
		
		for (int i = 0; i < 9; i++) {
			if (board[i][column] != 0)
				result[board[i][column] - 1] = false;
		}
		
		for (int i = (row / 3) * 3; i <= (row / 3) * 3 + 2; i++)
			for (int j = (column / 3) * 3; j <= (column / 3) * 3 + 2; j++) {
				if (board[i][j] != 0)
					result[board[i][j] - 1] = false;
			}
		
		return result;
	}
	
	public boolean isSolved() {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				if (this.board[i][j] == 0)
					return false;
			}
		}
		return true;
	}
	
	public void solve() {
		while (!isSolved() && (nakedSingles() || hiddenSingles()))
			;
	}
	
	public boolean nakedSingles() {
		boolean flag = false;
		for (int r = 0; r < 9; r++)
			for (int c = 0; c < 9; c++) {
				if (board[r][c] == 0) {
					int count = 0;
					int t = 0;
					boolean[] test = this.candidatesNew(r, c);
					for (int i = 0; i < 9; i++)
						if (test[i]) {
							count++;
							t = i + 1;
						}
					if (count == 1) {
						board[r][c] = t;
						flag = true;
					}
				}
			}
		return flag;
	}
	
	public boolean hiddenSingles() {
		boolean flag = false;
		int count = 0;
		for (int r = 0; r < 9; r++)
			for (int c = 0; c < 9; c++) {
				for (int t = 0; t < 9; t++) {
					if (board[r][c] == 0 && this.candidatesNew(r, c)[t]) {
						count = 0;
						for (int i = 0; i < 9; i++)
							if (board[r][i] == 0 && this.candidatesNew(r, i)[t])
								count++;
						if (count == 1) {
							board[r][c] = t + 1;
							flag = true;
							continue;
						}
						
						count = 0;
						for (int i = 0; i < 9; i++)
							if (board[i][c] == 0 && this.candidatesNew(i, c)[t])
								count++;
						if (count == 1) {
							board[r][c] = t + 1;
							flag = true;
							continue;
						}
						
						count = 0;
						for (int i = (r / 3) * 3; i <= (r / 3) * 3 + 2; i++)
							for (int j = (c / 3) * 3; j <= (c / 3) * 3 + 2; j++)
								if (board[i][j] == 0 && this.candidatesNew(i, j)[t])
									count++;
						if (count == 1) {
							board[r][c] = t + 1;
							flag = true;
						}
					}
				}
			}
		
		return flag;
	}
	
}