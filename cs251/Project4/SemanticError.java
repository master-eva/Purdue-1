public class SemanticError extends Exception {
    public SemanticError(int date) {
        super("Error semantics date " + date);
    }
}
