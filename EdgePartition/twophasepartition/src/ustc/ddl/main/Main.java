package ustc.ddl.main;

import ustc.ddl.cluster.StreamCluster;
import ustc.ddl.partitioner.Partitioner;
import ustc.ddl.properties.GlobalConfig;
import ustc.ddl.graph.*;
import java.io.*;


public class Main {
    static Graph graph = new OriginGraph(GlobalConfig.inputGraphPath);
    public static void main(String[] args) throws Exception {
        PrintStream printStream = new PrintStream(new FileOutputStream(GlobalConfig.outputResultPath, true));
        // 将System.out的输出重定向到PrintStream中
        PrintStream console = System.out;
        System.setOut(new PrintStream(new TeeOutputStream(console, printStream)));
        // initParaInfoFromConsole(args);
        printParaInfo();
        Runtime.getRuntime().gc();
        System.out.println("---------------start-------------");
        //Record start Mem and Time
        long beforeUsedMem = Runtime.getRuntime().totalMemory();
        long startTime = System.currentTimeMillis();
        //Phase 1
        StreamCluster streamCluster = new StreamCluster(graph);
        streamCluster.startStreamingClustering();
        //Phase 2
        Partitioner partitioner = new Partitioner(streamCluster.getDegree(), streamCluster.getVol(), streamCluster.getV2c(), graph);
        partitioner.streamingPartitioning();
        long endTime = System.currentTimeMillis();
        double rf = partitioner.getReplicateFactor();
        double lb = partitioner.getLoadBalance();
        graph.clear();
        System.gc();
        long afterUsedMem = Runtime.getRuntime().freeMemory();
        long memoryUsed = (beforeUsedMem - afterUsedMem) >> 20;
        System.out.println("Partition num:" + GlobalConfig.getPartitionNum());
        System.out.println("Partition time: " + (endTime - startTime) + " ms");
        System.out.println("Relative balance load:" + lb);
        System.out.println("Replicate factor: " + rf);
        System.out.println("Memory cost: " + memoryUsed + " MB");
        System.out.println("---------------end-------------");
    }

    public static class TeeOutputStream extends OutputStream {
        private OutputStream out1;
        private OutputStream out2;

        public TeeOutputStream(OutputStream out1, OutputStream out2) {
            this.out1 = out1;
            this.out2 = out2;
        }

        @Override
        public void write(int b) throws IOException {
            out1.write(b);
            out2.write(b);
        }
    }
    private static void printParaInfo() {
        System.out.println("input graph: " + GlobalConfig.inputGraphPath);
//        System.out.println("outputGraphPath: " + GlobalConfig.outputGraphPath);
//        System.out.println("hashNum: " + GlobalConfig.hashNum);
        // System.out.println("compressionRate: " + GlobalConfig.compressionRate);
        System.out.println("vCount: " + GlobalConfig.vCount);
        System.out.println("eCount: " + GlobalConfig.eCount);
        System.out.println("averageDegree: " + GlobalConfig.getAverageDegree());
        System.out.println("partitionNum: " + GlobalConfig.partitionNum);
        System.out.println("alpha: " + GlobalConfig.alpha);
//        System.out.println("beta: " + GlobalConfig.beta);
//        System.out.println("k: " + GlobalConfig.k);
        //System.out.println("l: " + GlobalConfig.l);
//        System.out.println("batchSize: " + GlobalConfig.batchSize);
        System.out.println("partitionNum: " + GlobalConfig.partitionNum);
//        System.out.println("threads: " + GlobalConfig.threads);
//        System.out.println("MaxClusterVolume: " + GlobalConfig.getMaxClusterVolume());
    }
}



