package ustc.ddl.partitioner;

import ustc.ddl.graph.*;
import ustc.ddl.properties.GlobalConfig;


public class Partitioner {
    private Graph graph;
    private int[] d; //delta degree
    private int[][] v2p; //vertex to partition
    private int[] volP; //volume of partition
    private int maxsize = 0;
    private int minsize = Integer.MAX_VALUE;
    public Partitioner() {

    }
    public Partitioner(Graph graph) {
        this.graph = graph;
        d = new int[graph.getVCount()];
        v2p = new int[graph.getVCount()][GlobalConfig.partitionNum];
        volP = new int[GlobalConfig.partitionNum];
    }

    public void streamingPartitioning() {
        graph.readGraphFromFile();
        Edge edge;
        //First Time
        while((edge = graph.readStep()) != null) {
            int first = edge.getSrcVId();
            int sec = edge.getDestVId();
            d[first] ++;
            d[sec] ++;
            getMaxsizeAndMinSize();
            int pStar = getBestPartition(d[first], d[sec]);
            v2p[first][pStar] = 1;
            v2p[sec][pStar] = 1;
            volP[pStar] += 1;
        }
    }

    private void getMaxsizeAndMinSize() {
        maxsize = 0;
        minsize = Integer.MAX_VALUE;
        for(int i = 0; i < GlobalConfig.partitionNum; i++) {
            if(volP[i] > maxsize) maxsize = volP[i];
            if(volP[i] < minsize) minsize = volP[i];
        }
    }

    private int getBestPartition(int vi, int vj) {
        int bestP = 0;
        double bestScore = 0;
        for(int p = 0; p < GlobalConfig.partitionNum; p++) {
            double score = getScore(vi, vj, p);
            if(score > bestScore) {
                bestScore = score;
                bestP = p;
            }
        }
        return bestP;
    }

    private double getScore(int vi, int vj, int p) {
        double repScore = getRepScore(vi, vj, p);
        double balScore = getBalScore(p);
        return repScore + balScore;
    }

    private double getBalScore(int p) {
        return GlobalConfig.getLambda() * (maxsize - volP[p] / (GlobalConfig.getEpsilon() + maxsize - minsize));
    }

    private double getRepScore(int vi, int vj, int p) {
        return g(vi, p, (double) d[vi] / (d[vi] + d[vj])) + g(vj, p, (double) d[vj] / (d[vi] + d[vj]));
    }

    private double g(int v, int p, double thetaV) {
        if(v2p[v][p] == 1) {
            return 1.0 + (1.0 - thetaV);
        }
        return 0.0;
    }
    public double getReplicateFactor() {
        int replicateTotal = 0;
        for (int[] ints : v2p) {
            for (int anInt : ints) {
                replicateTotal += anInt;
            }
        }
        return (double) replicateTotal / GlobalConfig.vCount;
    }


    public double getLoadBalance() {
        double maxLoad = 0.0;
        for (int i = 0; i < GlobalConfig.getPartitionNum(); i++) {
            if (maxLoad < volP[i]) {
                maxLoad = volP[i];
            }
        }
        return (double) GlobalConfig.getPartitionNum() / GlobalConfig.getECount() * maxLoad;
    }
}
