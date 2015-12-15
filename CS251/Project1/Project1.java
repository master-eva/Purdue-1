import java.awt.Point;
import java.util.ArrayList;
import java.util.Scanner;

public class Project1 {
    private int m;
    private int n;
    private WeightedQuickUnionUF qu;
    private int[][] grid;
    private ArrayList<Point> connections;
    private ArrayList<Integer> size;
    static Scanner input = new Scanner(System.in);

    /**
     * initializes UnionFind structure, grid and connection list
     *
     * @param m number of rows
     * @param n number of columns
     */
    public Project1(int m, int n) {
        this.m = m;
        this.n = n;
        this.qu = new WeightedQuickUnionUF(m * n);
        this.grid = new int[m][n];
        this.connections = new ArrayList<>();
        this.size = new ArrayList<>();
    }

    public static void main(String args[]) {
        int m, n;
        //System.out.print("Enter size of grid(m n): ");
        m = input.nextInt();
        n = input.nextInt();

        Project1 project1 = new Project1(m, n);
        project1.read_input();
        project1.process_connections();
        ArrayList<Point> sets = project1.retrieve_connected_sets();
        project1.output_boundaries_size(sets);
    }

    /**
     * Reads input from user (pair of connections presented as points), store the input in a list
     */
    public void read_input() {
        //System.out.print("Enter number of pairs of connections: ");
        int t = input.nextInt();
        for (int i = 0; i < t; i++) {
            int m1 = input.nextInt();
            int n1 = input.nextInt();
            int m2 = input.nextInt();
            int n2 = input.nextInt();
            connections.add(new Point(m1, n1));
            connections.add(new Point(m2, n2));
        }
    }

    /**
     * converts point into an integer
     *
     * @param p a point in matrix to be converted
     * @return a integer in map
     */
    public int map(Point p) {
        return (int) p.getX() * n + (int) p.getY();
    }

    /***
     * converts integer into a point
     *
     * @param i a integer in map to be converted
     * @return a point in matrix
     */
    public Point unmap(int i) {
        return new Point(i / n, i % n);
    }

    /***
     * scans connections and populates UnionFind structure
     */
    public void process_connections() {
        for (int i = 0; i < connections.size(); i+=2) {
            Point p1 = connections.get(i);
            Point p2 = connections.get(i+1);

            //check if the points can be connected,
            //if they can, connect them.
            if (is_adjacent(p1,p2))
                qu.union(map(p1), map(p2));
        }
    }

    /**
     * retrieve the connected sets from the UnionFind structure
     *
     * @return connected sets
     */
    public ArrayList<Point> retrieve_connected_sets() {
        ArrayList<Point> sets = new ArrayList<>();
        for (int i = 0; i < m * n; i++) {
            Point q = unmap(i);
            int root = qu.find(i);
            Point p = unmap(root);
            if (!sets.contains(p)) {
                sets.add(p);
                size.add(qu.getSize(map(p)));
                grid[(int) q.getX()][(int) q.getY()] = root;
            } else {
                grid[(int) q.getX()][(int) q.getY()] = root;
            }
        }
        return sets;
    }

    /**
     * Tests whether two Cells are connected in the grid
     *
     * @param p1 point 1
     * @param p2 point 2
     * @return true for the points can be connected
     */
    public boolean is_adjacent(Point p1, Point p2) {
        int m1 = (int) p1.getX();
        int n1 = (int) p1.getY();
        int m2 = (int) p2.getX();
        int n2 = (int) p2.getY();

        //return if the points can be connected
        //if they are connected, then they cannot be connected again
        //if they are not adjacent, then they cannot be connected
        return !qu.connected(map(p1), map(p2)) && (((m1 == m2) && (n1 - n2) == 1) || ((m1 == m2) && (n1 - n2) == -1) ||
                ((m1 - m2 == 1) && (n1 == n2)) || ((m1 - m2 == - 1) && (n1 == n2)));
    }

    /**
     * outputs the boundaries and size of each connected set
     *
     * @param sets contains set of roots
     */
    public void output_boundaries_size(ArrayList<Point> sets) {
        //System.out.println("number of sets: " + qu.count());

        //create property sets
        int length = sets.size();
        int[] minM = new int[length];
        int[] maxM = new int[length];
        int[] minN = new int[length];
        int[] maxN = new int[length];

        //initialize property sets
        for (int i = 0; i < length; i++) {
            minM[i] = m;
            minN[i] = n;
            maxM[i] = 0;
            maxN[i] = 0;
        }

        //get properties: size and bounds
        for (int i = 0; i < length; i++) {
            Point p = sets.get(i);
            for (int j = 0; j < m; j++)
                for (int k = 0; k < n; k++) {
                    if (grid[j][k] == grid[(int) p.getX()][(int) p.getY()]) {
                        if (j > maxM[i]) maxM[i] = j;
                        if (j < minM[i]) minM[i] = j;
                        if (k > maxN[i]) maxN[i] = k;
                        if (k < minN[i]) minN[i] = k;
                    }
                }
        }

        //print size
        for (int i = 0; i < length; i++) {
            Point p = sets.get(i);
            System.out.println("Set (" + (int) p.getX() + "," + (int) p.getY() + ") with size " + size.get(i));
        }

        //print bound
        for (int i = 0; i < length; i++) {
            Point p = sets.get(i);
            System.out.println("Boundaries for (" + (int) p.getX() + "," + (int) p.getY() + ") are "
                    + minN[i] + "<=x<=" + maxN[i] + " and " + minM[i] + "<=y<=" + maxM[i]);
        }
    }
}