import java.util.ArrayList;
import java.util.Scanner;

public class Project0 {
    static ArrayList<Token> tokens = new ArrayList<Token>();
    static String input;

    /* Split the input into the separate tokens */
    public ArrayList<Token> get_tokens(String input) {
        int i = 0;

        while (i < input.length()) {
            char c = input.charAt(i);
            Token t;

            // A switch statement to deal with tokens.
            // The default case process ' ', '.' and numbers(integers and floats)
            // Each other case serves for a character
            switch (c) {
                case '?':
                    t = new Token(20, "0");
                    break;
                case '(':
                    t = new Token(21, "0");
                    break;
                case ')':
                    t = new Token(22, "0");
                    break;
                case '+':
                    t = new Token(23, "0");
                    break;
                case '-':
                    t = new Token(24, "0");
                    break;
                case '*':
                    t = new Token(25, "0");
                    break;
                case '/':
                    t = new Token(26, "0");
                    break;
                case ';':
                    t = new Token(28, "0");
                    break;
                case '=':
                    t = new Token(29, "0");
                    break;
                default:
                    boolean flag = false;
                    String tempValue = "";
                    while (i < input.length()) {
                        c = input.charAt(i);
                        if (c == '.' && !flag) {
                            flag = true;
                            tempValue = tempValue + c;
                            i++;
                        } else if (c <= '9' && c >= '0') {
                            tempValue = tempValue + c;
                            i++;
                        } else if (c == ' ') {
                            break;
                        } else {
                            i--;
                            break;
                        }
                    }

                    if (tempValue.equals("")) {
                        t = null;
                    } else if (tempValue.equals(".")) {
                        t = new Token(27, "0");
                    } else if (!flag) {
                        t = new Token(11, Integer.parseInt(tempValue));
                    } else if (tempValue.charAt(tempValue.length() - 1) == '.') {
                        tempValue = tempValue.substring(0, tempValue.length() - 1);
                        t = new Token(11, Integer.parseInt(tempValue));
                        tokens.add(t);
                        t = new Token(27, "0");
                    } else {
                        t = new Token(12, Float.parseFloat(tempValue));
                    }
                    break;
            }

            if (t != null) {
                tokens.add(t);
            }

            i++;
        }
        return tokens;
    }

    /*Read input until a '?' is found */
    public String read_input(Scanner in) {
        input = "";
        while (in.hasNextLine()) {
            String temp = in.nextLine();

            //ignore characters that couldn't recognize by program, replace by space to separate
            temp = temp.replaceAll("[a-zA-Z ]", " ");

            //process line with '?' and without '?' differently
            if (temp.indexOf('?') == -1) {
                input = input + temp;
            } else {
                String[] a = temp.split("\\?");
                input = input + a[0] + "?";
                break;
            }
        }

        //System.out.print(input);
        return input;
    }

    /* Print all the tokens before and including the '?' token
     * Print tokens from list in the following way, "(token,tokenValue)"
     * */
    public void print_tokens(ArrayList<Token> tokens) {
        for (int i = 0; i < tokens.size(); i++) {
            Token t = tokens.get(i);
            char type = t.getType();
            String value = "";

            // get type of token, and then
            // use different methods from Token class to deal with them
            switch (type) {
                case 's':
                    value = t.getValue_s();
                    break;
                case 'i':
                    int tempInt = t.getValue_i();
                    value = Integer.toString(tempInt);
                    break;
                case 'f':
                    float tempFloat = t.getValue_f();
                    value = Float.toString(tempFloat);
                    break;
            }

            System.out.printf("(%d,%s)", t.getToken(), value);
        }
    }

    public static void main(String args[]) {
        Scanner in = new Scanner(System.in);
        Project0 p0 = new Project0();
        input = p0.read_input(in);
        tokens = p0.get_tokens(input);
        p0.print_tokens(tokens);
    }
}
