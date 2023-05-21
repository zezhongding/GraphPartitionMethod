package ustc.ddl.main;

import ustc.ddl.graph.Graph;
import ustc.ddl.graph.OriginGraph;
import ustc.ddl.partitioner.Partitioner;
import ustc.ddl.properties.GlobalConfig;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintStream;

public class Hdrf {
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
            // HDRF process
            Partitioner partitioner = new Partitioner(graph);
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
            System.out.println("vCount: " + GlobalConfig.vCount);
            System.out.println("eCount: " + GlobalConfig.eCount);
            System.out.println("averageDegree: " + GlobalConfig.getAverageDegree());
            System.out.println("partitionNum: " + GlobalConfig.partitionNum);
            System.out.println("lambda: " + GlobalConfig.lambda);
            System.out.println("epsilon: " + GlobalConfig.epsilon);
            System.out.println("partitionNum: " + GlobalConfig.partitionNum);
        }
}
