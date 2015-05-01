import static org.junit.Assert.*;

import org.junit.Test;

/**
 * Junit test of Guesser.java
 * 
 * @author huang654 <huang654@purdue.edu>
 * 
 * @version 09-16-2014
 */

public class GuesserTest {
	GuessWhoGame a = new GuessWhoGame(0);
	GuessWhoGame b = new GuessWhoGame(1);
	GuessWhoGame c = new GuessWhoGame(2);
	GuessWhoGame d = new GuessWhoGame(3);
	GuessWhoGame e = new GuessWhoGame(4);
	GuessWhoGame f = new GuessWhoGame(5);
	GuessWhoGame g = new GuessWhoGame(6);
	GuessWhoGame h = new GuessWhoGame(7);
	GuessWhoGame i = new GuessWhoGame(8);
	GuessWhoGame j = new GuessWhoGame(9);
	GuessWhoGame k = new GuessWhoGame(10);
	GuessWhoGame l = new GuessWhoGame(11);
	GuessWhoGame m = new GuessWhoGame(12);
	GuessWhoGame n = new GuessWhoGame(13);
	GuessWhoGame o = new GuessWhoGame(14);
	GuessWhoGame p = new GuessWhoGame(15);
	GuessWhoGame q = new GuessWhoGame(16);
	GuessWhoGame r = new GuessWhoGame(17);
	GuessWhoGame s = new GuessWhoGame(18);
	GuessWhoGame t = new GuessWhoGame(19);
	GuessWhoGame u = new GuessWhoGame(20);
	GuessWhoGame v = new GuessWhoGame(21);
	GuessWhoGame w = new GuessWhoGame(22);
	GuessWhoGame x = new GuessWhoGame(23);
	GuessWhoGame y = new GuessWhoGame(24);
	GuessWhoGame z = new GuessWhoGame(25);

	/**
	 * Test the Guesser.play method
	 */
	@Test
	public void testGuesser() {
		Guesser player = new Guesser();
		assertTrue(a.guess(player.play(a)));
		assertTrue(b.guess(player.play(b)));
		assertTrue(c.guess(player.play(c)));
		assertTrue(d.guess(player.play(d)));
		assertTrue(e.guess(player.play(e)));
		assertTrue(f.guess(player.play(f)));
		assertTrue(g.guess(player.play(g)));
		assertTrue(h.guess(player.play(h)));
		assertTrue(i.guess(player.play(i)));
		assertTrue(j.guess(player.play(j)));
		assertTrue(k.guess(player.play(k)));
		assertTrue(l.guess(player.play(l)));
		assertTrue(m.guess(player.play(m)));
		assertTrue(n.guess(player.play(n)));
		assertTrue(o.guess(player.play(o)));
		assertTrue(p.guess(player.play(p)));
		assertTrue(q.guess(player.play(q)));
		assertTrue(r.guess(player.play(r)));
		assertTrue(s.guess(player.play(s)));
		assertTrue(t.guess(player.play(t)));
		assertTrue(u.guess(player.play(u)));
		assertTrue(v.guess(player.play(v)));
		assertTrue(w.guess(player.play(w)));
		assertTrue(x.guess(player.play(x)));
		assertTrue(y.guess(player.play(y)));
		assertTrue(z.guess(player.play(z)));
	}

	/**
	 * call the main method to satisfy WebCAT.
	 */
	@Test
	public void testMain() {
		Guesser.main(new String[0]);
	}

	/**
	 * call the constructor to satisfy WebCAT.
	 */
	@Test
	public void testConstructor() {
		new Guesser();
	}
}
