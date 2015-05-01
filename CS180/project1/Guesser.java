/**
 * CS 180 - 814
 * 
 * @author huang654 <huang654@purdue.edu>
 * 
 * @version 09-15-2014
 */

public class Guesser {

	/**
	 * Ask questions about one person and guess who he is. The Guess Who Game!!
	 */
	public static String play(GuessWhoGame g) {
		if (g.isWearingGlasses()) {
			if (g.isWearingHat()) {
				if (g.isSmiling()) {
					if (g.eyeIsColor(Color.BROWN)) {
						if (g.hairIsColor(Color.BLACK)) {
							return "Xavier";
						} else {
							return "Sarah";
						}
					} else {
						if (g.eyeIsColor(Color.BLUE)) {
							return "Gertrude";
						} else {
							return "Emily";
						}
					}
				} else {
					return "Bob";
				}
			} else {
				if (g.isSmiling()) {
					if (g.shirtIsColor(Color.GREEN)) {
						if (g.eyeIsColor(Color.BLUE)) {
							if (g.hairIsColor(Color.BROWN)) {
								return "Alice";
							} else {
								return "Yasmine";
							}
						} else {
							if (g.eyeIsColor(Color.GREEN)) {
								return "Frank";
							} else {
								return "Isabelle";
							}
						}
					} else {
						if (g.shirtIsColor(Color.BLUE)) {
							return "Victor";
						} else {
							if (g.hairIsColor(Color.BROWN)) {
								return "Mallie";
							} else {
								return "Henry";
							}
						}

					}
				} else {
					if (g.shirtIsColor(Color.RED)) {
						return "Wendy";
					} else {
						if (g.hairIsColor(Color.RED)) {
							return "Larry";
						} else {
							return "Carol";
						}
					}
				}
			}
		} else {
			if (g.isWearingHat()) {
				if (g.shirtIsColor(Color.RED)) {
					return "Robert";
				} else {
					return "Dave";
				}
			} else {
				if (g.hairIsColor(Color.BROWN)) {
					if (g.shirtIsColor(Color.RED)) {
						if (g.eyeIsColor(Color.BROWN)) {
							return "Quinn";
						} else {
							if (g.eyeIsColor(Color.BLUE)) {
								return "Nick";
							} else {
								return "Philip";
							}
						}
					} else {
						if (g.eyeIsColor(Color.BLUE)) {
							return "Tucker";
						} else {
							return "Zander";
						}
					}
				} else {
					if (g.hairIsColor(Color.BLACK)) {
						if (g.shirtIsColor(Color.GREEN)) {
							if (g.eyeIsColor(Color.BROWN)) {
								return "Ursula";
							} else {
								return "Karen";
							}
						} else {
							return "Olivia";
						}
					} else {
						return "Jack";
					}
				}
			}
		}

	}

	/**
	 * 1. instantiate the GuessWhoGame
	 * 
	 * 2. play the game and get the name of the character
	 * 
	 * 3. Guess the name
	 * 
	 * 4. print if it is correct
	 * 
	 * 5.print score
	 */
	public static void main(String[] args) {
		Guesser a = new Guesser();
		GuessWhoGame g = new GuessWhoGame();
		String name = a.play(g);
		System.out.println(g.guess(name));
		System.out.println(g.score());
	}
}
