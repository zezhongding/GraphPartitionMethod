package ustc.ddl.cluster;

import ustc.ddl.graph.Graph;
import ustc.ddl.core.Edge;
import ustc.ddl.properties.GlobalConfig;

import java.util.*;
import java.lang.Math;
public class StreamCluster {

    private int[] cluster;
    private final int[] degree;
    private final HashMap<Integer, Integer> volume;
    // clusterId1 = clusterId2 save inner, otherwise save cut
    private final HashMap<Integer, HashMap<Integer, Integer>> innerAndCutEdge;
    private final Graph graph;
    private  List<Integer> clusterList;
    private final int maxVolume;
    //private final HashMap<Integer, Integer> clusterReorder;


    public StreamCluster(Graph graph) {
        this.cluster = new int[GlobalConfig.vCount];
        this.graph = graph;
        this.volume = new HashMap<>(); //capabilities of cluster key = id
        this.maxVolume = GlobalConfig.getMaxClusterVolume();
        this.innerAndCutEdge = new HashMap<>();
        this.clusterList = new ArrayList<>();
        this.degree = new int[GlobalConfig.vCount];
    }

    private void combineCluster(int srcVid, int destVid) {
        if (volume.get(cluster[srcVid]) >= maxVolume || volume.get(cluster[destVid]) >= maxVolume) return;

        int minVid = (volume.get(cluster[srcVid]) < volume.get(cluster[destVid]) ? srcVid : destVid);
        int maxVid = (srcVid == minVid ? destVid : srcVid);

        if ((volume.get(cluster[maxVid]) + this.degree[minVid]) <= maxVolume) {
            volume.put(cluster[maxVid], volume.get(cluster[maxVid]) + this.degree[minVid]);
            volume.put(cluster[minVid], volume.get(cluster[minVid]) - this.degree[minVid]);
            if (volume.get(cluster[minVid]) == 0) volume.remove(cluster[minVid]);
            cluster[minVid] = cluster[maxVid];
        }
    }

    public void startSteamCluster() {
        double averageDegree = 0.0;
        double numsEdge = 0;
        double numsNode = 0;
        int clusterID = 1;

        graph.readGraphFromFile();
        Edge edge;
        //First Time
        while((edge = graph.readStep()) != null) {
            numsEdge += 2;
            int src = edge.getSrcVId();
            int dest = edge.getDestVId();
            // allocate cluster
            if (cluster[src] == 0) {
                cluster[src] = clusterID++;
                numsNode += 1;
            }
            if (cluster[dest] == 0) {
                cluster[dest] = clusterID++;
                numsNode+=1;
            }
            averageDegree = 2.0 * numsEdge / numsNode;
            if(averageDegree < 0 ) System.out.println("Error!!! In StreamCluster average degree < 0");
            this.degree[src]++;
            this.degree[dest]++;

            // update volume
            if (!volume.containsKey(cluster[src])) {
                volume.put(cluster[src], 0);
            }
            if (!volume.containsKey(cluster[dest])) {
                volume.put(cluster[dest], 0);
            }
            volume.put(cluster[src], volume.get(cluster[src]) + 1);
            volume.put(cluster[dest], volume.get(cluster[dest]) + 1);

            if(volume.get(cluster[src]) >= maxVolume)
            {
                volume.put(cluster[src], volume.get(cluster[src]) - this.degree[src]);
                cluster[src] = clusterID++;
                volume.put(cluster[src], this.degree[src]);
            }

            if(volume.get(cluster[dest]) >= maxVolume)
            {
                volume.put(cluster[dest], volume.get(cluster[dest]) - this.degree[dest]);
                cluster[dest] = clusterID++;
                volume.put(cluster[dest], this.degree[dest]);
            }

            // combine cluster
            combineCluster(src, dest);
        }

        setUpIndex();
        computeEdgeInfo();

        // PrintInfomation();
    }

    /**
     * all get cluster IDs
     */
    private void setUpIndex() {
        // sort the volume of the cluster
        List<HashMap.Entry<Integer, Integer>> sortList = new ArrayList<HashMap.Entry<Integer, Integer>>(volume.entrySet());
//        Collections.sort(sortList, (v1, v2) -> (v2.getValue() - v1.getValue()));
        for(int i = 0; i < sortList.size(); i++){
            if (sortList.get(i).getValue() == 0) {
                continue;
            }
            this.clusterList.add(sortList.get(i).getKey());
        }
        //print
//        for(int i = 0; i < sortList.size(); i++){
//            System.out.println("clusterList: " + i + ": " + sortList.get(i).getKey() + "," + sortList.get(i).getValue());
//        }
        volume.clear();
        System.gc();
    }

    public void computeEdgeInfo() {
        // compute inner and cut edge
        //Second Time
        graph.readGraphFromFile();
        HashMap<Integer, HashSet<Integer>> replicateTable = new HashMap<>();
        Edge edge;
        double sum = 0.0;
        while((edge = graph.readStep()) != null){
            int src = edge.getSrcVId();
            int dest = edge.getDestVId();
            if(cluster[src] != cluster[dest]) sum++;
            if (!innerAndCutEdge.containsKey(cluster[src]))
                innerAndCutEdge.put(cluster[src], new HashMap<>());
            if (!innerAndCutEdge.get(cluster[src]).containsKey(cluster[dest]))
                innerAndCutEdge.get(cluster[src]).put(cluster[dest], 0);
            int oldValue = innerAndCutEdge.get(cluster[src]).get(cluster[dest]);
            innerAndCutEdge.get(cluster[src]).put(cluster[dest], oldValue + edge.getWeight());
        }

        System.out.println("Cluster rep: " + (sum + GlobalConfig.getVCount()) / GlobalConfig.getVCount());
    }


    public void PrintInfomation(){
        for(int i = 0; i < GlobalConfig.vCount; ++i) {
            System.out.println("vertex: " + i + ",clusterID: " + cluster[i]);
        }
        for(int i = 0; i < clusterList.size(); i++){
            System.out.println("clusterList: " + i + ": " + clusterList.get(i));
        }

        innerAndCutEdge.forEach((key1, value1) -> {

            System.out.print(key1.toString());
            System.out.print(" ");
            value1.forEach((key2, value2) ->{
                System.out.print(key2.toString());
                System.out.print(" ");
                System.out.print(value2.toString());
                System.out.print(" ");
            });
            System.out.print("\n");
        });

    }
    public List<Integer> getClusterList() {
        return clusterList;
    }


    public int[]  getCluster() {
        return cluster;
    }

    public HashMap<Integer, HashMap<Integer, Integer>> getInnerAndCutEdge() {
        return innerAndCutEdge;
    }

    public int getEdgeNum(int cluster1, int cluster2) {
        if(!innerAndCutEdge.containsKey(cluster1) || !innerAndCutEdge.get(cluster1).containsKey(cluster2)) return 0;

        return innerAndCutEdge.get(cluster1).get(cluster2);
    }

    @Override
    public String toString() {
        StringBuilder volumeStr = new StringBuilder();
        StringBuilder clusterStr = new StringBuilder();
        volume.forEach((k, v) -> {
            volumeStr.append("cluster ").append(k).append(" volume: ").append(v).append("\n");
        });

        for (int i = 0; i < graph.getVCount(); i++) {
            clusterStr.append("vid : ").append(i).append(" cluster: ").append(cluster[i]).append("\n");
        }

        return volumeStr.toString() + clusterStr.toString();
    }

    public int getClusterId(int vId) {
        return cluster[vId];
    }

    public HashMap<Integer, Integer> getVolume() {
        return volume;
    }

    public void changeCluster(int [] cluster) {
        this.cluster = cluster;
    }

    public void changeClusterList(List<Integer> clusterList) {
        this.clusterList = clusterList;
    }


    public void reprocess() {

//        graph.readGraphFromFile();
//
//        Edge edge;
//        while((edge = graph.readStep()) != null) {
//            int src = edge.getSrcVId();
//            int dest = edge.getDestVId();
//
//            this.degree[src]++;
//            this.degree[dest]++;
//
//        }

        //setUpIndex();
        computeEdgeInfo();
    }
}
