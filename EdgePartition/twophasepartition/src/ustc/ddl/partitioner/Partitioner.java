package ustc.ddl.partitioner;

import ustc.ddl.graph.Edge;
import ustc.ddl.graph.Graph;
import ustc.ddl.properties.GlobalConfig;

import java.util.*;


public class Partitioner {
    private int[] d;
    private HashMap<Integer, Integer> vol;
    private int[] v2c;
    private int[] c2p;
    private int[] vol_p;

    private int[] vol_p_edge;
    private int[][] v2p;
    Graph graph;
    public Partitioner() {

    }
    public Partitioner(int[] d, HashMap<Integer, Integer> vol, int[] v2c, Graph graph) {
        this.d = d;
        this.vol = vol;
        this.v2c = v2c;
        this.graph = graph;
        this.vol_p_edge = new int[GlobalConfig.partitionNum];
        c2p = new int[vol.keySet().size()];
        vol_p = new int[GlobalConfig.partitionNum];
        v2p = new int[graph.getVCount()][GlobalConfig.partitionNum];
    }

    public void streamingPartitioning() {
        mapClustersToPartitions();
        prepartitionEdges();
        partitionRemainingEdges();
    }

    private void partitionRemainingEdges() {
        graph.readGraphFromFile();
        Edge edge;
        //First Time
        while((edge = graph.readStep()) != null) {
            int first = edge.getSrcVId();
            int sec  = edge.getDestVId();
            int c_1 = v2c[first];
            int c_2 = v2c[sec];
            int target_p = 0;
            if(c_1 == c_2 || c2p[c_1] == c2p[c_2]) {
                continue;
            }
            double bestScore = 0;
            for(int pi : new int[]{c2p[v2c[first]], c2p[v2c[sec]]}) {
                double score = getScore(first, sec, pi);
                if(score > bestScore) {
                    bestScore = score;
                    target_p = pi;
                }
            }
            if(vol_p_edge[target_p] > GlobalConfig.alpha * GlobalConfig.eCount / GlobalConfig.partitionNum) {
                int p = 31;
                int hash = Math.max(d[first], d[sec]) % GlobalConfig.partitionNum;
                target_p = ((hash % p) + GlobalConfig.partitionNum) % GlobalConfig.partitionNum;
                if(vol_p_edge[target_p] > GlobalConfig.alpha * GlobalConfig.eCount / GlobalConfig.partitionNum) {
                    int minn = vol_p_edge[0];
                    int minn_id = 0;
                    for (int i = 1; i < vol_p_edge.length; i++) {
                        if(vol_p_edge[i] < minn) {
                            minn = vol_p_edge[i];
                            minn_id = i;
                        }
                    }
                    target_p = minn_id;
                }
            }
            v2p[first][target_p] = 1;
            v2p[sec][target_p] = 1;
            vol_p_edge[target_p] += 1;
        }
    }

    private double getScore(int u, int v, int pi) {
        double gu = isReplicatedOnP(u, pi) ? 1.0 + (1.0 - (double) d[u] / (d[u] + d[v])) : 0.0;
        double gv = isReplicatedOnP(v, pi) ? 1.0 + (1.0 - (double) d[v] / (d[u] + d[v])) : 0.0;
        double scu = isAssignedToP(u, pi) ? (double) vol.get(v2c[u]) / vol.get(v2c[u]) + vol.get(v2c[v]) : 0.0;
        double scv = isAssignedToP(v, pi) ? (double) vol.get(v2c[v]) / vol.get(v2c[u]) + vol.get(v2c[v]) : 0.0;
        return gu + gv + scu + scv;
    }

    private boolean isAssignedToP(int u, int pi) {
        return c2p[v2c[u]] == pi;
    }

    private boolean isReplicatedOnP(int v, int pi) {
        return v2p[v][pi] == 1;
    }


    private void prepartitionEdges() {
        graph.readGraphFromFile();
        Edge edge;
        //First Time
        while((edge = graph.readStep()) != null) {
            int first = edge.getSrcVId();
            int sec  = edge.getDestVId();
            int c_1 = v2c[first];
            int c_2 = v2c[sec];
            int target_p = 0;
            if(c_1 == c_2 || c2p[c_1] == c2p[c_2]) {
                target_p = c2p[c_1];
                if(vol_p_edge[target_p] > GlobalConfig.alpha * GlobalConfig.eCount / GlobalConfig.partitionNum) {
                    double bestScore = 0;
                    for(int pi : new int[]{c2p[v2c[first]], c2p[v2c[sec]]}) {
                        double score = getScore(first, sec, pi);
                        if(score > bestScore) {
                            bestScore = score;
                            target_p = pi;
                        }
                    }
                }
                v2p[first][target_p] = 1;
                v2p[sec][target_p] = 1;
                vol_p_edge[target_p] += 1;
            }
        }
        
    }

    private void mapClustersToPartitions() {
        List<Map.Entry<Integer,Integer>> list = new ArrayList<>(vol.entrySet());
        list.sort((o1, o2) -> o2.getValue() - o1.getValue());
        for(Map.Entry<Integer, Integer> c : list) {
            int target_p = getMinP(vol_p);
            c2p[c.getKey()] = target_p;
            vol_p[target_p] += c.getValue();
        }
    }

    private int getMinP(int[] vol_p) {
        int minn = 0;
        int minn_id = 0;
        for (int i = 0; i < vol_p.length; i++) {
            if(vol_p[i] == 0) {
                return i;
            }
            if(vol_p[i] < minn) {
                minn_id = i;
                minn = vol_p[i];
            }
        }
        return minn_id;
    }

    //TODO
    public double getReplicateFactor() {
        int replicateTotal = 0;
        for (int[] ints : v2p) {
            for (int anInt : ints) {
                replicateTotal += anInt;
            }
        }
        return (double) replicateTotal / GlobalConfig.vCount;
    }

    //TODO
    public double getLoadBalance() {
        double maxLoad = 0.0;
        for (int i = 0; i < GlobalConfig.getPartitionNum(); i++) {
            if (maxLoad < vol_p_edge[i]) {
                maxLoad = vol_p_edge[i];
            }
        }
        return (double) GlobalConfig.getPartitionNum() / GlobalConfig.getECount() * maxLoad;
    }
}
