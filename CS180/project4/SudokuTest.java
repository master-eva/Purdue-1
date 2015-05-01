import junit.framework.TestCase;

/**
 * A JUnit test case class. Every method starting with the word "test" will be
 * called when running the test with JUnit.
 */
public class SudokuTest extends TestCase {
	
	/**
	 * A test method. (Replace "X" with a name describing the test. You may
	 * write as many "testSomething" methods in this class as you wish, and each
	 * one will be called when running JUnit over this class.)
	 */
	public void testIsSolved_00() {
		int[][] b = { { 0, 0, 0, 4, 8, 6, 0, 0, 0 }, { 0, 0, 2, 0, 7, 9, 0, 1, 8 }, { 5, 6, 0, 2, 1, 0, 0, 7, 0 },
		        { 6, 4, 5, 1, 0, 8, 3, 0, 0 }, { 1, 0, 0, 0, 5, 0, 0, 4, 0 }, { 8, 9, 7, 0, 6, 0, 1, 0, 0 },
		        { 2, 0, 9, 0, 3, 0, 0, 0, 0 }, { 3, 0, 4, 0, 9, 2, 7, 0, 5 }, { 0, 0, 0, 0, 0, 0, 2, 3, 0 } };
		Sudoku s = new Sudoku(b);
		assertFalse(s.isSolved());
	}
	
	public void testIsSolved_01() {
		int[][] b = { { 0, 0, 0, 4, 8, 6, 0, 0, 0 }, { 0, 0, 2, 0, 7, 9, 0, 1, 8 }, { 5, 6, 0, 2, 1, 0, 0, 7, 0 },
		        { 6, 4, 5, 1, 0, 8, 3, 0, 0 }, { 1, 0, 0, 0, 5, 0, 0, 4, 0 }, { 8, 9, 7, 0, 6, 0, 1, 0, 0 },
		        { 2, 0, 9, 0, 3, 0, 0, 0, 0 }, { 3, 0, 4, 0, 9, 2, 7, 0, 5 }, { 0, 0, 0, 0, 0, 0, 2, 3, 0 } };
		Sudoku s = new Sudoku(b);
		s.solve();
		assertTrue(s.isSolved());
	}
	
	public void testHiddenSingles() {
		int[][] b = { { 0, 0, 0, 4, 8, 6, 0, 0, 0 }, { 0, 0, 2, 0, 7, 9, 0, 1, 8 }, { 5, 6, 0, 2, 1, 0, 0, 7, 0 },
		        { 6, 4, 5, 1, 0, 8, 3, 0, 0 }, { 1, 0, 0, 0, 5, 0, 0, 4, 0 }, { 8, 9, 7, 0, 6, 0, 1, 0, 0 },
		        { 2, 0, 9, 0, 3, 0, 0, 0, 0 }, { 3, 0, 4, 0, 9, 2, 7, 0, 5 }, { 0, 0, 0, 0, 0, 0, 2, 3, 0 } };
		Sudoku s = new Sudoku(b);
		assertTrue(s.hiddenSingles());
	}
	
	public void testNakedSingles() {
		int[][] b = { { 0, 0, 0, 4, 8, 6, 0, 0, 0 }, { 0, 0, 2, 0, 7, 9, 0, 1, 8 }, { 5, 6, 0, 2, 1, 0, 0, 7, 0 },
		        { 6, 4, 5, 1, 0, 8, 3, 0, 0 }, { 1, 0, 0, 0, 5, 0, 0, 4, 0 }, { 8, 9, 7, 0, 6, 0, 1, 0, 0 },
		        { 2, 0, 9, 0, 3, 0, 0, 0, 0 }, { 3, 0, 4, 0, 9, 2, 7, 0, 5 }, { 0, 0, 0, 0, 0, 0, 2, 3, 0 } };
		Sudoku s = new Sudoku(b);
		assertTrue(s.nakedSingles());
	}
	
	public void testSolveBoard() {
		int[][] ans = { { 2, 1, 5, 7, 4, 6, 9, 3, 8 }, { 3, 8, 7, 5, 9, 1, 2, 6, 4 }, { 4, 6, 9, 8, 3, 2, 5, 1, 7 },
		        { 5, 7, 3, 6, 2, 4, 1, 8, 9 }, { 6, 9, 4, 1, 7, 8, 3, 2, 5 }, { 8, 2, 1, 9, 5, 3, 4, 7, 6 },
		        { 7, 4, 6, 3, 1, 5, 8, 9, 2 }, { 9, 3, 2, 4, 8, 7, 6, 5, 1 }, { 1, 5, 8, 2, 6, 9, 7, 4, 3 } };
		int[][] b = { { 0, 0, 5, 7, 4, 0, 0, 0, 8 }, { 0, 8, 0, 0, 9, 0, 2, 6, 4 }, { 4, 6, 9, 8, 3, 2, 0, 1, 0 },
		        { 5, 0, 3, 0, 0, 4, 1, 0, 9 }, { 0, 0, 0, 0, 7, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 3, 0, 0, 6 },
		        { 7, 0, 0, 3, 1, 5, 0, 9, 2 }, { 0, 0, 2, 0, 0, 0, 0, 0, 1 }, { 1, 5, 8, 2, 6, 0, 7, 0, 0 } };
		Sudoku s = new Sudoku(b);
		s.solve();
		b = s.board();
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				assertEquals(ans[i][j], b[i][j]);
	}
	
	public void testConstructor() {
		Sudoku s = new Sudoku();
		int[][] b = s.board();
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				assertEquals(0, b[i][j]);
	}
	
	public void testCandidates() {
		int[][] b = { { 0, 0, 5, 7, 4, 0, 0, 0, 8 }, { 0, 8, 0, 0, 9, 0, 2, 6, 4 }, { 4, 6, 9, 8, 3, 2, 0, 1, 0 },
		        { 5, 0, 3, 0, 0, 4, 1, 0, 9 }, { 0, 0, 0, 0, 7, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 3, 0, 0, 6 },
		        { 7, 0, 0, 3, 1, 5, 0, 9, 2 }, { 0, 0, 2, 0, 0, 0, 0, 0, 1 }, { 1, 5, 8, 2, 6, 0, 7, 0, 0 } };
		Sudoku s = new Sudoku(b);
		boolean[] result = s.candidates(0, 0);
		for (int i = 0; i < 9; i++)
			assertEquals(s.candidatesNew(0, 0)[i], result[i + 1]);
	}
}