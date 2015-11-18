//import com.sun.tools.javac.comp.Flow;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.*;

public class project3 {
    private int graphSize;
    private int[] color;
    private int dfn = 0;
    public static Scanner s = new Scanner(System.in);
    ArrayList<Integer> ent;
    ArrayList<Integer> ang;

    public Graph createGraph() {
        System.out.println("Input the size of the graph: Input lines of adjacencies: (V0: V1, V2, V3...)");
        graphSize = s.nextInt();
        s.nextLine();
        if (graphSize == 0) {
            System.out.println("Warning: empty graph");
            return null;
        }
        Graph g = new Graph(graphSize + 1);
        color = new int[graphSize + 1];
        return g;
    }

    private void dfs(int v, Graph g) {
        color[v] = ++dfn;
        for (Integer i : g.adj(v)) {
            if (color[i] == 0) {
                dfs(i, g);
            }
        }
    }

    public boolean validateGraph(Graph g) {
        try {
            if (g == null) return false;
            for (int i = 0; i < graphSize; i++) {
                /* read all the inputs */
                String in = s.nextLine();
                String[] temp = in.split(":");
                int v = Integer.parseInt(temp[0]);

                if (v < 1 || v > graphSize) {
                    System.out.println("Error: Vertex number out of range!");
                    return false;
                }

                for (int k = 0; k <= graphSize; k++) {
                    color[k] = 0;
                }
                color[v] = 1;

                if (temp.length > 1) {
                    in = temp[1];
                    temp = in.split(",");
                    /* add edges into graph, also check adj */
                    for (int j = 0; j < temp.length; j++) {
                        int w = Integer.parseInt(temp[j]);
                        if (w < 1 || w > graphSize) {
                            System.out.println("Error: vertex number out of range.");
                            return false;
                        }
                        if (color[w] == 1) {
                            System.out.println("Error: illegal adjacency.");
                            return false;
                        }
                        g.addEdge(v, w);
                        color[w] = 1;
                    }
                }
            }

            /* check connectivity, mark every point in the order visit them */
            for (int i = 0; i <= graphSize; i++) {
                color[i] = 0;
            }
            dfs(1, g);
            /* if points visited is not equal to the total points, go wrong */
            if (dfn != graphSize) {
                System.out.println("Error: Graph not connected.");
                return false;
            }


        /* give the print */
            System.out.println("Number of vertices: " + graphSize);
            if (graphSize < 10) {
                /* sort the adj list if need print */
                g.sortEdgeList();

                for (int i = 1; i <= graphSize; i++) {
                    System.out.print(i + ":");
                    Iterator<Integer> adj = g.adj(i).iterator();
                    int e;
                    if (adj.hasNext()) {
                        e = adj.next();
                        System.out.print(e);
                    }
                    while (adj.hasNext()) {
                        e = adj.next();
                        System.out.print("," + e);
                    }
                    System.out.println();
                }
            } else {
                System.out.println("Graph passes.");
            }
            return true;
        } catch (Exception e) {
            System.out.println("Invaild Input.");
            return false;
        }
    }


    public boolean checkBipartiteBfs(Graph g) {
        /* initialize
        *  color[n] = color of vertex n;
        *  distinguish x and y by odd and even
        * */
        color = new int[graphSize + 1];
        for (int i = 0; i <= graphSize; i++) {
            color[i] = 0;
        }
        /* a list to store vertex plan to visit*/
        Queue<Integer> list = new Queue<Integer>();
        /* add first vertex to queue*/
        color[1] = 1;
        list.enqueue(1);

        while (!list.isEmpty()) {
            int v = list.dequeue();
            for (Integer w : g.adj(v)) {
                if (color[w] == 0) {
                    color[w] = color[v] + 1;
                    list.enqueue(w);
                } else {
                    int a = color[v] % 2;
                    int b = color[w] % 2;
                    if (a == b) {
                        System.out.println("Error: Graph not bipartite!");
                        return false;
                    }
                }
            }
        }
        /* initialize X and Y list */
        ent = new ArrayList<>();
        ang = new ArrayList<>();
        for (int i = 1; i <= graphSize; i++) {
            if (color[i] % 2 == 1) {
                ent.add(i);
            } else {
                ang.add(i);
            }
        }

        if (graphSize < 10) {
            /* sort and print if graph size less than 10*/
            Iterator<Integer> temp = ent.iterator();
            int e;

            System.out.print("Partition X:");
            if (temp.hasNext()) {
                e = temp.next();
                System.out.print(e);
            }
            while (temp.hasNext()) {
                e = temp.next();
                System.out.print(", " + e);
            }
            System.out.println();

            temp = ang.iterator();
            System.out.print("Partition Y:");
            if (temp.hasNext()) {
                e = temp.next();
                System.out.print(e);
            }
            while (temp.hasNext()) {
                e = temp.next();
                System.out.print(", " + e);
            }
            System.out.println();

        } else {
            System.out.println("Graph is bipartite.");
        }
        return true;
    }

    public FlowNetwork createFlowNetwork(Graph g) {
        FlowNetwork flowNet = new FlowNetwork(graphSize + 2);
        Iterator<Integer> temp = ent.iterator();
        int w;
        while (temp.hasNext()) {
            w = temp.next();
            flowNet.addEdge(new FlowEdge(0, w, 1));
            Iterator<Integer> adj = g.adj(w).iterator();
            int e;
            while (adj.hasNext()) {
                e = adj.next();
                flowNet.addEdge(new FlowEdge(w, e, 1));
            }
        }

        temp = ang.iterator();
        while (temp.hasNext()) {
            w = temp.next();
            flowNet.addEdge(new FlowEdge(w, graphSize + 1, 1));
        }

        return flowNet;
    }


    public static void main(String args[]) {
        /* methods to reduce the size of main method*/
        project3 p3 = new project3();
        Graph g = p3.createGraph();
        boolean flag = p3.validateGraph(g);
        if (!flag) return;
        flag = p3.checkBipartiteBfs(g);
        if (!flag) return;

        /* crate the flow net by adding all the edges from graph to it*/
        FlowNetwork flowNet = p3.createFlowNetwork(g);
        if (flowNet.V() - 2 < 20) {
            flowNet.sortEdgeList();
            for (int i = 0; i < flowNet.V() - 1; i++) {
                System.out.print(i + ":");
                FlowEdge e;
                Iterator<FlowEdge> temp = flowNet.adj(i).iterator();
                boolean m = true;
                while (temp.hasNext()) {
                    e = temp.next();
                    if (e.to() != i) {
                        if (m) {
                            System.out.print(e.to());
                            m = false;
                        } else {
                            System.out.print("," + e.to());
                        }
                    }
                }
                System.out.println();
            }
            System.out.println(flowNet.V() - 1 + ": ");
        } else {
            System.out.println("Flow network built.");
        }

        /* Apply Ford Fulkerson algorithm to calculate max flow
        * set the source as 0, set the sink as n + 1*/
        FordFulkerson ff = new FordFulkerson(flowNet, 0, flowNet.V() - 1);
        if (ff.value() > 0) {
            System.out.println("Matching found");
        }
        /* Print matches by check flow in every point*/
        boolean m = false;
        for (int i = 1; i <= flowNet.V() - 1; i++) {
            for (FlowEdge e : flowNet.adj(i)) {
                if (e.from() == i && e.to() != flowNet.V() - 1 && e.flow() > 0) {
                    if (m) {
                        System.out.print("," + "(" + e.from() + "," + e.to() + ")");
                    } else {
                        System.out.print("(" + e.from() + "," + e.to() + ")");
                        m = true;
                    }
                }
            }
        }
        System.out.println(" Number of edges " + (int) ff.value());
    }
}