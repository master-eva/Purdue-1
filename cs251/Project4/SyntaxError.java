public class SyntaxError extends Exception {
    public SyntaxError(int line) {
        super("Error input syntax line " + line);
    }
}
