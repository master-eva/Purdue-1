import java.util.ArrayList;

public class Project2Part1 {
	public static void main(String args[]) {
		Project2 p2 = new Project2();
        ArrayList<Token> tokens = p2.read_input();
        Parser p;

        while (tokens.contains(new Token(20, 0))) {
            //if there's a empty expression, remove it
            while (tokens.get(0).equals(new Token(28, 0))) tokens.remove(0);
            //if meet "?", quit
            if (tokens.get(0) .equals(new Token(20, 0))) break;
            //create a parser for modified tokens
            p = new Parser(tokens);

            //build expression, then print
            Tree t = p.build_expression_tree(tokens);
            if (t == null) continue;
            t.print();
            System.out.println();

            //evaluate the tree
            //catch the devided by zero cases
            try {
                t = p2.evaluate(t);
                t.get_info().print();
                System.out.println();
            } catch (Exception e) {
                System.out.println(e.getMessage());
                break;
            }

            //if reach the "?" quit
            if (p.currentTk.equals(new Token(20, 0))) break;

            //delete the tokens has been handled
            int s = tokens.indexOf(p.currentTk);
            for (int i = 0; i <= s; i++) {
                tokens.remove(0);
            }
        }
	}
}