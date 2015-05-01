public class MyCLIParser {

	public static void main(String[] args) {

		// if no arguments, print help and return
		if (args.length == 0) {
			help(args);
			return;
		}

		// -help
		else if (args[0].equals("-help"))
			help(args);

		// -add
		else if (args[0].equals("-add"))
			add(args);

		// -sub
		else if (args[0].equals("-sub"))
			sub(args);

		// -mul
		else if (args[0].equals("-mul"))
			mul(args);

		// -div
		else if (args[0].equals("-div"))
			div(args);

		// -stats
		else if (args[0].equals("-stats"))
			stats(args);

		// -table
		else if (args[0].equals("-table"))
			table(args);

	}

	public static boolean isInteger(String s) {
		for (int i = 0; i < s.length(); i++) {
			int chr = s.charAt(i);
			if (chr < 48 || chr > 57)
				return false;
		}
		return true;
	}

	private static void help(String[] args) {
		System.out.println("use -add to add integers together");
		System.out
				.println("use -sub to substract the second number from the first one");
		System.out.println("use -mul to multipuly integers together");
		System.out
				.println("use -div to divide the first number by  the second one");
		System.out
				.println("use -stats to find total, maximum, minimum, average of a series of numbers");
		System.out.println("use -table to make a table of this number");
	}

	private static void add(String[] args) {
		int sum = 0;
		if (args.length == 1) {
			System.out.println("Argument count mismatch");
			return;
		}
		for (int i = 1; i < args.length; i++) {
			if (!isInteger(args[i])) {
				System.out.println("Argument type mismatch");
				return;
			} else {
				sum = sum + Integer.parseInt(args[i]);
			}
		}
		System.out.println(sum);
	}

	private static void sub(String[] args) {
		if (args.length != 3) {
			System.out.println("Argument count mismatch");
			return;
		}
		if (!isInteger(args[1]) || !isInteger(args[2])) {
			System.out.println("Argument type mismatch");
			return;
		} else {
			System.out.println(Integer.parseInt(args[1])
					- Integer.parseInt(args[2]));
		}
	}

	private static void mul(String[] args) {
		int sum = 1;
		if (args.length == 1) {
			System.out.println("Argument count mismatch");
			return;
		}
		for (int i = 1; i < args.length; i++) {
			if (!isInteger(args[i])) {
				System.out.println("Argument type mismatch");
				return;
			} else {
				sum = sum * Integer.parseInt(args[i]);
			}
		}
		System.out.println(sum);
	}

	private static void div(String[] args) {
		if (args.length != 3) {
			System.out.println("Argument count mismatch");
			return;
		}
		if (!isInteger(args[1]) || !isInteger(args[2])) {
			System.out.println("Argument type mismatch");
			return;
		} else if (Integer.parseInt(args[2]) == 0) {
			System.out.println("undefined");
		} else {
			double ans = Double.parseDouble(args[1])
					/ Double.parseDouble(args[2]);
			System.out.printf("%.2f%n", ans);
		}
	}

	private static void stats(String[] args) {
		int sum = 0, lowest = 2147483647, highest = -2147483648;
		int t;
		double average = 0.0;
		if (args.length == 1) {
			System.out.println("Argument count mismatch");
			return;
		}
		for (int i = 1; i < args.length; i++) {
			if (!isInteger(args[i])) {
				System.out.println("Argument type mismatch");
				return;
			} else {
				t = Integer.parseInt(args[i]);
				sum = sum + t;
				if (t > highest)
					highest = t;
				if (t < lowest)
					lowest = t;
			}
		}
		average = (double) sum / (args.length - 1);
		System.out.printf("%d%n%d%n%d%n%.2f%n", sum, highest, lowest, average);
	}

	private static void table(String[] args) {
		int[][] matrix = new int[10][10];
		if (args.length != 2) {
			System.out.println("Argument count mismatch");
			return;
		}
		if (!isInteger(args[1])) {
			System.out.println("Argument type mismatch");
			return;
		} else {
			int t = Integer.parseInt(args[1]);
			for (int i = 0; i < matrix.length; i++)
				for (int j = 0; j < matrix[0].length; j++)
					matrix[i][j] = i * j + t;
		}
		for (int i = 0; i < matrix.length; i++) {
			for (int j = 0; j < matrix[0].length; j++) {
				System.out.format("%6d", matrix[i][j]);
			}
			System.out.println();
		}
	}
}
