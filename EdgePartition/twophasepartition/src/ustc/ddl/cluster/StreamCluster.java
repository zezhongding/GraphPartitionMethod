package ustc.ddl.cluster;

import ustc.ddl.graph.Edge;
import ustc.ddl.graph.Graph;
import ustc.ddl.properties.GlobalConfig;

import java.util.Arrays;
import java.util.HashMap;

public class StreamCluster {
    private int[] degree;
    private HashMap<Integer, Integer> vol;
    private int[] v2c;
    private int max_vol;
    private int next_id = 0;
    private Graph graph;
    StreamCluster() {

    }
    public StreamCluster(Graph graph) {
        this.graph = graph;
        degree = new int[graph.getVCount()];
        v2c = new int[graph.getVCount()];
        Arrays.fill(v2c, -1);
        max_vol = 2 * graph.getECount() / GlobalConfig.partitionNum;
        vol = new HashMap<>();
        calculateDegree();
    }

    private void calculateDegree() {
        graph.readGraphFromFile();
        Edge edge;
        //First Time
        while((edge = graph.readStep()) != null) {
            int src = edge.getSrcVId();
            int dest = edge.getDestVId();
            degree[src] ++;
            degree[dest] ++;
        }
    }
    public void startStreamingClustering() {
        graph.readGraphFromFile();
        Edge edge;
        //First Time
        while((edge = graph.readStep()) != null) {
            int src = edge.getSrcVId();
            int dest = edge.getDestVId();
            for(int v : new int[]{src, dest}) {
                if(v2c[v] == -1) {
                    v2c[v] = next_id;
                    if (!vol.containsKey(next_id)) {
                        vol.put(next_id, 0);
                    }
                    vol.put(next_id, degree[v]);
                    next_id++;
                }
            }
            for(int v : new int[]{src, dest}) {
                if(vol.get(v2c[v]) <= max_vol) {
                    int vs = (vol.get(v2c[src]) - degree[src] <= vol.get(v2c[dest]) - degree[dest] ? src : dest);
                    int vl = (vs == src ? dest : src);
                    if(vol.get(v2c[vl]) + degree[vs] <= max_vol) {
                        vol.put(v2c[vl], vol.get(v2c[vl]) + degree[vs]);
                        vol.put(v2c[vs], vol.get(v2c[vs]) - degree[vs]);
                        v2c[vs] = v2c[vl];
                    }
                }
            }
        }
    }

    public int[] getDegree() {
        return degree;
    }

    public HashMap<Integer, Integer> getVol() {
        return vol;
    }

    public int[] getV2c() {
        return v2c;
    }
}
