package ustc.ddl.main;

import ustc.ddl.cluster.StreamCluster;
import ustc.ddl.game.ClusterPackGame;
import ustc.ddl.graph.Graph;
import ustc.ddl.graph.OriginGraph;
import ustc.ddl.partitioner.CluSP;
import ustc.ddl.properties.GlobalConfig;
import ustc.ddl.thread.ClusterGameTask;
import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.concurrent.*;

public class Clugp {

    static Graph graph = new OriginGraph();
    static HashMap<Integer, Integer> clusterPartition = new HashMap<>();
    static int roundCnt;
    private static long gameStartTime;
    private static long gameEndTime;
    private static int taskNum;
    private static int clusterSize;
    public static void main(String[] args) {
        // initParaInfoFromConsole(args);
        printParaInfo();
        Runtime.getRuntime().gc();
        System.out.println("---------------start-------------");
        //Record start Mem and Time
        long beforeUsedMem = Runtime.getRuntime().totalMemory();
        long startTime = System.currentTimeMillis();
        //with cluster
        StreamCluster streamCluster = new StreamCluster(graph);
        streamCluster.startSteamCluster();
        List<Integer> clusterList = streamCluster.getClusterList();
        parallelGame(streamCluster, clusterList);
        // start streaming partition
        CluSP cluSP = new CluSP(graph, streamCluster, clusterPartition);
        cluSP.performStep();
        long endTime = System.currentTimeMillis();
        double rf = cluSP.getReplicateFactor();
        double lb = cluSP.getLoadBalance();
        // free unused mem
        graph.clear();
        cluSP.clear();
        System.gc();
        long afterUsedMem = Runtime.getRuntime().freeMemory();
        long memoryUsed = (beforeUsedMem - afterUsedMem) >> 20;
        System.out.println("Partition num:" + GlobalConfig.getPartitionNum());
        System.out.println("Partition time: " + (endTime - startTime) + " ms");
        System.out.println("Relative balance load:" + lb);
        System.out.println("Replicate factor: " + rf);
        System.out.println("Memory cost: " + memoryUsed + " MB");
        System.out.println("Total game round:" + roundCnt);
        System.out.println("Cluster game time: " + (gameEndTime - gameStartTime) + " ms");
        System.out.println("---------------end-------------");
    }

    /**
     * Game
     * @param streamCluster
     * @param clusterList
     */
    private static void parallelGame(StreamCluster streamCluster, List<Integer> clusterList) {
        // parallel game theory
        ExecutorService taskPool = Executors.newFixedThreadPool(GlobalConfig.getThreads());
        CompletionService<ClusterPackGame> completionService = new ExecutorCompletionService<>(taskPool);
        clusterSize = clusterList.size();
        System.out.println("Cluster Size: " + clusterSize);
        taskNum = (clusterSize + GlobalConfig.getBatchSize() - 1) / GlobalConfig.getBatchSize();
        System.out.println("TaskNum: " + taskNum);
        System.out.println("Cluster num: " + clusterSize);
        gameStartTime = System.currentTimeMillis();
        for(int i = 0; i < taskNum; i++){
            completionService.submit(new ClusterGameTask(streamCluster, i));
        }
        for(int i = 0; i < taskNum; i++){
            try{
                Future<ClusterPackGame> result = completionService.take();
                ClusterPackGame game = result.get();
                clusterPartition.putAll(game.getClusterPartition());
                roundCnt += game.getRoundCnt();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        taskPool.shutdownNow();
        gameEndTime = System.currentTimeMillis();
    }

    /**
     * init param, if not need comment the function in Main (eg. use properties)
     * @param args
     */
    private static void initParaInfoFromConsole(String[] args) {
        GlobalConfig.vCount = Integer.parseInt(args[0]);
        GlobalConfig.eCount = Integer.parseInt(args[1]);
        GlobalConfig.inputGraphPath = args[2];
        GlobalConfig.threads = Integer.parseInt(args[3]);
        GlobalConfig.partitionNum = Integer.parseInt(args[4]);
        GlobalConfig.batchSize = Integer.parseInt(args[5]);
        GlobalConfig.outputGraphPath = args[6];
    }

    /**
     * Print Parameters information
     */
    private static void printParaInfo() {
        System.out.println("input graph: " + GlobalConfig.inputGraphPath);
        System.out.println("outputGraphPath: " + GlobalConfig.outputGraphPath);
        System.out.println("hashNum: " + GlobalConfig.hashNum);
        System.out.println("compressionRate: " + GlobalConfig.compressionRate);
        System.out.println("vCount: " + GlobalConfig.vCount);
        System.out.println("eCount: " + GlobalConfig.eCount);
        System.out.println("partitionNum: " + GlobalConfig.partitionNum);
        System.out.println("alpha: " + GlobalConfig.alpha);
        System.out.println("k: " + GlobalConfig.k);
        System.out.println("l: " + GlobalConfig.l);
        System.out.println("batchSize: " + GlobalConfig.batchSize);
        System.out.println("partitionNum: " + GlobalConfig.partitionNum);
        System.out.println("threads: " + GlobalConfig.threads);
        System.out.println("MaxClusterVolume: " + GlobalConfig.getMaxClusterVolume());
    }
}

//    List<Integer> clusterList = streamCluster.getClusterList();
//    //!!! add phase to process the cluster
//    //        streamCluster.PrintInfomation();
//    //        FileProcess fp = new FileProcess(streamCluster);
//    //        fp.savefile();
//
//    ////without cluster
//    //        StreamCluster streamCluster = new StreamCluster(graph);
//    //        //streamCluster.startSteamCluster();
//    //        FileProcess fp = new FileProcess(streamCluster);
//    //        fp.readfile();
//    //        streamCluster = fp.getstreamCluster();
//    //        List<Integer> clusterList = streamCluster.getClusterList();
//    //        //streamCluster.PrintInfomation();
//    //        streamCluster.reprocess();
//    parallelGame(streamCluster, clusterList);